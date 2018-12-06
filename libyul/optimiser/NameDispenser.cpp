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
	m_counters[nullptr] = 1;
	for (auto name: _usedNames)
	{
		auto& counter = m_counters[name.prefixHandle()];
		if (name.suffix() + 1 > counter)
			counter = name.suffix() + 1;
	}
}

YulString NameDispenser::newName(YulString _nameHint, YulString _context)
{
	if (!_context.empty())
		return newNameInternal(YulString(_context.str().substr(0, 10) + '_' + _nameHint.prefix(), _nameHint.suffix()).prefixHandle());
	else
		return newNameInternal(_nameHint.prefixHandle());
}

YulString NameDispenser::newNameInternal(YulStringRepository::PrefixHandle _prefixHandle)
{
	return YulString{_prefixHandle ? _prefixHandle->prefix : "", m_counters[_prefixHandle]++ };
}
