//Created by Volodymyr Fomin on 21/02/2018

#ifndef _DOUBLE_H_
#define _DOUBLE_H_

#define _USE_MATH_DEFINES
#include <math.h>

#include "Evaluator.h"

// Class Double, contains plane double and implements methods for evaluating expressions with this type by Evaluator class
class Double
{
public:
	// Defining signature of functions we will use in Evaluator for this class
	typedef const Double (*Function)(const Double&);
	class BadDouble
	{
	public:
		BadDouble(const string& message="") : _message(message) {}
	private:
		string _message;
	};

	explicit Double(const double value=0) : _value(value){}

	operator double&() {return _value;}
	operator double() const {return _value;}

//	For parsing with Evaluator
	static const Double read(const Evaluator<Double, Function>& ev, const int startPos, void (Evaluator<Double, Function>::*move)()const)
	{
		Double res;
		if ((ev.getExpression()[startPos] == 'P' && ev.getExpression()[startPos+1] == 'I') 
			|| (ev.getExpression()[startPos] == 'p' && ev.getExpression()[startPos+1] == 'i')) 
		{
			res = Double(M_PI);
			(ev.*move)();
			(ev.*move)();
		}
		else if (ev.getExpression()[startPos] == 'e' || ev.getExpression()[startPos] == 'E') 
		{
			res = Double(M_E);
			(ev.*move)();
		}
		else
		{
			int endPos(startPos);
			while ((ev.getExpression()[endPos] >= '0' && ev.getExpression()[endPos] <= '9') || ev.getExpression()[endPos] == '.') 
			{
				++endPos;
				(ev.*move)();
			}
			//Parsing read number
			res = Double(atof((ev.getExpression().substr(startPos, endPos-startPos)).c_str()));
		}
		return res;
	}

//	Determining whether read by Evaluator expression is of this type
	static bool isElement(const string& expr, const int currentPos)
	{
		const char currentChar(expr[currentPos]);
		return (currentChar >= '0' && currentChar <= '9') || currentChar == '.' 
			|| (currentChar == 'p' && expr[currentPos+1] == 'i') || (currentChar == 'P' && expr[currentPos+1] == 'I') 
			|| currentChar == 'e' || currentChar == 'E'; 
		
	}
//	Applying function of specified signature, because Evaluator doesn't know how many arguments are passed to function of our signature
	static const Double apply(const Evaluator<Double, Function>& ev, const Double (Evaluator<Double, Function>::*parseExpression)()const, const Double& current, const Function& currentF)
	{
		return currentF(current);
	}
private:
	double _value;
};

//Overloading operators of division for throwing exceptions when dividing by zero and unary +, - for correct casting
Double& operator/=(Double& d1, const Double& d2)
{
	if(!d2)
		throw Double::BadDouble("Division by zero");
	d1.operator double&()/=d2;
	return d1;
}

const Double operator/(Double d1, const Double& d2)
{
	return operator/=( d1, d2);
}

const Double operator+(const Double& d)
{
	return d;
}

const Double operator-(const Double& d)
{
	return Double(-d.operator double());
}

//Operator for accurate console output
ostream& operator<<(ostream& out, const Double& d)
{
	return out << (abs(d.operator double())<1E-9?0:d.operator double());
}

#endif