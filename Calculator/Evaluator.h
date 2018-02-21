//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_
#include "HashMap.h"
using namespace std;

class Evaluator
{
	typedef double Element;
	typedef const double (*UnaryOperator)(const double);
	typedef const double (*BinaryOperator)(const double, const double);
public:
	typedef const Element (*Function)(const Element, const bool);
	//Transformation to radians/degrees
	static const double toRadians(const double);
	static const double toDegrees(const double);

	class EvaluatorException
	{
	public:
		explicit EvaluatorException(const string& message="") : _message(message){}
		EvaluatorException(const EvaluatorException& exception) : _message(exception._message){}
		~EvaluatorException(){}
		const string& error() const 
		{
			return _message;
		}
	private:
		EvaluatorException& operator=(const EvaluatorException&);
		string _message;
	};

	explicit Evaluator(const HashMap<string, Function>& functions, const string& exp="", const bool& rad=true);
	Evaluator(const Evaluator&);
	~Evaluator();

	Evaluator& operator=(const Evaluator&);

	operator double();
    
	const string& getExpression() const;
	void setExpression(const string&);

	bool& rad();

    const double parse() const;
private: 
	static unsigned int _freeId;
	static const UnaryOperator _unaryOperators[];
	static const BinaryOperator _binaryOperators[];

#pragma region Operators
	static const double power(const double a, const double n)
	{
		return pow(a, n);
	}
	static const double fac(const double x)
	{
		double res(x);
		if (x<0)
			throw EvaluatorException("Factorial accepts only positive integer numbers");
		else if(abs( x - static_cast<int>(res) ) > _eps)
		{
			if(abs( res - ( static_cast<int>(res)+1 ) ) > _eps)
				throw EvaluatorException("Factorial accepts only positive integer numbers");
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
				throw EvaluatorException("Modulo accepts only integer numbers");
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
	HashMap<string, Function> _functions;
    mutable bool _rad;

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
	const Element parseExpression() const;
    const Element parseTerm() const;
    const Element parseFactor() const;
	const Element parseNumber(const int) const;
	const Element parseFunction(const int) const;
	const Element parseOperator(const double) const;
};

#endif