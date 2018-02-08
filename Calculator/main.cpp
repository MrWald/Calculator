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
		"help", "rad", "exit", "(", ")", "+", "-", "*", "/", "!", "^", "%", "&", 
		"sin", "cos", "tg", "ctg", "asin", "acos", "atg", "actg", "ln", "log", "sqrt"
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