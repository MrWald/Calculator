//Created by Volodymyr Fomin on 07/02/2018

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include "Evaluator.h"

using namespace std;

unsigned int Evaluator::_freeId(0);
const double Evaluator::_eps(1E-9);

Evaluator::Evaluator(const string& exp, const bool& rad)
	: _id(++_freeId), _pos(-1), _ch(0), _expression(exp), _rad(rad)
{
#ifndef NDEBUG
	cout << "Evaluator ID-" << _id << " created" << endl;
#endif
}
Evaluator::Evaluator(const Evaluator& ev)
	: _id(++_freeId), _pos(-1), _ch(0), _expression(ev._expression), _rad(ev._rad)
{
#ifndef NDEBUG
	cout << "Evaluator ID-" << _id << " copied" << endl;
#endif
}
Evaluator::~Evaluator()
{
#ifndef NDEBUG
	cout << "Evaluator ID-" << _id << " destroyed" << endl;
#endif
}

Evaluator& Evaluator::operator=(const Evaluator& ev)
{
	if(this!=&ev)
	{
		_expression = ev._expression;
		_rad = ev._rad;
		_pos = -1;
		_ch = 0;
	}
	return *this;
}

const double Evaluator::toRadians(const double deg)
{
	return (deg*M_PI)/180.f;
}
const double Evaluator::toDegrees(const double rad)
{
	return (rad*180.f)/M_PI;
}

const unsigned long long int Evaluator::factorial(const unsigned int n)
{
	if(n==0 || n==1) return 1;
    return n * factorial(n-1);
}

void Evaluator::nextChar() const
{
	_ch = (++_pos < static_cast<int>(_expression.length())) ? _expression[_pos] : -1;
}
const bool Evaluator::eat(const char& c) const
{
	while(_ch == ' ') 
		nextChar();
	if (_ch == c)
	{
		nextChar();
		return true;
	}
	return false;
}

const double Evaluator::parse() const
{
	nextChar();
	//Expression evaluation
	double x(parseExpression());
	if (_pos < static_cast<int>(_expression.length()) && _ch!='P' && _ch!='I' && _ch!='e') 
		throw invalid_argument(string("Unexpected character: ")+=_ch);
	_pos = -1;
	_ch = 0;
	if(abs(x)<_eps)
		x=0;
	return x;
}
const double Evaluator::parseExpression() const
{
    //Calculating operations with greater priority first
    double x(parseTerm());
    //Afterwards move to addition/subtraction
    while(true)
    {
        if (eat('+')) 
			x += parseTerm(); //Addition
        else {
            if (eat('-')) 
				x -= parseTerm(); //Subtraction
			//If there are no operators of addition/subtraction -> return the result
            else 
				return x;
        }
    }
}
const double Evaluator::parseTerm() const
{
    //Calculate operations with greater priority first
    double x(parseFactor());
    //Afterwards move to multiplication/division
    while(true)
    {
		if(eat('*')) 
			x *= parseFactor(); //Multiplication
        else 
		{
            if(eat('/')) 
				x /= parseFactor(); //Division
			//If there are no operators of multiplication/division -> return the result
            else 
				return x;
        }
    }
}
const double Evaluator::parseFactor() const
{
    //Firstly process sign
	//Return later evaluated result with appropriate sign
    if (eat('+')) 
		return parseFactor();
    if (eat('-')) 
		return -parseFactor();
	//The result
    double x(0);
	//Remember position from which processing started to get a substring from the whole expression
    int startPos(_pos);
    if (eat('('))
    {
        //Read braces
        //Start evaluating sub expression as a new expression
        x = parseExpression();
        eat(')');
    }
    else 
	{
        //If currently read char is a number move through expression until come across a non-digit, not dot or not letters responsible for constants
        //If constant is read -> replace with number and process it again
		if ((_ch >= '0' && _ch <= '9') || _ch == '.' || (_ch == 'P' && _expression[_expression.find(_ch)+1]=='I') || _ch == 'e') 
		{
            x = parseNumber(startPos);
        } 
		else 
		{
            //If read letter not responsible for constant -> process as a function
            //Move through expression until come across a non-letter character. Then store read substring and find appropriate function for it
            //If appropriate function hadn't been found -> throw an exception
            if (_ch >= 'a' && _ch <= 'z') 
			{
                x = parseFunction(startPos);
			}
			else 
				throw invalid_argument(string("Unexpected character: ")+=_ch);
		} 
	}
    //After processing numbers/functions process operators
    x = parseOperator(x);
    return x;
}
const double Evaluator::parseNumber(const int startPos) const
{
	const string pi("3.14159265358979323846"), e("2.71828182845904523536");
	if (_ch == 'P' && _expression[_expression.find('P')+1]=='I') 
		_expression = _expression.replace(
			_expression.find('P'), pi.length(), 
			pi+_expression.substr(_expression.find('I')+1, _expression.length()));
    else if (_ch == 'e') 
		_expression = _expression.replace(
			_expression.find("e"), e.length(), 
			e+_expression.substr(_expression.find('e')+1, _expression.length()));
    _ch = _expression[_pos];
    while ((_ch >= '0' && _ch <= '9') || _ch == '.') 
		nextChar();
	//Parsing read number
	return atof((_expression.substr(startPos, _pos-startPos)).c_str());
}
const double Evaluator::parseFunction(const int startPos) const
{
	double res(0);
	while (_ch >= 'a' && _ch <= 'z') 
		nextChar();
    string func(_expression.substr(startPos, _pos-startPos));
    res = parseFactor();
	if(!func.compare("sin"))
	{
		if (!_rad) 
			res = toRadians(res);
		res = sin(res);
	}
	else if(!func.compare("cos"))
	{
		if (!_rad)
			res = toRadians(res);
		res = cos(res);
	}
	else if(!func.compare("tg"))
	{
		if (!_rad)
			res = toRadians(res);
		res = tan(res);
	}
	else if(!func.compare("ctg"))
	{
		if (!_rad) 
			res=toRadians(res);
		res = 1 / tan(res);
	}
	else if(!func.compare("asin"))
	{
		res = asin(res);
		if(!_rad)
			res = toDegrees(res);
	}
	else if(!func.compare("acos"))
	{
		res = acos(res);
		if(!_rad) 
			res = toDegrees(res);
	}
	else if(!func.compare("atg"))
	{
		res = atan(res);
		if(!_rad) 
			res = toDegrees(res);
	}
	else if(!func.compare("actg"))
	{
		res = atan(1 / res);
		if(!_rad) 
			res = toDegrees(res);
	}
	else if(!func.compare("ln"))
		res = log(res);
	else if(!func.compare("log"))
		res = log10(res);
	else if(!func.compare("sqrt"))
		res = sqrt(res);
	else
		throw invalid_argument("Unexpected function: " + func);
	return res;
}
const double Evaluator::parseOperator(const double calculated) const
{
	double res(calculated);
	if (eat('^')) 
		res = pow(res, parseExpression()); //Raising to power
    else if (eat('!')) //Factorial
    {
        if (calculated<0) 
			throw invalid_argument("Factorial accepts only positive integer numbers");
		else if(abs( calculated - static_cast<int>(res) ) > _eps)
		{
			if(abs( res - ( static_cast<int>(res)+1 ) ) > _eps)
				throw invalid_argument("Factorial accepts only positive integer numbers");
			else 
				res = static_cast<double>(factorial( static_cast<unsigned int>(res)+1 ));
		}
        else 
			res = static_cast<double>(factorial(static_cast<unsigned int>(res)));
    } 
	//Percents
	else if (eat('%'))
		res /= 100;
	//Modulo
    else if (eat('&')) 
	{
		if ( abs( calculated - static_cast<int>(res) ) > _eps ) 
		{
			if( abs( calculated - ( static_cast<int>(res)+1 ) ) > _eps )
				throw invalid_argument("Modulo accepts only integer numbers");
			else 
				res = ( static_cast<int>(res)+1 ) % static_cast<int>(parseExpression());
		}
		else
			res = static_cast<int>(res) % static_cast<int>(parseExpression());
	}
	return res;
}

const string& Evaluator::getExpression() const
{
	return _expression;
}
void Evaluator::setExpression(const string& exp)
{
	_expression=exp;
	_ch=0;
	_pos=-1;
}

bool& Evaluator::rad() const
{
	return _rad;
}