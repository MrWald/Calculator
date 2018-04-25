//Created by Volodymyr Fomin on 22/02/2018

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "Double.h"
#include "AComplex.h"
#include "HashMap.h"
#include "Evaluator.h"

extern bool Rad;

//Transformation to radians/degrees
const double toRadians(const double);
const double toDegrees(const double);

//Factorial
const size_t factorial(const size_t&);

const Double power(const Double&, const Double&);

const Double fac(const Double&);

const Double modulo(const Double&, const Double&);
const Double percent(const Double&);
const Double sine(const Double&);
const Double cosine(const Double&);
const Double tg(const Double&);
const Double ctg(const Double&);
const Double asine(const Double&);
const Double acosine(const Double&);
const Double atg(const Double&);
const Double actg(const Double&);
const Double ln(const Double&);
const Double logTen(const Double&);
const Double squareRoot(const Double&);

const HashMap<char, Evaluator<Double, Double::Function>::UnaryOperator> getUOps();

const HashMap<char, Evaluator<Double, Double::Function>::BinaryOperator> getBOps();

const HashMap<string, Double::Function> getFunctions();

const HashMap<char, Evaluator<AComplex, AComplex::Function>::BinaryOperator> getBOpsComplex();

const HashMap<string, AComplex::Function> getFunctionsComplex();

#endif