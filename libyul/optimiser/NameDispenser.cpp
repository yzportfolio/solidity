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

using namespace std;
using namespace dev;
using namespace yul;

NameDispenser::NameDispenser(Block const& _ast):
	NameDispenser(NameCollector(_ast).names())
{
}

NameDispenser::NameDispenser(set<YulString> _usedNames):
	m_usedNames(std::move(_usedNames))
{
}

YulString NameDispenser::newName(YulString _nameHint, YulString _context)
{
	if (!_context.empty())
		return newNameInternal(YulString(_context.prefix().substr(0, 10) + '_' + _nameHint.prefix(), _nameHint.suffix()));
	else
		return newNameInternal(_nameHint);
}

YulString NameDispenser::newNameInternal(YulString _nameHint)
{
	YulString name = _nameHint;

	while(name.empty() || m_usedNames.count(name))
		name = YulString(name.prefix(), name.suffix() + 1);

	m_usedNames.emplace(name);

	return name;
}
