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

#include <boost/algorithm/string.hpp>
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
/// Owns the string data for all YulStrings, which can be referenced by a Handle.
/// A Handle consists of an ID (that depends on the insertion order of YulStrings and is potentially
/// non-deterministic) and a deterministic string hash.
class YulStringRepository: boost::noncopyable
{
public:
	static constexpr std::uint64_t emptyHash() { return 14695981039346656037u; }

	template<typename T>
	static char enc32(T i)
	{
		if (i <= 9) return '0' + static_cast<char>(i);
		else
			return 'A' + static_cast<char>(i - 10);
	}

	struct StringData {
		StringData() {}
		StringData(std::string const& _prefix, std::size_t _suffix, std::uint64_t _hash): prefix(_prefix), suffix(_suffix), hash(_hash) {}
		std::string prefix;
		std::size_t suffix = 0;
		std::uint64_t hash = emptyHash();
		std::string fullString;
		std::string const& str() {
			if (suffix > 0)
			{
				if (fullString.empty())
				{
					fullString = prefix + '_';
					for (std::size_t i = suffix; i; i >>= 5)
					{

						fullString += enc32(i & 0x1F);
					}
				}
				return fullString;
			}
			else
				return prefix;
/*			return suffix > 0 ?
				prefix +  "_" + std::to_string(suffix) :
				prefix;*/
		}
	};

	using Handle = StringData*;

	YulStringRepository()
	{
	}
	static YulStringRepository& instance()
	{
		static YulStringRepository inst;
		return inst;
	}
	bool isEquivalent(std::string const& _prefix, std::size_t _suffix, std::string const& _string)
	{
		if (_string.size() > _prefix.size() + 1 && boost::starts_with(_string, _prefix))
		{
			if (_string[_prefix.size()] == '_')
			{
				for (auto it = _string.cbegin() + _prefix.size() + 1; it != _string.cend() && _suffix; it++)
				{
					if (*it != enc32(_suffix & 0x1F))
						return false;
					_suffix >>= 5;
				}
				return _suffix == 0;
			}
		}
		return false;

	}
	Handle stringToHandle(std::string const& _string, const std::size_t _suffix)
	{
		if (_string.empty() && _suffix == 0)
			return nullptr;
		std::uint64_t h = hash(_string, _suffix);
		auto range = m_hashToHandle.equal_range(h);
		for (auto it = range.first; it != range.second; ++it)
		{
			// if both have suffices, the suffices and prefixes have to match
			if (_suffix && it->second->suffix)
			{
				if (_suffix == it->second->suffix && it->second->prefix == _string)
			 		return it->second;
			}
			// if neither have suffices, the prefixes have to match
			else if (!_suffix && !it->second->suffix)
			{
				if (it->second->prefix == _string)
					return it->second;
			}
			// the first has a suffix and the second has none
			else if (_suffix)
			{
				 if (isEquivalent(_string, _suffix, it->second->prefix))
					 return it->second;
			}
			// the second has a suffix and the first has none
			else if (isEquivalent(it->second->prefix, it->second->suffix, _string))
					return it->second;
		}
		m_strings.emplace_front(_string, _suffix, h);
		auto handle = &m_strings.front();
		m_hashToHandle.emplace_hint(range.second, std::make_pair(h, handle));
		return handle;
	}
	static std::uint64_t hash(std::string const& v, std::size_t s)
	{
		// FNV hash - can be replaced by a better one, e.g. xxhash64
		std::uint64_t hash = emptyHash();
		for (auto c: v)
		{
			hash *= 1099511628211u;
			hash ^= c;
		}
		if (s)
		{
			hash *= 1099511628211u;
			hash ^= '_';
			while (s)
			{
				hash *= 1099511628211u;
				hash ^= enc32(s & 0x1F);
				s >>= 5;
			}
		}

		return hash;
	}
private:
	std::forward_list<StringData> m_strings;
	std::unordered_multimap<std::uint64_t, Handle> m_hashToHandle;
};

/// Wrapper around handles into the YulString repository.
/// Equality of two YulStrings is determined by comparing their ID.
/// The <-operator depends on the string hash and is not consistent
/// with string comparisons (however, it is still deterministic).
class YulString
{
public:
	YulString() = default;
	explicit YulString(std::string const& _s, const std::size_t suffix = 0): m_handle(YulStringRepository::instance().stringToHandle(_s, suffix)) {}
	explicit YulString(YulStringRepository::Handle _handle): m_handle(_handle) {}
	YulString(YulString const&) = default;
	YulString(YulString&&) = default;
	YulString& operator=(YulString const&) = default;
	YulString& operator=(YulString&&) = default;

	/// This is not consistent with the string <-operator!
	/// First compares the string hashes. If they are equal
	/// it checks for identical IDs (only identical strings have
	/// identical IDs and identical strings do not compare as "less").
	/// If the hashes are identical and the strings are distinct, it
	/// falls back to string comparison.
	bool operator<(YulString const& _other) const
	{
		if (!m_handle || !_other.m_handle)
			return _other.m_handle;
		if (m_handle->hash < _other.m_handle->hash) return true;
		if (_other.m_handle->hash < m_handle->hash) return false;

		// if both have no suffix, prefixes have to compare smaller
		if (!m_handle->suffix && !_other.m_handle->suffix)
			return m_handle->prefix < _other.m_handle->prefix;
		// if both have suffices, prefix must compare smaller, or, if equal, suffix must be smaller
		if (m_handle->suffix && _other.m_handle->suffix)
		{
			int result = m_handle->prefix.compare(_other.m_handle->prefix);
			if (result < 0) return true;
			if (result > 0) return false;
			return m_handle->suffix < _other.m_handle->suffix;
		}
		static const auto compare = [](std::string const& _prefix, std::size_t _suffix, std::string const& _str) -> bool
		{
			auto rlen = std::min(_prefix.size(), _str.size());
			int result = _prefix.compare(0, rlen, _str, 0, rlen);
			if (result < 0) return true;
			if (result > 0) return false;
			if (_str.size() <= rlen)
				return false;
			if ('_' < _str[rlen])
				return true;
			if (_str[rlen] < '_')
				return false;
			auto s = _suffix;
			auto i = rlen + 1;
			for(; i < _str.length() && s; s >>= 5, i++)
			{
				char c = YulStringRepository::enc32(s&0x1F);
				if (c < _str[i])
					return true;
				if (_str[i] < c)
					return false;
			}
			if (i == _str.length() && s)
				return true;
			return false;
		};
		if (m_handle->suffix)
			return compare(m_handle->prefix, m_handle->suffix, _other.m_handle->prefix);

		return !compare(_other.m_handle->prefix, _other.m_handle->suffix, m_handle->prefix);
	}
	/// Equality is determined based on the string handle.
	bool operator==(YulString const& _other) const { return m_handle == _other.m_handle; }
	bool operator!=(YulString const& _other) const { return m_handle != _other.m_handle; }

	bool empty() const { return !m_handle; }
	std::string const str() const
	{
		if (m_handle)
			return m_handle->str();
		else
			return "";
	}
	std::string const& prefix() const
	{
		if (m_handle)
			return m_handle->prefix;
		else
		{
			static std::string empty;
			return empty;
		}
	}
	std::size_t suffix() const
	{
		if (m_handle)
			return m_handle->suffix;
		else
			return 0;
	}
	YulStringRepository::Handle handle() const
	{
		return m_handle;
	}
	std::uint64_t hash() const
	{
		if (m_handle)
			return m_handle->hash;
		else
			return YulStringRepository::emptyHash();
	}

private:
	/// Handle of the string. Assumes that the empty string has ID zero.
	YulStringRepository::Handle m_handle = nullptr;
};

}

namespace std {
template <> struct hash<yul::YulString>
{
	size_t operator()(yul::YulString const& _str) const
	{
		return _str.hash();
	}
};
}
