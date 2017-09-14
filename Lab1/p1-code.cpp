/////////////////////////////////////////////
//// Interpreter for simplified infix expression with +,-,*,/.
//// Keyboard input, single digit numbers only and no spaces are allowed.
//// compile:  $>g++ prog1.cpp
//// run with: >2+3*4/2+3+4*2
/////////////////////////////////////////////

#include <cstdlib> //for atoi()
#include <iostream>
#include <ctype.h>
#include <cmath> // for pow()
#include <fstream> // file input
#include <vector> // used for symbol table

using namespace std;

class SymbolTable
{
public:
	SymbolTable(string newID, string newType, double newVal)
	{
		_id = newID;
		_type = newType;
		_value = newVal;
	}

	string GetID()
	{
		return _id;
	}

	void SetID(string newID)
	{
		_id = newID;
	}

	string GetType()
	{
		return _type;
	}

	void SetType(string newType)
	{
		_type = newType;
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
		_value = newValue;
	}

private:
	string _id, _type;
	double _value;
};

int Exp(), Term(), Exp2(int), Term2(int), Fact(), Fact2(int), Num();
void RemoveWS(), Exit(), Declarations(), Statements(), Declaration(string);
string prog; //string for reading 1-line input program
int indexx = 0; //global index for program string
ifstream input;
string word = ""; // next word we read

vector<SymbolTable> symbolTable;



int main(int argc, const char **argv)
{
	cout << ">";
	input.open(argv[1]);

	while (!input.eof())
	{
		input >> word;
		if (word == "program")
		{
			Declarations();
			Statements();
		}
		else
			exit(1);
	}

	input.close();
	//getline(cin, prog); // read whole line of data
	//RemoveWS(); // Call Function to remove whitespace
	//cin >> prog;  //reads 1-line input expression (program)
	//cout << "result= " << Exp() << endl;
}

void RemoveWS()
{
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
	exit(1);
}

void Declarations()
{
	input >> word;
	if (word == "begin")
		Exit();
	else if(word == "int" || word == "double")
		Declaration(word);

	Declarations();
}

void Statements()
{
	cout << "Hi";
}

void Declaration(string type)
{
	string id = "";
	bool foundSemiColon = false;

	while (!foundSemiColon)
	{
		input >> id;
		if (id.length() > 1 && id.find(',') != std::string::npos)
		{
			// remove comma from ID
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
			//if we find a single comma, reloop
			continue;

		if (id.find(';') != std::string::npos)
		{
			// remove semicolon from ID
			for (int i = 0; i < id.length(); i++)
			{
				if (id.at(i) == ';')
				{
					id.erase(i, 1);
					i--;
				}
			}

			foundSemiColon = true;
		}
		SymbolTable newItem(id, type, 0);
		symbolTable.push_back(newItem);
	}

	/*for (auto &i : symbolTable)
	{
		cout << i.GetType() << " " << i.GetID() << " " << i.GetDoubleValue() << endl;
	}*/
	exit(1);
}

int Exp()
{
	/* 
	this is used to get an expression. It calls Exp2, which lets us know which expression we have encountered
	*/
	return Exp2(Term());
}

int Term()
{
	/* 
	this function calls on Term2 to get a single Term. We call this function from the other functions so that we can grab
	the next term in the equation
	*/
	return Term2(Fact());
}

int Exp2(int inp)
{
	/*
	this function handles the + and - expressions. It will recurse on itself if it detects either operator.
	The recursion will call on Term() again to get the next term, which will then be sent to Term2 first, which will check the next operator as well.
	calling on Term2 again will make sure that if a * or / is found, then the next Term will be multiplied or divided instead of added or subtracted.
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
	/* 
	This function identifies the operator that comes after the parameter term.
	 If the operator is * or /, then precadence takes over and we recurse, passing (current parameter * next parameter) or 
	 (current parameter / next parameter) into Term2 again.
	 We get the next parameter by calling Fact().
	 If we detect a + or -, we decrement our index and let the function return the original parameter value to the Exp2 function to handle +, - operators
	 */
	int result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		if (a == '*')
			result = Term2(result * Fact()); //handles consecutive * operators
		else if (a == '/')
			result = Term2(result / Fact()); //handles consecutive / operators
		else if (a == '+' || a == '-' || a == ')')     //if + or -, get back one position
			indexx--;
	}
	return result;
}

int Fact()
{
	// This function turns a character in the input data into an integer and returns that integer.
	// It's primarily used to get terms
	// Indexx is incremented whenever this function is called.
	// This function is called by the Term function, which is called within the Exp2 function for + and - operators
	/*cout << "Fact()" << endl;
	char a = prog.at(indexx++); //get one chr from program string

	if (a == '(')
		return Exp();

	return atoi(&a); //converts a char to a numeric number and return*/
	return Fact2(Num());
}

int Fact2(int inp)
{
	/*
	handles exponent
	*/
	int result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		if (a == '^')
			result = Fact2(pow(result, Fact()));
		else
			indexx--;
	}
	return result;

}

int Num()
{
	char a = prog.at(indexx++);

	if (a == '(')
		return Exp();
	
	return atoi(&a);
}
