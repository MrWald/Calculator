//Created by Volodymyr Fomin on 07/02/2018

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include "Evaluator.h"

using namespace std;

unsigned int Evaluator::_freeId(0);

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

const double Evaluator::toRadians(const double& deg)
{
	return (deg*M_PI)/180.f;
}
const double Evaluator::toDegrees(const double& rad)
{
	return (rad*180.f)/M_PI;
}

const unsigned long long Evaluator::factorial(const unsigned int& n)
{
	if(n==0 || n==1) return 1;
    return n * factorial(n-1);
}

void Evaluator::nextChar()
{
	_ch = (++_pos < static_cast<int>(_expression.length())) ? _expression[_pos] : -1;
}
const bool Evaluator::eat(const char& c)
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

const double Evaluator::parse()
{
	nextChar();
	//Expression evaluation
	double x(parseExpression());
	if (_pos < static_cast<int>(_expression.length()) && _ch!='P' && _ch!='I' && _ch!='e') 
		throw invalid_argument(string("Unexpected character: ")+=_ch);
	_pos = -1;
	_ch = 0;
	if(abs(x)<1E-9)
		x=0;
	return x;
}
const double Evaluator::parseExpression()
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
const double Evaluator::parseTerm()
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
const double Evaluator::parseFactor()
	{
        //Firstly process sign
		//Return later evaluated result with appropriate sign
        if (eat('+')) 
			return parseFactor();
        if (eat('-')) 
			return -parseFactor();
		const string pi("3.14159265358979323846"), e("2.71828182845904523536");
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
                x = atof((_expression.substr(startPos, _pos-startPos)).c_str());
            } 
			else 
			{
                //If read letter not responsible for constant -> process as a function
                //Move through expression until come across a non-letter character. Then store read substring and find appropriate function for it
                //If appropriate function hadn't been found -> throw an exception
                if (_ch >= 'a' && _ch <= 'z') 
				{
                    while (_ch >= 'a' && _ch <= 'z') 
						nextChar();
                    string func(_expression.substr(startPos, _pos-startPos));
                    x = parseFactor();
					if(!func.compare("sin"))
					{
						if (_rad) 
							x = sin(x);
						else 
							x = sin(toRadians(x));
					}
					else if(!func.compare("cos"))
					{
						if (_rad) 
							x = cos(x);
						else
							x = cos(toRadians(x));
					}
					else if(!func.compare("tg"))
					{
						if (_rad) 
							x = tan(x);
						else 
							x = tan(toRadians(x));
					}
					else if(!func.compare("ctg"))
					{
						if (_rad) 
							x = 1 / tan(x);
						else 
							x = 1 / tan(toRadians(x));
					}
					else if(!func.compare("asin"))
					{
						if (_rad) 
							x = asin(x);
						else 
							x = toDegrees(asin(x));
					}
					else if(!func.compare("acos"))
					{
						if (_rad) 
							x = acos(x);
						else 
							x = toDegrees(acos(x));
					}
					else if(!func.compare("atg"))
					{
						if (_rad)
							x = atan(x);
						else 
							x = toDegrees(atan(x));
					}
					else if(!func.compare("actg"))
					{
						if (_rad) 
							x = atan(1 / x);
						else 
							x = toDegrees(atan(1 / x));
					}
					else if(!func.compare("ln"))
					{
						x = log(x);
					}
					else if(!func.compare("log"))
					{
						x = log10(x);
					}
					else if(!func.compare("sqrt"))
					{
						x = sqrt(x);
					}
					else
						throw invalid_argument("Unexpected function: "+func);
				}
				else 
					throw invalid_argument(string("Unexpected character: ")+=_ch);
			} 
		}
        //After processing numbers/functions process operators
        if (eat('^')) 
			x = pow(x, parseExpression()); //Raising to power
        else if (eat('!')) //Factorial
        {
          if (x<0) 
			   throw invalid_argument("Factorial accepts only positive integer numbers");
		  else if(abs( x - static_cast<int>(x) ) > 1E-9)
		  {
			  if(abs( x - ( static_cast<int>(x)+1 ) ) > 1E-9)
				  throw invalid_argument("Factorial accepts only positive integer numbers");
			  else 
				  x = static_cast<double>(factorial( static_cast<unsigned int>(x)+1 ));
		  }
          else 
			  x = static_cast<double>(factorial(static_cast<unsigned int>(x)));
        } 
		//Percents
		else if (eat('%'))
			x = x / 100;
		//Modulo
        else if (eat('&')) 
		{
			if ( abs( x - static_cast<int>(x) ) >= 1E-8 ) 
			{
				if( abs( x - ( static_cast<int>(x)+1 ) ) > 1E-9 )
					throw invalid_argument("Modulo accepts only integer numbers");
				else 
					x = ( static_cast<int>(x)+1 ) % static_cast<int>(parseExpression());
			}
			else
				x = static_cast<int>(x) % static_cast<int>(parseExpression());
		}
        return x;
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

bool& Evaluator::rad()
{
	return _rad;
}
const bool& Evaluator::rad() const
{
	return _rad;
}