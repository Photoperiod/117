/////////////////////////////////////////////
//// Interpreter for simplified infix expression with +,-,*,/.
//// Keyboard input, single digit numbers only and no spaces are allowed.
//// compile:  $>g++ prog1.cpp
//// run with: >2+3*4/2+3+4*2
/////////////////////////////////////////////

#include <cstdlib> //for atoi()
#include <iostream>
#include <ctype.h>
using namespace std;

int Exp(), Term(), Exp2(int), Term2(int), Fact();
void RemoveWS();
string prog; //string for reading 1-line input program
int indexx = 0; //global index for program string

int main(int argc, const char **argv)
{
	cout << ">";
	getline(cin, prog); // read whole line of data
	RemoveWS(); // Call Function to remove whitespace
	//cin >> prog;  //reads 1-line input expression (program)
	cout << "result= " << Exp() << endl;
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

int Exp()
{
	cout << "Exp()" << endl;
	return Exp2(Term());
}

int Term()
{
	cout << "Term()" << endl;
	return Term2(Fact());
}

int Exp2(int inp)
{
	cout << "Exp2(" << inp << ")" << endl;
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
	/* This function identifies the operator that comes after the parameter term.
	 If the operator is * or /, then precadence takes over and we recurse, passing (current parameter * next parameter) or 
	 (current parameter / next parameter) into Term2 again.
	 We get the next parameter by calling Fact().
	 If we detect a + or -, we decrement our index and let the function return the original parameter value.
	 The original parameter then gets passed to Exp2() to handle + and - cases.
	 */
	cout << "Term2(" << inp << ")" << endl;
	int result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		if (a == '*')
			result = Term2(result * Fact()); //handles consecutive * operators
		else if (a == '/')
			result = Term2(result / Fact()); //handles consecutive / operators
		else if (a == '+' || a == '-')     //if + or -, get back one position
			indexx--;
	}
	return result;
}

int Fact()
{
	// This function turns a character in the input data into an integer and returns that integer.
	// It's primarily used to get terms
	// Indexx is incremented whenever this function is called.
	cout << "Fact()" << endl;
	char a = prog.at(indexx++); //get one chr from program string
	return atoi(&a); //converts a char to a numeric number and return
}
