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
 * Simple profiler for the Yul optimizer.
 */

#include <libdevcore/CommonIO.h>
#include <liblangutil/ErrorReporter.h>
#include <liblangutil/Scanner.h>
#include <libyul/AsmAnalysis.h>
#include <libyul/AsmAnalysisInfo.h>
#include <libsolidity/parsing/Parser.h>
#include <libyul/AsmData.h>
#include <libyul/AsmParser.h>
#include <libyul/AsmPrinter.h>
#include <liblangutil/SourceReferenceFormatter.h>

#include <libyul/optimiser/Suite.h>

#include <iostream>
#include <chrono>

using namespace std;
using namespace dev;
using namespace langutil;
using namespace dev::solidity;
using namespace yul;


class YulOpti
{
public:
	void printErrors()
	{
		SourceReferenceFormatter formatter(cout);

		for (auto const& error: m_errors)
			formatter.printExceptionInformation(
				*error,
				(error->type() == Error::Type::Warning) ? "Warning" : "Error"
			);
	}

	bool parse(string const& _input)
	{
		ErrorReporter errorReporter(m_errors);
		shared_ptr<Scanner> scanner = make_shared<Scanner>(CharStream(_input, ""));
		m_ast = yul::Parser(errorReporter, yul::Dialect::strictAssemblyForEVM()).parse(scanner, false);
		if (!m_ast || !errorReporter.errors().empty())
		{
			cout << "Error parsing source." << endl;
			printErrors();
			return false;
		}
		m_analysisInfo = make_shared<yul::AsmAnalysisInfo>();
		AsmAnalyzer analyzer(
			*m_analysisInfo,
			errorReporter,
			EVMVersion::byzantium(),
			boost::none,
			Dialect::strictAssemblyForEVM()
		);
		if (!analyzer.analyze(*m_ast) || !errorReporter.errors().empty())
		{
			cout << "Error analyzing source." << endl;
			printErrors();
			return false;
		}
		return true;
	}

	bool run(string source)
	{
		if (!parse(source))
			return false;
		OptimiserSuite::run(*m_ast, *m_analysisInfo);
		return true;
	}

private:
	ErrorList m_errors;
	shared_ptr<yul::Block> m_ast;
	shared_ptr<AsmAnalysisInfo> m_analysisInfo;
};

int main(int argc, char** argv)
{
	if (argc != 2 && argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " [source file] [runs]" << std::endl;
		return 1;
	}

	const int runs = argc == 3 ? atoi(argv[2]) : 200;

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		YulOpti{}.run(readFileAsString(argv[1]));
		auto endTime = std::chrono::high_resolution_clock::now();

		std::cout << "First run: " << 1000.0 * std::chrono::duration<double>(endTime - startTime).count() << " ms." << std::endl;
	}

	std::chrono::duration<double> accumulatedTime{};

	for (auto i = 0; i < runs; i++)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		YulOpti{}.run(readFileAsString(argv[1]));
		auto endTime = std::chrono::high_resolution_clock::now();
		accumulatedTime += endTime - startTime;
	}
    if (runs)
		std::cout << "Average time: " << 1000.0 * accumulatedTime.count() / static_cast<double>(runs) << " ms." << std::endl;

	return 0;
}
