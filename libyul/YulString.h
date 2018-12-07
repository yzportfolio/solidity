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
//	32-bit hashes are slower on 64-bit systems, but may be faster with Emscripten/asm.js
//	using HashType = std::uint32_t;
//	static constexpr std::uint32_t emptyHash() { return 2166136261u; }
//	static constexpr std::uint32_t fnvPrime() { return 16777619u; }
	using HashType = std::uint64_t;
	static constexpr HashType emptyHash() { return 14695981039346656037u; }
	static constexpr HashType fnvPrime() { return 14695981039346656037u; }
	using SuffixType = std::uint64_t;

	class StringData
	{
	public:
		StringData(SuffixType _suffix, std::string _prefix): m_suffix(_suffix), m_prefix(std::move(_prefix)) {}
		SuffixType suffix() const { return m_suffix; }
		std::string const& prefix() const { return m_prefix; }
		std::string const& fullString() const
		{
			if (m_fullString.empty())
				m_fullString = m_suffix > 0 ? m_prefix + '_' + std::to_string(m_suffix) : m_prefix;
			return m_fullString;
		}
		bool operator<(StringData const& _rhs) const
		{
			if (m_suffix < _rhs.m_suffix)
				return true;
			if (_rhs.m_suffix < m_suffix)
				return false;
			return m_prefix < _rhs.m_prefix;
		}
	private:
		SuffixType m_suffix;
		std::string m_prefix;
		mutable std::string m_fullString;
	};
	using StringHandle = StringData const*;

	YulStringRepository() {}
	static YulStringRepository& instance()
	{
		static YulStringRepository inst;
		return inst;
	}

	std::pair<StringHandle, HashType> stringHandleAndHash(std::string const& _string, SuffixType _suffix)
	{
		if (_string.empty() && !_suffix)
			return { nullptr, emptyHash() };
		HashType hash = emptyHash();
		SuffixType realSuffix = 0;
		std::size_t prefixLength = _string.size();
		std::tie(hash, realSuffix, prefixLength) = getHashAndRealSuffixAndPrefixLength(_string, _suffix);

		auto range = m_hashToStringHandle.equal_range(hash);
		for (auto it = range.first; it != range.second; ++it)
			if (!it->second->prefix().compare(0, it->second->prefix().size(), _string, 0, prefixLength) && it->second->suffix() == realSuffix)
				return { it->second, hash };

		m_stringDataStore.emplace_front(StringData{realSuffix, _string.substr(0, prefixLength)});
		auto handle = &m_stringDataStore.front();
		m_hashToStringHandle.emplace_hint(range.second, std::make_pair(hash, handle));
		return { handle, hash };
	}
private:
	static constexpr SuffixType maxSuffix = 1000000000u;
	static constexpr int maxDigits(SuffixType helper = maxSuffix) {
		return helper > 1 ? 1 + maxDigits(helper / 10) : 0;
	}

	static std::tuple<HashType, SuffixType, std::size_t> getHashAndRealSuffixAndPrefixLength(
		std::string const& _string,
		SuffixType _suffix
	)
	{
		HashType hash = emptyHash();
		SuffixType realSuffix = 0;
		std::size_t prefixLength = _string.size();
		auto it = _string.rbegin();

		if (_suffix)
		{
			if (_suffix < maxSuffix)
				realSuffix = _suffix;
			else
			{
				SuffixType stringifiedSuffix = _suffix / maxSuffix;
				_suffix %= maxSuffix;
				while (stringifiedSuffix)
				{
					hash ^= static_cast<std::uint8_t>('0' + (stringifiedSuffix % 10));
					hash *= fnvPrime();
					stringifiedSuffix /= 10;
				}
			}
		}
		else if (!_suffix)
		{
			int digits = 0;
			for (; it != _string.rend() && '0' <= *it && *it <= '9' && digits < maxDigits(); ++it, ++digits)
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

		for(; it != _string.rend(); ++it)
		{
			hash ^= static_cast<std::uint8_t>(*it);
			hash *= fnvPrime();
		}

		for(auto s = _suffix; s; s >>= 8)
		{
			hash ^= static_cast<std::uint8_t>(s);
			hash *= fnvPrime();
		}

		return { hash, realSuffix, prefixLength };
	}

	std::forward_list<StringData> m_stringDataStore;
	std::unordered_multimap<HashType, StringHandle> m_hashToStringHandle;
};

/// Wrapper around handles into the YulString repository.
/// Equality of two YulStrings is determined by comparing their ID.
/// The <-operator depends on the string hash and is not consistent
/// with string comparisons (however, it is still deterministic).
class YulString
{
public:
	using SuffixType = YulStringRepository::SuffixType;

	YulString() = default;
	explicit YulString(std::string const& _s, SuffixType _suffix = 0)
	{
		std::tie(m_handle, m_hash) = YulStringRepository::instance().stringHandleAndHash(_s, _suffix);
	}
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
		if (m_hash < _other.m_hash)
			return true;
		if (_other.m_hash < m_hash)
			return false;
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
	SuffixType suffix() const
	{
		if (m_handle)
			return m_handle->suffix();
		else
			return 0;
	}
private:
	/// Handle of the string. Assumes that the empty string has ID zero.
	YulStringRepository::StringHandle m_handle = nullptr;
	YulStringRepository::HashType m_hash = YulStringRepository::emptyHash();
};

}
