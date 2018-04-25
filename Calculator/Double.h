//Created by Volodymyr Fomin on 21/02/2018

#ifndef _DOUBLE_H_
#define _DOUBLE_H_

#include "Exception.h"

template<class Element, class Function>
class Evaluator;

// Class Double, contains plane double and implements methods for evaluating expressions with this type by Evaluator class
class Double
{
public:
	// Defining signature of functions we will use in Evaluator for this class
	typedef const Double (*Function)(const Double&);
	class BadDouble;

	explicit Double(const double value=0);

	operator double&();
	operator const double() const;

//	For parsing with Evaluator
	static const Double read(const Evaluator<Double, Function>&, const int startPos, const Evaluator<Double, Function>& (Evaluator<Double, Function>::*move)()const);

//	Determining whether read by Evaluator expression is of this type
	static const bool isElement(const string& expr, const int currentPos);

//	Applying function of specified signature, because Evaluator doesn't know how many arguments are passed to function of our signature
	static const Double apply(const Evaluator<Double, Function>&, const Double (Evaluator<Double, Function>::*parseExpression)()const, const Double& current, const Function& currentF);

private:
	double _value;
};

class Double::BadDouble : public Exception
{
public:
    explicit BadDouble(const string& message="") : Exception(message) {}
	BadDouble(const BadDouble& bd) : Exception(bd) {}
    virtual ~BadDouble(){}
private:
    BadDouble& operator=(const BadDouble&);
};

Double& operator/=(Double&, const Double&);

const Double operator/(Double, const Double&);

const Double operator+(const Double&);
const Double operator-(const Double&);

ostream& operator<<(ostream& out, const Double& d);

#endif