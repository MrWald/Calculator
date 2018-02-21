//Created by Volodymyr Fomin on 07/02/2018
#include <iostream>
#include <sstream>
#include "Evaluator.h"

using namespace std;

void listCommands(const string commands[], const unsigned int& size)
{
	for(unsigned int i(0);i<size;i++)
		cout << commands[i] << endl;
	return;
}

#pragma region Declarations

const double sine(const double, const bool);
const double cosine(const double, const bool);
const double tg(const double, const bool);
const double ctg(const double, const bool);
const double asine(const double, const bool);
const double acosine(const double, const bool);
const double atg(const double, const bool);
const double actg(const double, const bool);
const double ln(const double, const bool);
const double logTen(const double, const bool);
const double squareRoot(const double, const bool);

#pragma endregion

const HashMap<string, Evaluator::Function> getMap()
{
	const string names[11] = {"sin", "cos", "tg", "ctg", "asin", "acos", "atg", "actg", "ln", "log", "sqrt"};
	const Evaluator::Function funcs[11] = {&sine, &cosine, &tg, &ctg, &asine, &acosine, &atg, &actg, &ln, &logTen, &squareRoot};
	return HashMap<string, Evaluator::Function>(names, funcs, 11);
}

int main()
{
	const unsigned int commandsSize(24);
	string commands[commandsSize]=
	{
		"help - list all available commands, functions and operators", "rad - read angle in radians or degrees", "exit - exit from the programm", 
		"( - opening brace", ") - closing brace, optional", "+ - addition", "- - subtraction", "* - multiplication", "/ - division", 
		"! - factorial, put in the end of expression", "^ - raise to power", "% - turn to percents", "& - modulo", 
		"sin - sine", "cos - cosine", "tg - tangent", "ctg - cotangent", "asin - arcsine", "acos - arccosine", "atg - arctangent", "actg - arccotangent", 
		"ln - natural logarithm", "log - logarithm to base 10", "sqrt - square root"
	};
	string expression;
	Evaluator evaluator(getMap());
	while(true)
	{
		cout << "\nEnter command or expression" << endl;
		getline(cin, expression);
		if(!expression.compare("exit"))
			break;
		else if(!expression.compare("help"))
		{
			listCommands(commands, commandsSize);
		}
		else if(!expression.compare("rad"))
		{
			evaluator.rad() = !evaluator.rad();
			cout << "Radians turned " << (evaluator.rad()?"on":"off") << endl;
		}
		else
		{
			try
			{
				evaluator.setExpression(expression);
				cout << evaluator << endl;
			}
			catch(const Evaluator::EvaluatorException& e)
			{
				cerr << e.error() << endl;
				evaluator.setExpression("");
				cout << "Enter 'help' if you forgot appropriate expressions" << endl;
			}
		}
		expression="";
	}
	return 0;
}

#pragma region Functions
	static const double sine(const double x, const bool rad)
	{
		return sin(!rad?Evaluator::toRadians(x):x);
	}
	static const double cosine(const double x, const bool rad)
	{
		return cos(!rad?Evaluator::toRadians(x):x);
	}
	static const double tg(const double x, const bool rad)
	{
		return tan(!rad?Evaluator::toRadians(x):x);
	}
	static const double ctg(const double x, const bool rad)
	{
		return 1./tan(!rad?Evaluator::toRadians(x):x);
	}
	static const double asine(const double x, const bool rad)
	{
		const double res(asin(x));
		return (!rad?Evaluator::toDegrees(res):res);
	}
	static const double acosine(const double x, const bool rad)
	{
		const double res(acos(x));
		return (!rad?Evaluator::toDegrees(res):res);
	}
	static const double atg(const double x, const bool rad)
	{
		const double res(atan(x));
		return (!rad?Evaluator::toDegrees(res):res);
	}
	static const double actg(const double x, const bool rad)
	{
		const double res(atan(1./x));
		return (!rad?Evaluator::toDegrees(res):res);
	}
	static const double ln(const double x, const bool)
	{
		return log(x);
	}
	static const double logTen(const double x, const bool)
	{
		return log10(x);
	}
	static const double squareRoot(const double x, const bool)
	{
		return sqrt(x);
	}
#pragma endregion