#ifndef _DOUBLE_H_
#define _DOUBLE_H_

#define _USE_MATH_DEFINES
#include <math.h>

#include "Evaluator.h"

class Double
{
public:
	Double(const double value) : _value(value){}

	operator double&() {return _value;}
	operator double() const {return _value;}

//	For parsing with Evaluator
	static const Double read(const Evaluator<Double>& evaluator, const int startPos)
	{
		Double res(0);
		if ((evaluator._ch == 'P' && evaluator._expression[evaluator._pos+1] == 'I') || (evaluator._ch == 'p' && evaluator._expression[evaluator._pos+1] == 'i')) 
		{
			res = M_PI;
			evaluator.nextChar();
			evaluator.nextChar();
		}
		else if (evaluator._ch == 'e' || evaluator._ch == 'E') 
		{
			res = M_E;
			evaluator.nextChar();
		}
		else
		{
			evaluator._ch = evaluator._expression[evaluator._pos];
			while ((evaluator._ch >= '0' && evaluator._ch <= '9') || evaluator._ch == '.') 
				evaluator.nextChar();
			//Parsing read number
			res = atof((evaluator._expression.substr(startPos, evaluator._pos-startPos)).c_str());
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

#endif