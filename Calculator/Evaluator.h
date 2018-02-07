//Created by Volodymyr Fomin on 07.02.2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

class Evaluator{
private: 
	static unsigned int _freeId;
	const unsigned int _id;
	int _pos; 
	char _ch;
    string _s;
    bool _rad;

	//Transformation to radians/degrees
	static const double toRadians(const double& deg)
	{
		return (deg*M_PI)/180.f;
	}
	static const double toDegrees(const double& rad)
	{
		return (rad*180.f)/M_PI;
	}

	//Reading next char of expression
	void nextChar()
    {
        _ch = (++_pos < _s.length()) ? _s[_pos] : -1;
    }

	//If passed a character we're currently reading move to the next character of expression and return true, otherwise return false
    const bool eat(const char& charToEat)
    {
        while(_ch == ' ') 
			nextChar();
        if (_ch == charToEat)
        {
            nextChar();
            return true;
        }
        return false;
    }

	const double parseExpression()
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

    const double parseTerm()
    {
        //Calculate operations with greater priority first
        double x(parseFactor());
        //Після чого виконується ділення зчитанної функції або числа на наступне число або функцію, яка теж зчитується і це відбувається доти, доки программа перестане натрапляти після чисел або функцій на відповідні символи множення/ділення
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

    const double parseFactor() 
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
        int startPos = _pos;
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
			if ((_ch >= '0' && _ch <= '9') || _ch == '.' || (_ch == 'P' && _s[_s.find(_ch)+1]=='I') || _ch == 'e') 
			{
                if (_ch == 'P' && _s[_s.find('P')+1]=='I') 
					_s = _s.replace(_s.find('P'), pi.length(), pi+_s.substr(_s.find('I')+1, _s.length()));
                else if (_ch == 'e') 
					_s = _s.replace(_s.find("e"), e.length(), e+_s.substr(_s.find('e')+1, _s.length()));
                _ch = _s[_pos];
                while ((_ch >= '0' && _ch <= '9') || _ch == '.') 
					nextChar();
				//Parsing read number
                x = atof((_s.substr(startPos, _pos-startPos)).c_str());
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
                    string func(_s.substr(startPos, _pos-startPos));
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
			x = pow(x, parseFactor()); //Raising to power
        else if (eat('!')) //Factorial
        {
          if (x<0 || x > static_cast<int>(x) ) 
			   throw invalid_argument("Factorial accepts only positive integer numbers");
           x = factorialNumber(static_cast<unsigned int>(x));
        } 
		//Percents
		else if (eat('%'))
			x = x / 100;
		//Modulo
        else if (eat('&')) 
		{
			if (x > static_cast<int>(x) ) 
			   throw invalid_argument("Modulo accepts only integer numbers");
			x = static_cast<int>(x) % static_cast<int>(parseFactor());
		}
        return x;
    }

    //Factorial
    static const int factorialNumber(const unsigned int& n)
    {
        if(n==0 || n==1) return 1;
        return n * factorialNumber(n-1);
    }
public:
	Evaluator(const string& s="", const bool& rad=true)
		: _id(++_freeId), _pos(-1), _ch(0), _rad(rad), _s(s)
	{
#ifndef NDEBUG
		cout << "Evaluator ID-" << _id << " created" << endl;
#endif
    }

	Evaluator(const Evaluator& ev)
		: _id(++_freeId), _pos(-1), _ch(0), _s(ev._s), _rad(ev._rad)
	{
#ifndef NDEBUG
		cout << "Evaluator ID-" << _id << " copied" << endl;
#endif
	}

	~Evaluator()
	{
#ifndef NDEBUG
		cout << "Evaluator ID-" << _id << " destroyed" << endl;
#endif
	}

	Evaluator& operator=(const Evaluator& ev)
	{
		if(this!=&ev)
		{
			_s = ev._s;
			_rad = ev._rad;
			_pos = -1;
			_ch = 0;
		}
		return *this;
	}
    /*
	Priorities:
		1.Sign
		3.Braces
		3.Numbers
		4.Functions/Operators
		5.Multiplication/Division
		6.Addition/Subtraction
     */

	const string& getExpression() const
	{
		return _s;
	}

	void setExpression(const string& newExpression)
	{
		_s=newExpression;
		_ch=0;
		_pos=-1;
	}

	bool& rad()
	{
		return _rad;
	}

	const bool& rad() const
	{
		return _rad;
	}

    const double parse()
    {
        nextChar();
        //Expression evaluation
        double x = parseExpression();
        if (_pos < _s.length() && _ch!='P' && _ch!='I' && _ch!='e') throw invalid_argument("Unexpected character: " + static_cast<char>(_ch));
		_pos = -1;
		_ch = 0;
        return x;
    }
};

#endif