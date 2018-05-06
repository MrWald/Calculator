# Calculator
Expression parser
A programme which parses entered expression in string format and evaluates it. 
Parsing of values itself is managed by the class of values that needs to be evaluated. 
Evaluator class accepts Hash Maps with Unary and Binary operators and functions, 
which can be applied to a type passed in template arguments, and optionally the expression itself. 
Expression which should be evaluated can be changed after creation of the Evaluator object. 
Parsing of expression can be divided in 6 steps: 1.Sign; 2.Braces; 3.Values; 4.Functions/Operators; 5.Multiplication/Division; 6.Addition/Subtraction. 
