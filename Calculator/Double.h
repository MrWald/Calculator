//Created by Volodymyr Fomin on 21/02/2018

#ifndef _DOUBLE_H_
#define _DOUBLE_H_

#include "Evaluator.h"

// Class Double, contains plane double and implements methods for evaluating expressions with this type by Evaluator class
class Double
{
public:
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
	static const Double read(const Evaluator<Double>& ev, const int startPos, void (Evaluator<Double>::*move)()const)
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
			//evaluator._ch = evaluator._expression[evaluator._pos];
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

const Double operator/(const Double& d1, const Double& d2)
{
	return operator/=( Double(d1), d2);
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