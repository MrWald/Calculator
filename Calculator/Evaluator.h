//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include <iostream>

using namespace std;

class Evaluator{
public:
	Evaluator(const string& s="", const bool& rad=true);
	Evaluator(const Evaluator&);
	~Evaluator();

	Evaluator& operator=(const Evaluator&);

	operator double();
    
	const string& getExpression() const;
	void setExpression(const string&);

	bool& rad() const;

    const double parse() const;
private: 
	static unsigned int _freeId;
	const unsigned int _id;
	static const double _eps;
	mutable int _pos; 
	mutable char _ch;
    string _expression;
    mutable bool _rad;

	//Transformation to radians/degrees
	static const double toRadians(const double);
	static const double toDegrees(const double);
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
	const double parseExpression() const;
    const double parseTerm() const;
    const double parseFactor() const;
	const double parseNumber(const int) const;
	const double parseFunction(const int) const;
	const double parseOperator(const double) const;
};

#endif