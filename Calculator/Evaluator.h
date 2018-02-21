//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include "HashMap.h"
#ifndef NDEBUG
#include <iostream>
#endif
using namespace std;
template<class Element>
class Evaluator
{
	friend const Element Element::read(const Evaluator& e, const int st);
public:
	typedef const Element (*Function)(const Element, const bool);
	typedef const Element (*UnaryOperator)(const Element);
	typedef const Element (*BinaryOperator)(const Element, const Element);
	//Transformation to radians/degrees
	static const double toRadians(const double deg)
	{
		return (deg*M_PI)/180.f;
	}
	static const double toDegrees(const double rad)
	{
		return (rad*180.f)/M_PI;
	}

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

	explicit Evaluator(const HashMap<char, UnaryOperator>& unaryOperators, const HashMap<char, BinaryOperator>& binaryOperators,
		const HashMap<string, Function>& functions, const string& exp="", const bool& rad=true)
		: _id(++_freeId), _pos(-1), _ch(0), _expression(exp), _unaryOperators(unaryOperators), _binaryOperators(binaryOperators), _functions(functions), _rad(rad)
	{
	#ifndef NDEBUG
		cout << "Evaluator ID-" << _id << " created" << endl;
	#endif
	}
	Evaluator(const Evaluator<Element>& ev)
		: _id(++_freeId), _pos(-1), _ch(0), _expression(ev._expression), _unaryOperators(ev._unaryOperators), _binaryOperators(ev._binaryOperators), _functions(ev._functions), _rad(ev._rad)
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

	Evaluator& operator=(const Evaluator<Element>& ev)
	{
		if(this!=&ev)
		{
			_expression = ev._expression;
			_rad = ev._rad;
			_functions = ev._functions;
			_unaryOperators = ev._unaryOperators;
			_binaryOperators = ev._binaryOperators;
			_pos = -1;
			_ch = 0;
		}
		return *this;
	}

	operator Element()
	{

		return parse();
	}
    
	const string& getExpression() const
	{
		return _expression;
	}
	void setExpression(const string& exp)
	{
		_expression=exp;
		_ch=0;
		_pos=-1;
	}

	const char& currentChar() const
	{
		return _ch;
	}

	const int& currentPos() const
	{
		return _pos;
	}

	bool& rad()
	{
		return _rad;
	}

    const Element parse() const
	{
		if(!_expression.compare(""))
			throw EvaluatorException("Expression undefined");
		nextChar();
		//Expression evaluation
		Element x(parseExpression());
		if (_pos < static_cast<int>(_expression.length()) && !Element::isElement(_expression, _pos)) 
			throw EvaluatorException(string("Unexpected character: ")+=_ch);
		_pos = -1;
		_ch = 0;
		return x;
	}
private: 
	static unsigned int _freeId;

	const unsigned int _id;
	mutable int _pos; 
	mutable char _ch;
    string _expression;
	HashMap<string, Function> _functions;
	HashMap<char, UnaryOperator> _unaryOperators;
	HashMap<char, BinaryOperator> _binaryOperators;
    mutable bool _rad;

	//Reading next char of expression
	void nextChar() const
	{
		_ch = (++_pos < static_cast<int>(_expression.length())) ? _expression[_pos] : -1;
	}
	//If passed a character we're currently reading move to the next character of expression and return true, otherwise return false
	void eatWhiteSpaces() const
	{
		while(_ch == ' ') 
			nextChar();
	}
    const bool eat(const char& c) const
	{
		eatWhiteSpaces();
		if (_ch == c)
		{
			nextChar();
			return true;
		}
		return false;
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
	const Element parseExpression() const
	{
		//Calculating operations with greater priority first
		Element x(parseTerm());
		//Afterwards move to addition/subtraction
		while(true)
		{
			if (eat('+')) 
				x += parseTerm(); //Addition
			else if (eat('-')) 
				x -= parseTerm(); //Subtraction
				//If there are no operators of addition/subtraction -> return the result
			else 
				return x;
		}
	}
    const Element parseTerm() const
	{
		//Calculate operations with greater priority first
		Element x(parseFactor());
		//Afterwards move to multiplication/division
		while(true)
		{
			if(eat('*')) 
				x *= parseFactor(); //Multiplication
			else if(eat('/')) 
				x /= parseFactor(); //Division
				//If there are no operators of multiplication/division -> return the result
			else 
				return x;
		}
	}
    const Element parseFactor() const
	{
		//Firstly process sign
		//Return later evaluated result with appropriate sign
		if (eat('+')) 
			return parseFactor();
		if (eat('-')) 
			return -parseFactor();
		//The result
		Element x(0);
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
			if(Element::isElement(_expression, _pos))
				x = Element::read(*this, startPos);
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
					throw EvaluatorException(string("Unexpected character: ")+=_ch);
			} 
		}
		//After processing numbers/functions process operators
		x = parseOperator(x);
		return x;
	}
	const Element parseFunction(const int startPos) const
	{
		Element res(0);
		while (_ch >= 'a' && _ch <= 'z') 
			nextChar();
		string func(_expression.substr(startPos, _pos-startPos));
		res = parseFactor();
		try
		{
			res = _functions[func](res, _rad);
		}
		catch(...)
		{
			throw EvaluatorException("Unexpected function: " + func);
		}
		return res;
	}
	const Element parseOperator(const Element calculated) const
	{
		Element res(0);
		eatWhiteSpaces();
		UnaryOperator uOp;
		BinaryOperator bOp;
		try
		{
			uOp = _unaryOperators[_ch];
			nextChar();
			res = uOp(calculated);
		}
		catch(...)
		{
			try
			{
				bOp = _binaryOperators[_ch];
				nextChar();
				res = bOp(calculated, parseExpression());
			}
			catch(...)
			{
				res = calculated;
			}
		}
		/*if (eat('^')) 
			res = _binaryOperators[0](res, parseExpression()); //Raising to power
		else if (eat('!')) //Factorial
			res = _unaryOperators[0](res);
		//Percents
		else if (eat('%'))
			res = _unaryOperators[1](res);
		//Modulo
		else if (eat('&')) 
			res = _binaryOperators[1](res, parseExpression());*/
		return res;
	}
};

template<class T>unsigned int Evaluator<T>::_freeId(0);

template<class T>ostream& operator<<(ostream& out, const Evaluator<T>& ev)
{
	return out << ev.parse();
}

#endif