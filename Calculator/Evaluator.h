//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include "HashMap.h"
#include "Exception.h"


// User of Evaluator defines signature of functions that will be used for specific type, as well as type itself
template<class Element, class Function>
class Evaluator
{
public:
	// Types of Unary and Binary operators. For readability
	//typedef const Element (*Function)(const Element&, const bool);
	typedef const Element (*UnaryOperator)(const Element&);
	typedef const Element (*BinaryOperator)(const Element&, const Element&);

	// Class for giving exceptions
	class EvaluatorException;

	Evaluator(const HashMap<char, UnaryOperator>&, const HashMap<char, BinaryOperator>&, const HashMap<string, Function>&, const string& exp="");

	Evaluator(const Evaluator<Element, Function>&);

	~Evaluator();

	Evaluator<Element, Function>& operator=(const Evaluator<Element, Function>&);

	operator Element();
    
	const string& getExpression() const;
	Evaluator<Element, Function>& setExpression(const string& exp);

	const char currentChar() const;

	const int currentPos() const;

    const Element parse() const;
private: 
	static size_t _freeId;
	const size_t _id;
	mutable size_t _pos;
	mutable char _ch;
    string _expression;
	HashMap<string, Function> _functions;
	HashMap<char, UnaryOperator> _unaryOperators;
	HashMap<char, BinaryOperator> _binaryOperators;

	//Reading next char of expression
	const Evaluator<Element, Function>& nextChar() const;

	//If passed a character we're currently reading move to the next character of expression and return true, otherwise return false
	const Evaluator<Element, Function>& eatWhiteSpaces() const;

    const bool eat(const char& c) const;

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

	const Element parseFunction(const size_t startPos) const;

	const Element parseOperator(const Element& calculated) const;
};

template<class Element, class Function>
size_t Evaluator<Element, Function>::_freeId(0);

template<class Element, class Function>
Evaluator<Element, Function>::Evaluator(const HashMap<char, UnaryOperator>& unaryOperators, const HashMap<char, BinaryOperator>& binaryOperators,
										const HashMap<string, Function>& functions, const string& exp)
		: _id(++_freeId), _pos(-1), _ch(0), _expression(exp), _unaryOperators(unaryOperators), _binaryOperators(binaryOperators), _functions(functions)
{
#ifndef NDEBUG
	cout << "Evaluator ID-" << _id << " created" << endl;
#endif
}

template<class Element, class Function>
Evaluator<Element, Function>::Evaluator(const Evaluator<Element, Function>& ev)
		: _id(++_freeId), _pos(-1), _ch(0), _expression(ev._expression), _unaryOperators(ev._unaryOperators), _binaryOperators(ev._binaryOperators), _functions(ev._functions)
{
#ifndef NDEBUG
	cout << "Evaluator ID-" << _id << " copied" << endl;
#endif
}

template<class Element, class Function>
Evaluator<Element, Function>::~Evaluator()
{
#ifndef NDEBUG
    cout << "Evaluator ID-" << _id << " destroyed" << endl;
#endif
}

template<class Element, class Function>
Evaluator<Element, Function>& Evaluator<Element, Function>::operator=(const Evaluator<Element, Function>& ev)
{
    if(this!=&ev)
    {
        _expression = ev._expression;
        _functions = ev._functions;
        _unaryOperators = ev._unaryOperators;
        _binaryOperators = ev._binaryOperators;
        _pos = -1;
        _ch = 0;
    }
    return *this;
}

template<class Element, class Function>
Evaluator<Element, Function>::operator Element()
{
    return parse();
}

template<class Element, class Function>
const string& Evaluator<Element, Function>::getExpression() const
{
    return _expression;
}

template<class Element, class Function>
Evaluator<Element, Function>& Evaluator<Element, Function>::setExpression(const string& exp)
{
    _expression=exp;
    _ch=0;
    _pos=-1;
    return *this;
}

template<class Element, class Function>
const char Evaluator<Element, Function>::currentChar() const
{
    return _ch;
}

template<class Element, class Function>
const int Evaluator<Element, Function>::currentPos() const
{
    return _pos;
}

template<class Element, class Function>
const bool Evaluator<Element, Function>::eat(const char& c) const
{
    eatWhiteSpaces();
    if (_ch == c)
    {
        nextChar();
        return true;
    }
    return false;
}

template<class Element, class Function>
const Evaluator<Element, Function>& Evaluator<Element, Function>::nextChar() const
{
    _ch = (++_pos < static_cast<int>(_expression.length())) ? _expression[_pos] : -1;
    return *this;
}

template<class Element, class Function>
const Evaluator<Element, Function>& Evaluator<Element, Function>::eatWhiteSpaces() const
{
    while(_ch == ' ')
        nextChar();
    return *this;
}

template<class Element, class Function>
const Element Evaluator<Element, Function>::parse() const
{
    if(_expression.empty())
        throw EvaluatorException("Expression undefined");
    nextChar();
    //Expression evaluation
    Element x;
    try
    {
        x = parseExpression();
    }
    catch(...)
    {
        throw EvaluatorException("Something bad happend. Check your input");
    }
    if (_pos < static_cast<int>(_expression.length()) && !Element::isElement(_expression, _pos))
        throw EvaluatorException(string("Unexpected character: ")+=_ch);
    _pos = -1;
    _ch = 0;
    return x;
}

template<class Element, class Function>
const Element Evaluator<Element, Function>::parseExpression() const
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

template<class Element, class Function>
const Element Evaluator<Element, Function>::parseTerm() const
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

template<class Element, class Function>
const Element Evaluator<Element, Function>::parseFactor() const
{
    //Firstly process sign
    //Return later evaluated result with appropriate sign
    if (eat('+'))
        return +parseFactor();
    if (eat('-'))
        return -parseFactor();
    //The result
    Element x(0);
    //Remember position from which processing started to get a substring from the whole expression
    size_t startPos(_pos);
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
        if(Element::isElement(_expression, startPos))
            x = Element::read(*this, startPos, &Evaluator<Element, Function>::nextChar);
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

template<class Element, class Function>
const Element Evaluator<Element, Function>::parseFunction(const size_t startPos) const
{
    Element res(0);
    while (_ch >= 'a' && _ch <= 'z')
        nextChar();
    string func(_expression.substr(startPos, _pos-startPos));
    res = parseFactor();
    try
    {
        res = Element::apply(*this, &Evaluator<Element, Function>::parseExpression, res, _functions[func]);
    }
    catch(...)
    {
        throw EvaluatorException("Unexpected function: " + func);
    }
    return res;
}

template<class Element, class Function>
const Element Evaluator<Element, Function>::parseOperator(const Element& calculated) const
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
    return res;
}

template<class Element, class Function>
class Evaluator<Element, Function>::EvaluatorException : public Exception
{
public:
    explicit EvaluatorException(const string& message="") : Exception(message){}
    EvaluatorException(const EvaluatorException& exception) : Exception(exception){}
    virtual ~EvaluatorException(){}
private:
    EvaluatorException& operator=(const EvaluatorException&);
};

template<class Element, class Function>ostream& operator<<(ostream& out, const Evaluator<Element, Function>& ev)
{
	return out << ev.parse();
}

#endif