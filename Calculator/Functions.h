//Created by Volodymyr Fomin on 22/02/2018

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "Double.h"
#include "AComplex.h"

bool Rad = true;

//Transformation to radians/degrees
const double toRadians(const double deg)
{
	return (deg*M_PI)/180.f;
}
const double toDegrees(const double rad)
{
	return (rad*180.f)/M_PI;
}
//Factorial
const unsigned long long int factorial(const unsigned int& n)
{
	if(n==0 || n==1) return 1;
	return n * factorial(n-1);
}

#pragma region Functions and Operators
const Double power(const Double& a, const Double& n)
{
	return Double(pow(a, n));
}
const Double fac(const Double& x)
{
	Double res(x);
	const double eps(1E-9);
	if (x<0)
		throw Evaluator<Double, Double::Function>::EvaluatorException("Factorial accepts only positive integer numbers");
	else if(abs(static_cast<double>(x - static_cast<int>(res)) ) > eps)
	{
		if(abs( static_cast<double>(res - ( static_cast<int>(res)+1 ) )) > eps)
			throw Evaluator<Double, Double::Function>::EvaluatorException("Factorial accepts only positive integer numbers");
		else
			res = Double(static_cast<double>( factorial( static_cast<unsigned int>(res)+1 ) ));
	}
	else
		res = Double(static_cast<double>( factorial(static_cast<unsigned int>(res)) ) );
	return res;
}
const Double modulo(const Double& x, const Double& y)
{
	Double res(x);
	const double eps(1E-9);
	if ( abs(static_cast<double>( x - static_cast<int>(x) )) > eps || abs(static_cast<double>( y - static_cast<int>(y) )) > eps)
	{
		if( abs(static_cast<double>( x - ( static_cast<int>(x)+1 ) )) > eps || abs(static_cast<double>( y - static_cast<int>(y)+1 )) > eps)
			throw Evaluator<Double, Double::Function>::EvaluatorException("Modulo accepts only integer numbers");
		else
			res = Double( ( static_cast<int>(res)+1 ) % static_cast<int>(y) );
	}
	else
		res = Double( static_cast<int>(res) % static_cast<int>(y) );
	return res;
}
const Double percent(const Double& x)
{
	return Double(x/100);
}

const Double sine(const Double& x)
{
	return Double( sin(!Rad?toRadians(x):x) );
}
const Double cosine(const Double& x)
{
	return Double( cos(!Rad?toRadians(x):x) );
}
const Double tg(const Double& x)
{
	return Double( tan(!Rad?toRadians(x):x) );
}
const Double ctg(const Double& x)
{
	return Double( 1./tan(!Rad?toRadians(x):x) );
}
const Double asine(const Double& x)
{
	const double res(asin(x));
	return Double(!Rad?toDegrees(res):res);
}
const Double acosine(const Double& x)
{
	const double res(acos(x));
	return Double(!Rad?toDegrees(res):res);
}
const Double atg(const Double& x)
{
	const double res(atan(x));
	return Double(!Rad?toDegrees(res):res);
}
const Double actg(const Double& x)
{
	const double res(atan(1./x));
	return Double(!Rad?toDegrees(res):res);
}
const Double ln(const Double& x)
{
	return Double(log(x));
}
const Double logTen(const Double& x)
{
	return Double(log10(x));
}
const Double squareRoot(const Double& x)
{
	return Double(sqrt(x));
}
#pragma endregion

#pragma region Creating Standard Functions and Operators
const HashMap<char, Evaluator<Double, Double::Function>::UnaryOperator> getUOps()
{
	const char names[2] = {'!', '%'};
	const Evaluator<Double, Double::Function>::UnaryOperator uOps[2] = {&fac, &percent};
	return HashMap<char, Evaluator<Double, Double::Function>::UnaryOperator>(names, uOps, 2, 10);
}

const HashMap<char, Evaluator<Double, Double::Function>::BinaryOperator> getBOps()
{
	const char names[2] = {'^', '&'};
	const Evaluator<Double, Double::Function>::BinaryOperator bOps[2] = {&power, &modulo};
	return HashMap<char, Evaluator<Double, Double::Function>::BinaryOperator>(names, bOps, 2, 10);
}

const HashMap<string, Double::Function> getFunctions()
{
	const string names[11] = {"sin", "cos", "tg", "ctg", "asin", "acos", "atg", "actg", "ln", "log", "sqrt"};
	const Double::Function funcs[11] = {&sine, &cosine, &tg, &ctg, &asine, &acosine, &atg, &actg, &ln, &logTen, &squareRoot};
	return HashMap<string, Double::Function>(names, funcs, 11, 20);
}

const HashMap<char, Evaluator<AComplex, AComplex::Function>::BinaryOperator> getBOpsComplex()
{
	const char names[1] = {'^'};
	const Evaluator<AComplex, AComplex::Function>::BinaryOperator bOps[1] = {&AComplex::power};
	return HashMap<char, Evaluator<AComplex, AComplex::Function>::BinaryOperator>(names, bOps, 1, 5);
}

const HashMap<string, AComplex::Function> getFunctionsComplex()
{
	const string names[11] = {"conj"};
	const AComplex::Function funcs[1] = {&AComplex::conj};
	return HashMap<string, AComplex::Function>(names, funcs, 1, 5);
}
#pragma endregion

#endif