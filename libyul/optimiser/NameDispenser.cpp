/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTYwithout even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * Optimiser component that can create new unique names.
 */

#include <libyul/optimiser/NameDispenser.h>

#include <libyul/optimiser/NameCollector.h>
#include <libyul/AsmData.h>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace dev;
using namespace yul;

NameDispenser::NameDispenser(Block const& _ast):
	NameDispenser(NameCollector(_ast).names())
{
}

NameDispenser::NameDispenser(set<YulString> _usedNames)
{
	m_counters[YulString()] = 1;
	for (auto name: _usedNames)
	{
		auto split = splitPrefixSuffix(name);
		auto& counter = m_counters[split.first];
		if (split.second + 1 > counter)
			counter = split.second + 1;
	}
}

YulString NameDispenser::newName(YulString _nameHint, YulString _context)
{
	auto split = splitPrefixSuffix(_nameHint, true);

	if (!_context.empty())
		return newNameInternal(YulString(_context.str().substr(0, 10) + '_' + split.first.str()));
	else
		return newNameInternal(split.first);
}

YulString NameDispenser::newNameInternal(YulString _nameHint)
{
	auto counter = m_counters[_nameHint]++;
	return counter > 0 ? YulString{_nameHint.str() + '_' + std::to_string(counter) } : _nameHint;
}

std::pair<YulString, size_t> NameDispenser::splitPrefixSuffix(yul::YulString _string, bool _ignoreSuffixValue)
{
	if (_string.empty())
		return { _string, 0 };
	else
	{
		std::string const& str = _string.str();

		size_t suffix = 0;

		size_t nameLength = str.size();

		size_t digits = 0;
		for (--nameLength; nameLength > 0 && std::isdigit(str[nameLength]); --nameLength)
			if (++digits > 9)
				return { _string, 0 };

		if (str[nameLength] == '_')
		{
			if (!_ignoreSuffixValue && digits > 0)
				suffix = boost::lexical_cast<std::size_t>(str.data() + nameLength + 1, digits);

			while(nameLength > 0 && str[nameLength - 1] == '_') --nameLength;

			return { YulString(str.substr(0, nameLength)), suffix };
		}
		else
			return { _string, 0 };
	}
}
