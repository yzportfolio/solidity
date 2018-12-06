/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * String abstraction that avoids copies.
 */

#pragma once

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <forward_list>

namespace yul
{

class YulString;

/// Repository for YulStrings.
/// Owns the string data and hashes for the prefixes of all YulStrings, which can be referenced by a PrefixHandle.
class YulStringRepository: boost::noncopyable
{
public:
	static constexpr std::uint64_t emptyHash() { return 14695981039346656037u; }
	static constexpr std::uint64_t fnvPrime() { return 14695981039346656037u; }

	struct PrefixData
	{
		PrefixData(std::string::const_iterator _begin, std::string::const_iterator _end, std::uint64_t _hash)
			: prefix(_begin, _end), hash(_hash) {}
		std::string prefix;
		std::uint64_t hash;
		bool operator<(PrefixData const& _rhs) const
		{
			if (hash < _rhs.hash) return true;
			if (_rhs.hash < hash) return false;
			return prefix < _rhs.prefix;
		}
	};
	using PrefixHandle = PrefixData const*;

	YulStringRepository()
	{
	}
	static YulStringRepository& instance()
	{
		static YulStringRepository inst;
		return inst;
	}
	PrefixHandle prefixHandle(std::string const& _string)
	{
		if (_string.empty())
			return nullptr;

		std::uint64_t hash = emptyHash();
		for (auto it = _string.rbegin(); it != _string.rend(); ++it)
		{
			hash *= fnvPrime();
			hash ^= *it;
		}

		return prefixHandle(_string, _string.size(), hash);
	}
	PrefixHandle prefixHandle(std::string const& _string, std::size_t _prefixLength, std::uint64_t _hash)
	{
		if (!_prefixLength)
			return nullptr;
		auto range = m_hashToPrefixHandle.equal_range(_hash);
		for (auto it = range.first; it != range.second; ++it)
			if (!it->second->prefix.compare(0, it->second->prefix.size(), _string, 0, _prefixLength))
				return it->second;
		m_strings.emplace_front(_string.begin(), _string.begin() + _prefixLength, _hash);
		auto handle = &m_strings.front();
		m_hashToPrefixHandle.emplace_hint(range.second, std::make_pair(_hash, handle));
		return handle;
	}
	std::tuple<PrefixHandle, std::uint64_t> splitPrefixAndSuffix(std::string const& _string)
	{
		if (_string.empty())
			return { nullptr, 0 };

		auto prefixLength = _string.size();
		auto it = _string.rbegin();

		std::uint64_t hash = emptyHash();
		std::uint64_t suffix = 0;

		// TODO: need to prevent suffix overflows
		for (; it != _string.rend() && '0' <= *it && *it <= '9'; ++it)
		{
			suffix *= 10;
			suffix += *it - '0';
			hash *= fnvPrime();
			hash ^= *it;
		}
		if (it == _string.rend() || *it != '_')
			suffix = 0;
		else if (suffix)
		{
			++it;
			hash = emptyHash();
			prefixLength = static_cast<std::size_t>(_string.rend() - it);
		}

		for (;it != _string.rend(); ++it)
		{
			hash *= fnvPrime();
			hash ^= *it;
		}

		return { prefixHandle(_string, prefixLength, hash), suffix };
	}
private:
	std::forward_list<PrefixData> m_strings;
	std::unordered_multimap<std::uint64_t, PrefixHandle> m_hashToPrefixHandle;
};

/// Wrapper around handles into the YulString repository.
/// Equality of two YulStrings is determined by comparing their ID.
/// The <-operator depends on the string hash and is not consistent
/// with string comparisons (however, it is still deterministic).
class YulString
{
public:
	YulString() = default;
	explicit YulString(std::string const& _s, std::uint64_t _suffix = 0): m_suffix(_suffix) {
		if (_suffix)
			m_prefixHandle = YulStringRepository::instance().prefixHandle(_s);
		else
			std::tie(m_prefixHandle, m_suffix) = YulStringRepository::instance().splitPrefixAndSuffix(_s);
	}
	YulString(YulString const&) = default;
	YulString(YulString&&) = default;
	YulString& operator=(YulString const&) = default;
	YulString& operator=(YulString&&) = default;

	/// This is not consistent with the string <-operator!
	/// If prefixes are identical, compare suffices.
	/// If one prefix is empty and one is not, the empty one is smaller.
	/// Otherwise compare according to the prefix data (compare prefix hashes and fall back to prefix strings).
	bool operator<(YulString const& _other) const
	{
		if (m_prefixHandle == _other.m_prefixHandle)
			return m_suffix < _other.m_suffix;
		if (!m_prefixHandle || !_other.m_prefixHandle)
			return _other.m_prefixHandle;
		return *m_prefixHandle < *_other.m_prefixHandle;
	}
	/// Equality is determined based on prefix handle and suffix.
	bool operator==(YulString const& _other) const
	{
		return m_prefixHandle == _other.m_prefixHandle && m_suffix == _other.m_suffix;
	}
	bool operator!=(YulString const& _other) const { return !operator==(_other); }

	bool empty() const { return !m_prefixHandle && !m_suffix; }
	std::string str() const
	{
		if (m_prefixHandle)
			return m_suffix > 0 ? m_prefixHandle->prefix + '_' + std::to_string(m_suffix) : m_prefixHandle->prefix;
		else
			return m_suffix > 0 ? '_' + std::to_string(m_suffix) : "";
	}
	std::string const& prefix() const
	{
		if (m_prefixHandle)
			return m_prefixHandle->prefix;
		else
		{
			static std::string emptyString;
			return emptyString;
		}
	}
	std::uint64_t suffix() const
	{
		return m_suffix;
	}
	YulStringRepository::PrefixHandle prefixHandle() const
	{
		return m_prefixHandle;
	}
private:
	/// Handle of the string. Assumes that the empty string has ID zero.
	YulStringRepository::PrefixHandle m_prefixHandle = nullptr;
	std::uint64_t m_suffix = 0;
};

}
