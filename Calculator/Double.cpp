//
// Created by Vladimir Fomin on 4/25/18.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include "Double.h"
#include "Evaluator.h"

Double::Double(const double value)
        : _value(value)
{}

Double::operator double&()
{
    return _value;
}

Double::operator const double() const
{
    return _value;
}

const Double Double::read(const Evaluator<Double, Function>& ev, const int startPos, const Evaluator<Double, Function>& (Evaluator<Double, Function>::*move)()const)
{
    Double res;
    if ((ev.getExpression()[startPos] == 'P' && ev.getExpression()[startPos+1] == 'I')
        || (ev.getExpression()[startPos] == 'p' && ev.getExpression()[startPos+1] == 'i'))
    {
        res = Double(M_PI);
        (ev.*move)();
        (ev.*move)();
    }
    else if (ev.getExpression()[startPos] == 'e' || ev.getExpression()[startPos] == 'E')
    {
        res = Double(M_E);
        (ev.*move)();
    }
    else
    {
        int endPos(startPos);
        while ((ev.getExpression()[endPos] >= '0' && ev.getExpression()[endPos] <= '9') || ev.getExpression()[endPos] == '.')
        {
            ++endPos;
            (ev.*move)();
        }
        //Parsing read number
        res = Double(atof((ev.getExpression().substr(startPos, endPos-startPos)).c_str()));
    }
    return res;
}

const bool Double::isElement(const string& expr, const int currentPos)
{
    const char currentChar(expr[currentPos]);
    return (currentChar >= '0' && currentChar <= '9') || currentChar == '.'
           || (currentChar == 'p' && expr[currentPos+1] == 'i') || (currentChar == 'P' && expr[currentPos+1] == 'I')
           || currentChar == 'e' || currentChar == 'E';

}

const Double Double::apply(const Evaluator<Double, Function>& ev, const Double (Evaluator<Double, Function>::*parseExpression)()const, const Double& current, const Function& currentF)
{
    return currentF(current);
}

//Overloading operators of division for throwing exceptions when dividing by zero and unary +, - for correct casting
Double& operator/=(Double& d1, const Double& d2)
{
    if(!d2)
        throw Double::BadDouble("Division by zero");
    d1.operator double&()/=d2;
    return d1;
}

const Double operator/(Double d1, const Double& d2)
{
    return operator/=( d1, d2);
}

const Double operator+(const Double& d)
{
    return d;
}

const Double operator-(const Double& d)
{
    return Double(-static_cast<double>(d));
}

//Operator for accurate console output
ostream& operator<<(ostream& out, const Double& d)
{
    return out << (abs(static_cast<double>(d))<1E-9?0:static_cast<double>(d));
}