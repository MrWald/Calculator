//Created by Volodymyr Fomin on 07/02/2018

//Evaluator class accepts specified class, which will be parsed from string with particular signature of functions.
//For Evaluator to do that, class that can be parsed must implement following methods:
//1.read - evaluator passes its object, which currently reads an object of this type from string, postion where Evaluator is currently located in and function which moves evaluator on next char in passed string
//2.isElement - determining whether character, which evaluator object currently reading may be element of object of class to be processed. Accepts string and position which evaluator currently reading
//3.apply - Applying function of specified signature. Passed object which currently reads object of following class from string, function to start parsing rest of expression, currently read element and current function of specified signature read from string 

#include <iostream>
#include "Functions.h"

void listCommands(const string commands[], const unsigned int& size)
{
	for(unsigned int i(0);i<size;i++)
		cout << commands[i] << endl;
	return;
}

int main()
{
	const unsigned int commandsSize(24);
	string commands[commandsSize]=
	{
		"help - list all available commands, functions and operators", "rad - read angle in radians or degrees", "exit - exit from the programm", 
		"( - opening brace", ") - closing brace, optional", "+ - addition", "- - subtraction", "* - multiplication", "/ - division", 
		"! - factorial, put in the end of expression", "^ - raise to power", "% - turn to percents", "& - modulo", 
		"sin - sine", "cos - cosine", "tg - tangent", "ctg - cotangent", "asin - arcsine", "acos - arccosine", "atg - arctangent", "actg - arccotangent", 
		"ln - natural logarithm", "log - logarithm to base 10", "sqrt - square root"
	};
	string expression;
	Evaluator<Double, Double::Function> evaluator(getUOps(), getBOps(), getFunctions());
	while(true)
	{
		cout << "\nEnter command or expression" << endl;
		getline(cin, expression);
		if(!expression.compare("exit"))
			break;
		else if(!expression.compare("help"))
		{
			listCommands(commands, commandsSize);
		}
		else if(!expression.compare("rad"))
		{
			Rad = !Rad;
			cout << "Radians turned " << (Rad?"on":"off") << endl;
		}
		else
		{
			try
			{
				evaluator.setExpression(expression);
				cout << evaluator << endl;
			}
			catch(const Evaluator<Double, Double::Function>::EvaluatorException& e)
			{
				cerr << e.error() << endl;
				evaluator.setExpression("");
				cout << "Enter 'help' if you forgot appropriate expressions" << endl;
			}
		}
		expression="";
	}
	return 0;
}