//
// Created by Volodymyr Fomin on 2/2/18.
//
// Complex numbers
// in cartesian (algebraic) form

#ifndef _ACOMPLEX_H_
#define _ACOMPLEX_H_

#include <iostream>

using namespace std;

template<class Element, class Function>
class Evaluator;

class AComplex
{
private:
    class TComplex;
public:
	// Defining signature of functions we will use in Evaluator for this class
	typedef const AComplex (*Function)(const AComplex&);
	AComplex (const double re=0, const double im=0);
	AComplex (const AComplex&);
	~AComplex();
	AComplex& operator= (const AComplex&);

	operator const double() const;

	double& re();
	double& im();

	const double re() const;
	const double im() const;

	const double mod() const;
	const double arg() const;

    const size_t getId() const;

    static const size_t amount();

	//	For parsing with Evaluator
	static const AComplex read(const Evaluator<AComplex, Function>&, const int startPos, const Evaluator<AComplex, Function>& (Evaluator<AComplex, Function>::*move)()const);

//	Determining whether read by Evaluator expression is of this type
	static const bool isElement(const string& expr, const int currentPos);

//	Applying function of specified signature, because Evaluator doesn't know how many arguments are passed to function of our signature
	static const AComplex apply(const Evaluator<AComplex, Function>&, const AComplex (Evaluator<AComplex, Function>::*parseExpression)()const, const AComplex& current, const Function& currentF);

	static const AComplex conj(const AComplex&);

	static const AComplex power(const AComplex& ac, const AComplex& n);
private:

	AComplex (const TComplex&);

	static const double parseDouble(const int& startPos, int& endPos, const Evaluator<AComplex, Function>& ev, const Evaluator<AComplex, Function>& (Evaluator<AComplex, Function>::*move)()const);
	static size_t _freeId;
	const size_t _id;
	double _re, _im;
};

class AComplex::TComplex
{
public:
    explicit TComplex(const double ro, const double phi);
    TComplex(const AComplex&);

    const double re() const;

    const double im() const;

    static const double limitPhi(const double);

    static const TComplex power(const TComplex& c, const double n);
private:
    double _ro, _phi;
};

AComplex& operator+=(AComplex& c1, const AComplex& c2);
AComplex& operator-=(AComplex& c1, const AComplex& c2);
AComplex& operator*=(AComplex& c1, const AComplex& c2);
AComplex& operator/=(AComplex& c1, const AComplex& c2);

const AComplex operator+ (const AComplex&, const AComplex&);
const AComplex operator- (const AComplex&, const AComplex&);

const AComplex operator+ (const AComplex&);
const AComplex operator- (AComplex);

const bool operator== (const AComplex&, const AComplex&);

const bool operator!= (const AComplex&, const AComplex&);

ostream& operator<<(ostream&, const AComplex&);
istream& operator>>(istream&, AComplex&);

#endif