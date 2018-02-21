#ifndef _DOUBLE_H_
#define _DOUBLE_H_

#define _USE_MATH_DEFINES
#include <math.h>

#include "Evaluator.h"

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

	Double(const Double& d) : _value(d._value) {}

	operator double&() {return _value;}
	operator double() const {return _value;}

//	For parsing with Evaluator
	static const Double read(const Evaluator<Double>& evaluator, const int startPos)
	{
		Double res;
		if ((evaluator._ch == 'P' && evaluator._expression[evaluator._pos+1] == 'I') || (evaluator._ch == 'p' && evaluator._expression[evaluator._pos+1] == 'i')) 
		{
			res = Double(M_PI);
			evaluator.nextChar();
			evaluator.nextChar();
		}
		else if (evaluator._ch == 'e' || evaluator._ch == 'E') 
		{
			res = Double(M_E);
			evaluator.nextChar();
		}
		else
		{
			evaluator._ch = evaluator._expression[evaluator._pos];
			while ((evaluator._ch >= '0' && evaluator._ch <= '9') || evaluator._ch == '.') 
				evaluator.nextChar();
			//Parsing read number
			res = Double(atof((evaluator._expression.substr(startPos, evaluator._pos-startPos)).c_str()));
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

ostream& operator<<(ostream& out, const Double& d)
{
	return out << (abs(d.operator double())<1E-9?0:d.operator double());
}

#endif