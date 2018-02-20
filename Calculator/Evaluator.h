//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_
#include "HashMap.h"
using namespace std;

class Evaluator
{
	typedef double (*Function)(double);
public:
	class EvaluatorException
	{
	public:
		explicit EvaluatorException(const string& error="");
		EvaluatorException(const EvaluatorException&);
		~EvaluatorException();
		EvaluatorException& operator=(const EvaluatorException& exception);
		const string& error() const;
		string& error();
		const unsigned int& getId() const;
		static const unsigned int& amount();
	private:
		static unsigned int _freeId;

		const unsigned int _id;
		string _message;
	};

	explicit Evaluator(const string& exp="", const bool& rad=true);
	Evaluator(const Evaluator&);
	~Evaluator();

	Evaluator& operator=(const Evaluator&);

	operator double();
    
	const string& getExpression() const;
	void setExpression(const string&);

	static bool& rad();

    const double parse() const;
private: 
	static EvaluatorException _error;
	static unsigned int _freeId;
	static const HashMap<string, Function> _functions;
	static HashMap<string, Function> getMap();
	static double ctan(double x)
	{
		return 1./tan(x);
	}
	static double actan(double x)
	{
		return atan(1./x);
	}

	const unsigned int _id;
	static const double _eps;
	mutable int _pos; 
	mutable char _ch;
    string _expression;
    static bool _rad;

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