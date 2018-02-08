//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_
#include <iostream>

using namespace std;

class Evaluator{
private: 
	static unsigned int _freeId;
	const unsigned int _id;
	int _pos; 
	char _ch;
    string _expression;
    bool _rad;

	//Transformation to radians/degrees
	static const double toRadians(const double&);
	static const double toDegrees(const double&);

	//Reading next char of expression
	void nextChar();

	//If passed a character we're currently reading move to the next character of expression and return true, otherwise return false
    const bool eat(const char&);

	/*
	Priorities:
		1.Sign
		3.Braces
		3.Numbers
		4.Functions/Operators
		5.Multiplication/Division
		6.Addition/Subtraction
     */
	const double parseExpression();
    const double parseTerm();
    const double parseFactor();

    //Factorial
    static const unsigned long long factorial(const unsigned int&);

public:
	Evaluator(const string& s="", const bool& rad=true);
	Evaluator(const Evaluator&);
	~Evaluator();

	Evaluator& operator=(const Evaluator&);
    
	const string& getExpression() const;

	void setExpression(const string&);

	bool& rad();

	const bool& rad() const;
    const double parse();
};

#endif