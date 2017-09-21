/////////////////////////////////////////////
//// Interpreter for simplified infix expression with +,-,*,/.
////
//// File input. Variable names can be longer than a single character.
//// +, -, /, *, ^, (), and = operators all recognized
//// Valid keywords are "program", "begin", "end", and "print".
//// valid Types are int and double
//// int can be assigned a double value but the double will automatically cast to integer, removing decimal values.
////
//// compile:  $> g++ prog1.cpp -std=c++11
//// run with: a.exe filename
/////////////////////////////////////////////

#include <cstdlib> //for atoi()
#include <iostream>
#include <ctype.h>
#include <cmath> // for pow()
#include <fstream> // file input
#include <vector> // used for symbol table
#include <algorithm> // used for finding data in std::vector

using namespace std;

int Exp(), Term(), Exp2(int), Term2(int), Fact(), Fact2(double), Num();
void RemoveWS(), Exit(), Exit(string), Declarations(), Statements(), Declaration(string), Statement(string), PrintStatement(), AssignStatement(string);

class Symbol
{
	// This class will keep track of our variable names, types, and values.

public:
	Symbol(string newID, string newType, double newVal)
	{
		// 3-parameter constructor

		_id = newID;
		_type = newType;
		if (_type == "int")
			_value = (int)newVal; // cast newVal to an int if type is int.
		else if (_type == "double")
			_value = newVal;
		else
			Exit("Semantic Error: Type '" + newType + " Does Not Exist"); // check for invalid type symantic error
	}

	string GetID()
	{
		return _id;
	}

	string GetType()
	{
		return _type;
	}

	int GetIntValue()
	{
		return (int)_value;
	}

	double GetDoubleValue()
	{
		return _value;
	}

	void SetValue(double newValue)
	{
		if (_type == "int")
			_value = (int)newValue;
		else if (_type == "double")
			_value = newValue;
	}

private:
	string _id, _type;
	double _value;
};

string prog; //string for reading 1-line input program
int indexx = 0; //global index for program string
int line = 0; // keep track of line number in program
ifstream input, checkForEnd;
string word = ""; // next word we read
streampos oldPosition; // will keep track of input buffer

vector<Symbol> symbolTable; // vector of SymbolTable class objects

int main(int argc, const char **argv)
{
	string endString;
	bool doesEndExist = false;

	input.open(argv[1]); // begin input stream from command line argument file
	checkForEnd.open(argv[1]);

	while (!checkForEnd.eof())
	{
		checkForEnd >> endString;
		if (endString == "end")
			doesEndExist = true;
	}

	checkForEnd.close();

	if (!doesEndExist)
		Exit("Syntax Error: Keyword 'end' does not exist. Please put 'end' at the bottom of your program");

	oldPosition = input.tellg(); // store old input position in case we need to go backwards
	input >> word;
	line++;
	if (word == "program")
	{
		line++;
		Declarations();
		Statements();
	}
	else
		// throw error for lacking program keyword
		Exit("Syntax Error: No entry point for program. Please declare 'program' in your file");
	
	input.close();
}

void RemoveWS()
{

	if (prog.back() == ';')
		// remove semicolon from string
		prog.pop_back();
	else
		// throw syntax error for lacking semicolon
		Exit("Syntax Error: No semicolon at end of expression ");

	// Iterate over input string and remove whitespace
	for (int i = 0; i < prog.length(); i++)
	{
		if (prog.at(i) == ' ')
		{
			prog.erase(i, 1);
			i--; // String length has shortened by 1, decrement i to reflect this change
		}
	}

	
}

void Exit()
{
	// default exit function
	exit(1);
}

void Exit(string errorMessage)
{
	/*
	This overloaded function will throw an error that shows the line # the error is on, and the
	message that was passed as a parameter.
	*/
	if (line > 0)
		// This case is used for when "program" is missing from file
		cout << "Error on line " << line << ": " << errorMessage << endl;
	else
		cout << errorMessage << endl;

	exit(1);
}

void Declarations()
{
	/*
	This function will handle keywords "begin", "int", and "double". It will also throw
	an error if we don't detect any of these words in our declaration statements.
	*/

	oldPosition = input.tellg();
	input >> word;
	

	if (word == "begin")
	{
		// Once we read begin, we can end our declaration phase.
		line++;
		return;
	}
	else if (word == "int" || word == "double")
	{
		// Once we've identified a valid type, we will begin a declaration
		Declaration(word);
	}
	else if(word == "float" || word == "string" || word == "short" || word == "long" || word == "char")
		Exit("Semantic Error: Type '" + word + "' Does Not Exist"); // check for invalid type semantic error
	else
		// Throw a syntax error if we don't recognize any of the keywords in the file
		Exit("Lexical Error: Declaration must be 'begin', 'int', or 'double'");

	line++;
	Declarations(); // reiterate until we read "begin"
}

void Statement(string variableName)
{
	/*
	Handles statement execution. Specifically handles print and assigning values to variables

	Example syntax:
	a = 5+2;
	print a;
	*/

	if (variableName == "print")
	{
		PrintStatement();
	}
	else if (variableName.back() == ';')
		Exit("Syntax Error: Invalid Statement '" + variableName +"'");
	else 
	{
		for (auto &item : symbolTable)
		{
			// make sure that the passed in variable name exists in our symbol table
			if (variableName == item.GetID())
				AssignStatement(variableName); // start an assignment operation for our variable
		}
	}
}

void Statements()
{
	/*
	This function handles the parsing of statements. It will detect the "end" keyword and will
	call on the Statement function to execute statements.

	example syntax:
	a = 5+2;
	print a;
	end
	*/

	oldPosition = input.tellg(); // store old position in case we need to move backwards
	input >> word;
	
	// if we haven't read the "end" keyword, we will keep reading each line left-to-right and
	// executing statement code based on variable names or the "print" keyword
	if (word == "end")
		Exit();
	else
		Statement(word);

	line++;
	Statements();
}

void Declaration(string type)
{
	/*
	This function handles a declaration statement.

	Example syntax:
	int a, b, c;
	double d , e;
	double f;
	*/

	string id = "";
	bool foundSemiColon = false;

	while (!foundSemiColon)
	{
		// until we find a semicolon, keep looping

		oldPosition = input.tellg(); // save input buffer position
		input >> id;

		if (id.length() > 1 && id.find(',') != std::string::npos)
		{
			// remove comma from ID. handles statements like int a, b, c;
			for (int i = 0; i < id.length(); i++)
			{
				if (id.at(i) == ',')
				{
					id.erase(i, 1);
					i--;
				}
			}
		}
		else if (id == ",")
			//if we find a single comma, reloop. Handles case like int a , b , c
			continue;

		if (id.find(';') != std::string::npos)
		{
			// remove semicolon from ID if it exists
			for (int i = 0; i < id.length(); i++)
			{
				if (id.at(i) == ';')
				{
					id.erase(i, 1);
					i--;
				}
			}

			foundSemiColon = true; // we found semicolon. This will kill loop
		}

		for (int i = 0; i < id.length(); i++)
		{
			//check syntax of variable name for errors.
			if (!isalpha(id.at(i)))
				Exit("Lexical Error: Variable name " + id + " is invalid. Variable name must only contain letters");
		}

		Symbol newItem(id, type, 0.0); // create new Symbol object
		symbolTable.push_back(newItem); // store new symbol into back of vector
	}
}

void PrintStatement()
{
	/*
	This function will print the result of an expression or the value of the called upon variable
	example syntax:
	print a;
	print (5+2)^3;
	*/

	string newID = "";

	oldPosition = input.tellg(); // save input buffer position
	input >> newID;

	bool variableFound = false; // used for control statements

	if (newID.back() == ';')
		newID.pop_back(); // get rid of semicolon in name

	if (isalpha(newID.at(0)))
	{
		// search for variable ID in symbol table. Throw error if name not found. Print value if it is found.
		for (auto &item : symbolTable)
		{
			if (newID == item.GetID())
			{
				variableFound = true;
				if (item.GetType() == "int")
					cout << item.GetIntValue() << endl;
				else if (item.GetType() == "double")
					cout << item.GetDoubleValue() << endl;
			}
		}
		if (!variableFound)
			Exit("Semantic Error: Variable '" + newID + "' does not exist"); // throw Semantic error if variable doesn't exist.
	}
	else
	{
		// Execute print expression code
		input.seekg(oldPosition); // move input buffer back to old position so we can read line again
		getline(input, prog);
		RemoveWS();
		cout << Exp() << endl;
		indexx = 0;
	}
}

void AssignStatement(string newID)
{
	/*
	This code will deal with assignment statements. Variables can be assigned the value of another variable or
	the value of an expression.
	Example Syntax:

	a = (3+2) * 5;
	b = a;
	*/

	input >> word; // get operator
	bool foundVariableName = false, foundAssignmentVariable = false; // used for control statements

	if (word.at(0) == '=')
	{
		double newVal = 0; // newValue will be assigned to left-hand variable
		oldPosition = input.tellg();
		getline(input, prog);
		RemoveWS();

		if (isalpha(prog.at(0)))
		{
			// if right-hand side of statement is variable, put variable value into left-hand variable
			for (auto &item : symbolTable)
			{
				// search for ID in symbol table
				if (item.GetID() == prog)
				{
					foundAssignmentVariable = true;

					// assign the right-hand variable's value to newVal
					if (item.GetType() == "int")
						newVal = item.GetIntValue();
					else if (item.GetType() == "double")
						newVal = item.GetDoubleValue();
				}
			}
			if (!foundAssignmentVariable)
				Exit("Semantic Error: Variable '" + prog + "' does not exist"); // Throw error
		}
		else
		{
			// if we have an expression on right-side, calculate expression
			
			newVal = (double)Exp();
			indexx = 0;
		}

		for (auto &item : symbolTable)
		{
			// search for ID in symbol table
			if (item.GetID() == newID)
			{
				foundVariableName = true;
				item.SetValue(newVal); // set item value to new value
			}
		}

		if (!foundVariableName)
			// if we don't find the variable, throw semantic error
			Exit("Semantic Error: Variable '" + newID + "' does not exist");
	}
	else
		Exit("Syntax Error. Variable " + newID + " must have '=' operator to assign it a new value");

}

int Exp()
{
	/*
	This is used to get an expression. It calls Exp2, which lets us know which
	expression we have encountered.

	E -> E2 T
	*/

	return Exp2(Term());
}

int Term()
{
	/*
	This function calls on Term2 to get a single Term. We call this function
	from the other functions so that we can grab the next term in the equation
	*/

	return Term2(Fact());
}

int Exp2(int inp)
{
	/*
	This function handles the + and - expressions. It will recurse on itself if
	it detectes either operator. The recursion will call on Term() again to get
	the next term, which will then be sent to Term2 first, which will check the 
	next operator as well. Calling on Term2 again will make sure that if a * or /
	is found, then the next term will be multiplied or divided instead of added
	or subtracted.

	E2 -> E2 + T | E2 - T
	*/

	int result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		if (a == '+')
			result = Exp2(result + Term());  //handles T+T
		else if (a == '-')
			result = Exp2(result - Term());  //handles T-T
	}
	return result;
}

int Term2(int inp)
{
	/* This function identifies the operator that comes after the parameter term. If the operator is * or /, 
	then precadence takes over and we recurse, passing (current parameter * next parameter) or 
	(current parameter / next parameter) into Term2 again. 102 103  We get the next parameter by calling Fact().
	If we detect a + or - or ), we decrement our index and let the function return the original  
	parameter value to the Exp2 function to handle +, - operators
	
	This function also recognizes the end of a (E) expression by identifying right parentheses. 
	 T2 -> T2 * F | T2 / F | ')', '+', '-'
	*/

	int result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		if (a == '*')
			result = Term2(result * Fact()); //handles consecutive * operators
		else if (a == '/')
			result = Term2(result / Fact()); //handles consecutive / operators
		else if (a == '+' || a == '-' || a == ')')     //if + or - or ), get back one position
			indexx--;
	}
	return result;
}

int Fact()
{
	/*
	Used for recursing in Fact2. This allows us to satisfy num ^ F. Allows us to
	get F, so we can calculate num ^ F in right-recursive form.
	*/

	return Fact2((double)Num()); // have to cast Num() to double so that pow works properly
}

int Fact2(double inp)
{
	/*
	Reads a character and detects if it is '^'. If it is, then it recurses to get
	num ^ F. Else, we decrement index and return original inp value.

	F -> num | num ^ F
	*/

	double result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		if (a == '^')
			// pow is type double and requires parameters of type double.
			result = Fact2(pow((double)result, (double)Fact())); // recurse to get num ^F
		else
			indexx--;
	}
	return result;

}

int Num()
{
	/*
	This function reads a character from the input string and looks for '('. If
	it detects left parentheses, then it simply returns a number value. This is
	the lowest level of our grammar.

	Num -> 0 | 1 | 2 | ... | 9 | (E)
	*/

	char a = prog.at(indexx++);

	if (a == '(')
		return Exp();

	return (int)a - 48; // convert character to integer using ascii table. atoi was giving me problems.
}
