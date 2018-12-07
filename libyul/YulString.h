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

	class StringData
	{
	public:
		StringData(std::uint64_t _hash, std::uint64_t _suffix, std::string _prefix):
		m_hash(_hash), m_suffix(_suffix), m_prefix(std::move(_prefix)) {}
		std::uint64_t hash() const { return m_hash; }
		std::uint64_t suffix() const { return m_suffix; }
		std::string const& prefix() const { return m_prefix; }
		std::string const& fullString() const
		{
			if (m_fullString.empty())
				m_fullString = m_suffix > 0 ? m_prefix + '_' + std::to_string(m_suffix) : m_prefix;
			return m_fullString;
		}
		bool operator<(StringData const& _rhs) const
		{
			if (m_hash < _rhs.m_hash)
				return true;
			if (_rhs.m_hash < m_hash)
				return false;
			if (m_suffix < _rhs.m_suffix)
				return true;
			if (_rhs.m_suffix < m_suffix)
				return false;
			return m_prefix < _rhs.m_prefix;
		}
	private:
		std::uint64_t m_hash;
		std::uint64_t m_suffix;
		std::string m_prefix;
		mutable std::string m_fullString;
	};
	using StringHandle = StringData const*;

	YulStringRepository()
	{
	}
	static YulStringRepository& instance()
	{
		static YulStringRepository inst;
		return inst;
	}

	static std::tuple<std::uint64_t, std::uint64_t, std::uint64_t> getHashAndRealSuffixAndPrefixLength(std::string const& _string, std::uint64_t _suffix)
	{
		std::uint64_t hash = emptyHash();
		std::uint64_t realSuffix = 0;
		std::uint64_t prefixLength = _string.size();
		auto it = _string.rbegin();

		if (_suffix)
			realSuffix = _suffix;
		else
		{
			for (; it != _string.rend() && '0' <= *it && *it <= '9'; ++it)
			{
				_suffix *= 10;
				_suffix += *it - '0';
			}
			if (it != _string.rend() && *it == '_')
			{
				++it;
				realSuffix = _suffix;
			}
		}

		if (realSuffix)
			prefixLength = _string.rend() - it;

		for(auto s = prefixLength; s; s >>= 8)
		{
			hash *= fnvPrime();
			hash ^= s & 0xFF;
		}

		for(auto s = _suffix; s; s >>= 8)
		{
			hash *= fnvPrime();
			hash ^= s & 0xFF;
		}

		for(; it != _string.rend(); ++it)
		{
			hash *= fnvPrime();
			hash ^= *it;
		}

		return { hash, realSuffix, prefixLength };
	}

	StringHandle stringHandle(std::string const& _string, std::uint64_t _suffix)
	{
		if (_string.empty() && !_suffix)
			return nullptr;
		std::uint64_t hash = emptyHash();
		std::uint64_t realSuffix = 0;
		std::uint64_t prefixLength = _string.size();
		std::tie(hash, realSuffix, prefixLength) = getHashAndRealSuffixAndPrefixLength(_string, _suffix);

		auto range = m_hashToStringHandle.equal_range(hash);
		for (auto it = range.first; it != range.second; ++it)
			if (!it->second->prefix().compare(0, it->second->prefix().size(), _string, 0, prefixLength) && it->second->suffix() == realSuffix)
				return it->second;

		m_stringDataStore.emplace_front(StringData{hash, realSuffix, _string.substr(0, prefixLength)});
		auto handle = &m_stringDataStore.front();
		m_hashToStringHandle.emplace_hint(range.second, std::make_pair(hash, handle));
		return handle;
	}
private:
	std::forward_list<StringData> m_stringDataStore;
	std::unordered_multimap<std::uint64_t, StringHandle> m_hashToStringHandle;
};

/// Wrapper around handles into the YulString repository.
/// Equality of two YulStrings is determined by comparing their ID.
/// The <-operator depends on the string hash and is not consistent
/// with string comparisons (however, it is still deterministic).
class YulString
{
public:
	YulString() = default;
	explicit YulString(std::string const& _s, std::uint64_t _suffix = 0):
		m_handle(YulStringRepository::instance().stringHandle(_s, _suffix)) {}
	YulString(YulString const&) = default;
	YulString(YulString&&) = default;
	YulString& operator=(YulString const&) = default;
	YulString& operator=(YulString&&) = default;

	/// This is not consistent with the string <-operator!
	/// If handles are identical, return false.
	/// If one string is empty and one is not, the empty one is smaller.
	/// Otherwise compare the string data.
	bool operator<(YulString const& _other) const
	{
		if (*this == _other)
			return false;
		if (empty() || _other.empty())
			return !_other.empty();
		return *m_handle < *_other.m_handle;
	}
	/// Equality is determined based on handles.
	bool operator==(YulString const& _other) const { return m_handle == _other.m_handle; }
	bool operator!=(YulString const& _other) const { return m_handle != _other.m_handle; }

	bool empty() const { return !m_handle; }
	std::string const& str() const
	{
		if (m_handle)
			return m_handle->fullString();
		else
			return prefix();
	}
	std::string const& prefix() const
	{
		if (m_handle)
			return m_handle->prefix();
		else
		{
			static std::string emptyString;
			return emptyString;
		}
	}
	std::uint64_t suffix() const
	{
		if (m_handle)
			return m_handle->suffix();
		else
			return 0;
	}
private:
	/// Handle of the string. Assumes that the empty string has ID zero.
	YulStringRepository::StringHandle m_handle = nullptr;
};

}
