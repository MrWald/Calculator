//Created by Volodymyr Fomin on 07/02/2018

#include <sstream>
#include "Evaluator.h"

using namespace std;

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
	Evaluator evaluator;
	while(true)
	{
		cout << "Enter command or expression" << endl;
		getline(cin, expression);
		if(!expression.compare("exit"))
			break;
		else if(!expression.compare("help"))
		{
			listCommands(commands, commandsSize);
		}
		else if(!expression.compare("rad"))
		{
			evaluator.rad() = !evaluator.rad();
			cout << "Radians turned " << (evaluator.rad()?"on":"off") << endl;
		}
		else
		{
			try
			{
				evaluator.setExpression(expression);
				cout << evaluator.parse() << endl;
			}
			catch(invalid_argument e){
				cerr << e.what() << endl;
				evaluator.setExpression("");
				cout << "Enter 'help' if you forgot appropriate expressions" << endl;
			}
		}
		expression="";
	}
	return 0;
}