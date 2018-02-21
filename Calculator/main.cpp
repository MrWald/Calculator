//Created by Volodymyr Fomin on 07/02/2018
#include <iostream>
#include <sstream>
#include "Evaluator.h"
#include "Double.h"

using namespace std;

#pragma region Declarations

const Double power(const Double&, const Double&);
const Double fac(const Double&);
const Double modulo(const Double&, const Double&);
const Double percent(const Double&);

const Double sine(const Double&, const bool);
const Double cosine(const Double&, const bool);
const Double tg(const Double&, const bool);
const Double ctg(const Double&, const bool);
const Double asine(const Double&, const bool);
const Double acosine(const Double&, const bool);
const Double atg(const Double&, const bool);
const Double actg(const Double&, const bool);
const Double ln(const Double&, const bool);
const Double logTen(const Double&, const bool);
const Double squareRoot(const Double&, const bool);

#pragma endregion

#pragma region Functions and Operators Hash Maps

const HashMap<char, Evaluator<Double>::UnaryOperator> getUOps()
{
	const char names[2] = {'!', '%'};
	const Evaluator<Double>::UnaryOperator uOps[2] = {&fac, &percent};
	return HashMap<char, Evaluator<Double>::UnaryOperator>(names, uOps, 2, 10);
}

const HashMap<char, Evaluator<Double>::BinaryOperator> getBOps()
{
	const char names[2] = {'^', '&'};
	const Evaluator<Double>::BinaryOperator bOps[2] = {&power, &modulo};
	return HashMap<char, Evaluator<Double>::BinaryOperator>(names, bOps, 2, 10);
}

const HashMap<string, Evaluator<Double>::Function> getFunctions()
{
	const string names[11] = {"sin", "cos", "tg", "ctg", "asin", "acos", "atg", "actg", "ln", "log", "sqrt"};
	const Evaluator<Double>::Function funcs[11] = {&sine, &cosine, &tg, &ctg, &asine, &acosine, &atg, &actg, &ln, &logTen, &squareRoot};
	return HashMap<string, Evaluator<Double>::Function>(names, funcs, 11, 20);
}
#pragma endregion

void listCommands(const string commands[], const unsigned int& size)
{
	for(unsigned int i(0);i<size;i++)
		cout << commands[i] << endl;
	return;
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
	Evaluator<Double> evaluator(getUOps(), getBOps(), getFunctions());
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
			catch(const Evaluator<Double>::EvaluatorException& e)
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

//Factorial
const unsigned long long int factorial(const unsigned int& n)
{
	if(n==0 || n==1) return 1;
	return n * factorial(n-1);
}

#pragma region Operators
	const Double power(const Double& a, const Double& n)
	{
		return Double(pow(a, n));
	}
	const Double fac(const Double& x)
	{
		Double res(x);
		const double eps(1E-9);
		if (x<0)
			throw Evaluator<Double>::EvaluatorException("Factorial accepts only positive integer numbers");
		else if(abs( x - static_cast<int>(res) ) > eps)
		{
			if(abs( res - ( static_cast<int>(res)+1 ) ) > eps)
				throw Evaluator<Double>::EvaluatorException("Factorial accepts only positive integer numbers");
			else 
				res = Double(static_cast<double>( factorial( static_cast<unsigned int>(res)+1 ) ));
		}
        else 
			res = Double(static_cast<double>( factorial(static_cast<unsigned int>(res)) ) );
		return res;
	}
	const Double percent(const Double& x)
	{
		return Double(x/100);
	}
	const Double modulo(const Double& x, const Double& y)
	{
		Double res(x);
		const double eps(1E-9);
		if ( abs( x - static_cast<int>(x) ) > eps || abs( y - static_cast<int>(y) ) > eps) 
		{
			if( abs( x - ( static_cast<int>(x)+1 ) ) > eps || abs( y - static_cast<int>(y)+1 ) > eps)
				throw Evaluator<Double>::EvaluatorException("Modulo accepts only integer numbers");
			else 
				res = Double( ( static_cast<int>(res)+1 ) % static_cast<int>(y) );
		}
		else
			res = Double( static_cast<int>(res) % static_cast<int>(y) );
		return res;
	}
#pragma endregion

#pragma region Functions
	const Double sine(const Double& x, const bool rad)
	{
		return Double( sin(!rad?Evaluator<Double>::toRadians(x):x) );
	}
	const Double cosine(const Double& x, const bool rad)
	{
		return Double( cos(!rad?Evaluator<Double>::toRadians(x):x) );
	}
	const Double tg(const Double& x, const bool rad)
	{
		return Double( tan(!rad?Evaluator<Double>::toRadians(x):x) );
	}
	const Double ctg(const Double& x, const bool rad)
	{
		return Double( 1./tan(!rad?Evaluator<Double>::toRadians(x):x) );
	}
	const Double asine(const Double& x, const bool rad)
	{
		const double res(asin(x));
		return Double(!rad?Evaluator<Double>::toDegrees(res):res);
	}
	const Double acosine(const Double& x, const bool rad)
	{
		const double res(acos(x));
		return Double(!rad?Evaluator<Double>::toDegrees(res):res);
	}
	const Double atg(const Double& x, const bool rad)
	{
		const double res(atan(x));
		return Double(!rad?Evaluator<Double>::toDegrees(res):res);
	}
	const Double actg(const Double& x, const bool rad)
	{
		const double res(atan(1./x));
		return Double(!rad?Evaluator<Double>::toDegrees(res):res);
	}
	const Double ln(const Double& x, const bool)
	{
		return Double(log(x));
	}
	const Double logTen(const Double& x, const bool)
	{
		return Double(log10(x));
	}
	const Double squareRoot(const Double& x, const bool)
	{
		return Double(sqrt(x));
	}
#pragma endregion