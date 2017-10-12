/////////////////////////////////////////////
//// Mark Philipp
//// 109941708
////
//// This program will read amino acid sequences. It will assign the sequence as a key to an unordered map.
//// It's value will be the ID that is associated with the sequence.
//// If a duplicate sequence is detected, then the same sequence will recognize multiple id's associated with it.
////
//// compile:  $> g++ lab4code.cpp -std=c++11
//// run with: a.exe filename
/////////////////////////////////////////////

#include <iostream>
#include <unordered_map>
#include <fstream> // file input
#include <vector> 
#include <string>

using namespace std;

int main(int argc, const char **argv)
{
	ifstream input;
	string nextWord = "";
	unordered_map<string, vector<string>> sequences; // will store our sequences as keys and our id's as strings inside a vector

	input.open(argv[1]); // begin input stream from command line argument file

	while (!input.eof())
	{
		string label; // keeps track of label
		getline(input, label); // grab label
		getline(input, nextWord); // grab sequence line

		if (sequences.find(nextWord) != sequences.end())
			// if our sequence already exists, then push the label onto the label vector for the sequence.
			sequences[nextWord].push_back(label); 
		else
		{
			// if sequence doesn't exist, create it.
			vector<string> labels; // create new vector
			labels.push_back(label); // push label onto vector
			sequences.insert({ nextWord, labels }); // create new key, value pair in sequence map.
		}
	}

	ofstream output("results"); // Will output results to a file.

	for (auto& keys : sequences)
	{
		if (keys.second.size() > 1)
		{
			// if the size of the sequence's label vector is more than 1, we need to list them all
			string labelString;

			for (auto& labels : keys.second)
			{
				// for every label, add the label + a comma to the label string
				labelString += labels + ", ";
			}

			// get rid of empty space and , at end of string
			labelString.pop_back();
			labelString.pop_back();

			cout << labelString << endl; // output to console
			output << labelString << endl; // output result to file
		}
		else
		{
			// if the sequence id vector size is 1, then we just output the label with no commas
			cout << keys.second.back() << endl;
			output << keys.second.back() << endl;
		}

		cout << keys.first << endl;
		output << keys.first << endl;
	}

	input.close();
	output.close();
}