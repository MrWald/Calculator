//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_
#include "HashMap.h"
using namespace std;

class Evaluator
{
	typedef const double (*Function)(const double, const bool);
	typedef const double (*UnaryOperator)(const double);
	typedef const double (*BinaryOperator)(const double, const double);
public:
	class EvaluatorException
	{
	public:
		explicit EvaluatorException(const string& error="");
		EvaluatorException(const EvaluatorException&);
		~EvaluatorException();
		EvaluatorException& operator=(const EvaluatorException& exception);
		const string& error() const;
		string& error();
		const unsigned int& getId() const;
		static const unsigned int& amount();
	private:
		static unsigned int _freeId;

		const unsigned int _id;
		string _message;
	};

	explicit Evaluator(const string& exp="", const bool& rad=true);
	Evaluator(const Evaluator&);
	~Evaluator();

	Evaluator& operator=(const Evaluator&);

	operator double();
    
	const string& getExpression() const;
	void setExpression(const string&);

	bool& rad();

    const double parse() const;
private: 
	static EvaluatorException _error;
	static unsigned int _freeId;

	static const HashMap<string, Function> _functions;
	static const HashMap<string, Function> getMap();
	static const UnaryOperator _unaryOperators[];
	static const BinaryOperator _binaryOperators[];

#pragma region Functions
	static const double sine(const double x, const bool rad)
	{
		return sin(!rad?toRadians(x):x);
	}
	static const double cosine(const double x, const bool rad)
	{
		return cos(!rad?toRadians(x):x);
	}
	static const double tg(const double x, const bool rad)
	{
		return tan(!rad?toRadians(x):x);
	}
	static const double ctg(const double x, const bool rad)
	{
		return 1./tan(!rad?toRadians(x):x);
	}
	static const double asine(const double x, const bool rad)
	{
		const double res(asin(x));
		return (!rad?toDegrees(res):res);
	}
	static const double acosine(const double x, const bool rad)
	{
		const double res(acos(x));
		return (!rad?toDegrees(res):res);
	}
	static const double atg(const double x, const bool rad)
	{
		const double res(atan(x));
		return (!rad?toDegrees(res):res);
	}
	static const double actg(const double x, const bool rad)
	{
		const double res(atan(1./x));
		return (!rad?toDegrees(res):res);
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

#pragma region Operators
	static const double power(const double a, const double n)
	{
		return pow(a, n);
	}
	static const double fac(const double x)
	{
		double res(x);
		if (x<0)
		{
			_error.error() = "Factorial accepts only positive integer numbers";
			throw &_error;
		}
		else if(abs( x - static_cast<int>(res) ) > _eps)
		{
			if(abs( res - ( static_cast<int>(res)+1 ) ) > _eps)
			{
				_error.error() = "Factorial accepts only positive integer numbers";
				throw &_error;
			}
			else 
				res = static_cast<double>(factorial( static_cast<unsigned int>(res)+1 ));
		}
        else 
			res = static_cast<double>(factorial(static_cast<unsigned int>(res)));
		return res;
	}
	static const double percent(const double x)
	{
		return x/100;
	}
	static const double modulo(const double x, const double y)
	{
		double res(x);
		if ( abs( x - static_cast<int>(x) ) > _eps || abs( y - static_cast<int>(y) ) > _eps) 
		{
			if( abs( x - ( static_cast<int>(x)+1 ) ) > _eps || abs( y - static_cast<int>(y)+1 ) > _eps)
			{
				_error.error() = "Modulo accepts only integer numbers";
				throw &_error;
			}
			else 
				res = ( static_cast<int>(res)+1 ) % static_cast<int>(y);
		}
		else
			res = static_cast<int>(res) % static_cast<int>(y);
		return res;
	}
#pragma endregion

	const unsigned int _id;
	static const double _eps;
	mutable int _pos; 
	mutable char _ch;
    string _expression;
    mutable bool _rad;

	//Transformation to radians/degrees
	static const double toRadians(const double);
	static const double toDegrees(const double);
	//Factorial
    static const unsigned long long int factorial(const unsigned int);

	//Reading next char of expression
	void nextChar() const;
	//If passed a character we're currently reading move to the next character of expression and return true, otherwise return false
    const bool eat(const char&) const;

	/*
	Priorities:
		1.Sign
		3.Braces
		3.Numbers
		4.Functions/Operators
		5.Multiplication/Division
		6.Addition/Subtraction
     */
	const double parseExpression() const;
    const double parseTerm() const;
    const double parseFactor() const;
	const double parseNumber(const int) const;
	const double parseFunction(const int) const;
	const double parseOperator(const double) const;
};

#endif