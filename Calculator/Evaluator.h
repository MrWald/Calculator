//Created by Volodymyr Fomin on 07/02/2018

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_
#include "HashMap.h"
using namespace std;

class Evaluator
{
	typedef const double (*Function)(const double, const bool);
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

	bool& rad();

    const double parse() const;
private: 
	static EvaluatorException _error;
	static unsigned int _freeId;

	static const HashMap<string, Function> _functions;
	static const HashMap<string, Function> getMap();

#pragma region Functions
	static const double sine(const double x, const bool rad)
	{
		return sin(!rad?toRadians(x):x);
	}
	static const double cosine(const double x, const bool rad)
	{
		return cos(!rad?toRadians(x):x);
	}
	static const double tg(const double x, const bool rad)
	{
		return tan(!rad?toRadians(x):x);
	}
	static const double ctg(const double x, const bool rad)
	{
		return 1./tan(!rad?toRadians(x):x);
	}
	static const double asine(const double x, const bool rad)
	{
		const double res(asin(x));
		return (!rad?toDegrees(res):res);
	}
	static const double acosine(const double x, const bool rad)
	{
		const double res(acos(x));
		return (!rad?toDegrees(res):res);
	}
	static const double atg(const double x, const bool rad)
	{
		const double res(atan(x));
		return (!rad?toDegrees(res):res);
	}
	static const double actg(const double x, const bool rad)
	{
		const double res(atan(1./x));
		return (!rad?toDegrees(res):res);
	}
	static const double ln(const double x, const bool)
	{
		return log(x);
	}
	static const double logTen(const double x, const bool)
	{
		return log10(x);
	}
	static const double squareRoot(const double x, const bool)
	{
		return sqrt(x);
	}
#pragma endregion

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