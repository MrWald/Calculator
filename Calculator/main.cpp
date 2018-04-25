//Created by Volodymyr Fomin on 07/02/2018

//Evaluator class accepts specified class, which will be parsed from string with particular signature of functions.
//For Evaluator to do that, class that can be parsed must implement following methods:
//1.read - evaluator passes its object, which currently reads an object of this type from string, postion where Evaluator is currently located in and function which moves evaluator on next char in passed string
//2.isElement - determining whether character, which evaluator object currently reading may be element of object of class to be processed. Accepts string and position which evaluator currently reading
//3.apply - Applying function of specified signature. Passed object which currently reads object of following class from string, function to start parsing rest of expression, currently read element and current function of specified signature read from string 

#include <iostream>
#include "Functions.h"
#include "Exception.h"
#include "Evaluator.h"

void listCommands(const string commands[], const unsigned int& size)
{
	for(unsigned int i(0);i<size;i++)
		cout << commands[i] << endl;
	return;
}

int main()
{
	const unsigned int commandsSize(26);
	string commands[commandsSize]=
	{
		"help - list all available commands, functions and operators", "rad - read angle in radians or degrees", "complex - enable reading complex numbers", "exit - exit from the programm", 
		"( - opening brace", ") - closing brace, optional", "+ - addition", "- - subtraction", "* - multiplication", "/ - division", 
		"! - factorial, put in the end of expression(Only real numbers)", "^ - raise to power", "% - turn to percents(Only real numbers)", "& - modulo(Only real numbers)", 
		"sin - sine(Only real numbers)", "cos - cosine(Only real numbers)", "tg - tangent(Only real numbers)", "ctg - cotangent(Only real numbers)", 
		"asin - arcsine(Only real numbers)", "acos - arccosine(Only real numbers)", "atg - arctangent(Only real numbers)", "actg - arccotangent(Only real numbers)", 
		"ln - natural logarithm(Only real numbers)", "log - logarithm to base 10(Only real numbers)", "sqrt - square root(Only real numbers)", "conj - conjugated number to entered(Only complex numbers)"
	};
	string expression;
	Evaluator<Double, Double::Function> evaluator(getUOps(), getBOps(), getFunctions());
	Evaluator<AComplex, AComplex::Function> evC(
		HashMap<char, Evaluator<AComplex, AComplex::Function>::UnaryOperator>(0), getBOpsComplex(), getFunctionsComplex());
	bool readComplex(false);
	while(true)
	{
		cout << "\nEnter command or expression" << endl;
		getline(cin, expression);
		if(expression=="exit")
			break;
		else if(expression=="help")
		{
			listCommands(commands, commandsSize);
		}
		else if(expression=="rad")
		{
			Rad = !Rad;
			cout << "Radians turned " << (Rad?"on":"off") << endl;
		}
		else if(expression=="complex")
		{
			readComplex = !readComplex;
			cout << "Reading complex numbers " << (readComplex?"on":"off") << endl;
		}
		else
		{
			try
			{
				if(!readComplex)
				{
					evaluator.setExpression(expression);
					cout << evaluator << endl;
				}
				else
				{
					evC.setExpression(expression);
					cout << evC << endl;
				}
			}
			catch(const Exception& e)
			{
				cerr << e << endl;
				evaluator.setExpression("");
				cout << "Enter 'help' if you forgot appropriate expressions" << endl;
			}
		}
		expression="";
	}
	return 0;
}