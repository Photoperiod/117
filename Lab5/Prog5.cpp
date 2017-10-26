/////////////////////////////////////////////
//// Mark Philipp
//// 109941708
////
//// This Program simulates garbage collection. User will be able to choose to insert data into a node and delete data from a node.
//// User will be able to call on garbage collection to clean up unreachable nodes and wipe their data.
//// Program uses multiple Vectors that contain a custom class type called Node. These vectors are used to simulate a linked list of memory
////
//// compile:  $> g++ Prog5.cpp -std=c++11
//// run with: a.exe
/////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Node 
{
	// Custom class that will keep track of keys, names, and next nodes.
private:
	int key, next, name; // name will be used to identify which node this is.  Key is value of the node. Next is node this node points to
	bool mark;

public:

	///// Constructors /////

	Node()
	{
		// default constructor
		name = 0;
		key = 0;
		next = 0;
		mark = false;
	}

	Node(int newName, int newKey, int nextNode, bool newMark)
	{
		// custom constructor. Will be used for initialization
		name = newName;
		key = newKey;
		next = nextNode;
		mark = newMark;
	}

	///// Getters /////

	int GetName()
	{
		return name;
	}

	int GetKey()
	{
		return key;
	}

	int GetNext()
	{
		return next;
	}

	bool GetMark()
	{
		return mark;
	}

	///// Setters /////

	void SetKey(int newKey)
	{
		key = newKey;
	}

	void SetMark(int newMark)
	{
		mark = newMark;
	}

	int SetNext(int newVal)
	{
		next = newVal;
	}

	///// Utilities /////

	void Insertion(int newKey, int newNext)
	{
		// called when inserting data into this node. Updates variables appropriately.
		key = newKey;
		next = newNext;
	}
};

vector<Node> freeList, L1, L2, allNodes; // allNodes will be used to check for garbage nodes. will compare all lists to allNodes. If we don't find a node from allNodes in any of the other lists, then we know it's garbage

void InitializeNodes()
{
	// This function initializes the nodes of our freeList and allNodes vectors. Creates 10 Nodes for each list.
	for (int i = 1; i <= 10; i++)
	{
		if (i == 1) // handle first node creation
		{
			Node newNode(i, 0, i + 1, 0);
			freeList.push_back(newNode);
			allNodes.push_back(newNode);
		}
		else if (i < 10) // handle node creation between 1 and 10
		{
			Node newNode(i, 0, i + 1, 0);
			freeList.push_back(newNode);
			allNodes.push_back(newNode);
		}
		else // handle 10th node creation
		{
			Node newNode(i, 0, -1, 0);
			freeList.push_back(newNode);
			allNodes.push_back(newNode);
		}
	}
}

Node* GetNode(Node& nodeToFind)
{
	// This function is used for finding a node in the allNodes list that is equal to the node we're looking for.
	// Primarily will be used to update nodes in the allNodes list when inserting and deleting happens.
	// Example usage: GetNode(freeList.at(i))->Insertion( 1, 2, 0 );
	for (auto& node : allNodes)
	{
		// iterate over each node in allNodes
		if (node.GetName() == nodeToFind.GetName())
			// if we find the node we want, return a reference of it
			return &node;
	}

	//if we get here, our node doesn't exist in the list so we return null
	return NULL;
}

void ResetMarks()
{
	// This function resets all the marks to 0 after the sweep phase has executed.
	// Iterates over each list and sets all marks to 0

	for (auto& node : L1)
		node.SetMark(0);
	for (auto& node : L2)
		node.SetMark(0);
	for (auto& node : freeList)
		node.SetMark(0);
	for (auto& node : allNodes)
		node.SetMark(0);
}

void MarkPhase()
{
	// Any node that is in L1, L2, or freeList are all reachable.
	// If a node from allNodes does not exist in L1, L2, and freeList, then we know it's garbage because nobody is pointing to it.

	for (auto& node : freeList)
	{
		// iterate over freeList first
		node.SetMark(1);
		GetNode(node)->SetMark(1); // set node mark in allNodes list
	}
	for (auto& node : L1)
	{
		// iterate over L1 list
		node.SetMark(1);
		GetNode(node)->SetMark(1); // set node mark in allNodes list
	}
	for (auto& node : L2)
	{
		// iterate over L2 list
		node.SetMark(1);
		GetNode(node)->SetMark(1); // set node mark in allNodes list
	}
}

void SweepPhase()
{
	// iterate through allNodes and check their marks.
	// All nodes with mark value of 0 will be pushed into the beginning of free list.

	for (int i = 0; i < allNodes.size(); i++)
	{
		// Create LIFO insertion for vector
		if (allNodes.at(i).GetMark() == 0)
			freeList.insert(freeList.begin(), allNodes.at(i)); // insert at front of vector.
	}

	// now that the free list is populated, point each node to next and previous

	for (int i = 0; i < freeList.size(); i++)
	{
		// update each collected node's key to 0 and set their next pointers accordingly.
		if (i > -1 && i < freeList.size() - 1)
		{
			// update all nodes except final node in list
			freeList.at(i).Insertion(0, freeList.at(i + 1).GetName());
			GetNode(freeList.at(i))->Insertion(0, freeList.at(i + 1).GetName());
		}
		else if (i == freeList.size() - 1)
		{
			// update final node in list
			freeList.at(i).Insertion(0, -1);
			GetNode(freeList.at(i))->Insertion(0, -1);
		}
	}

	ResetMarks(); // reset the marks on all the nodes back to 0
}

void GarbageCollect()
{
	// This function calls the Mark and Sweep phases
	MarkPhase();
	SweepPhase();
}

void Insert(vector<Node> &nodeList, int key)
{
	// This function will insert a key value into a node

	if (freeList.size() == 0)
	{
		// if we have no space left in freeList, we run garbage collection.
		GarbageCollect();
		return;
	}

	nodeList.push_back(freeList.at(0)); // push a new node into the referenced list
	freeList.erase(freeList.begin()); // delete the node from the freeList

	if (nodeList.size() == 1)
	{
		// run insertion commands for the only node in nodeList.
		nodeList.back().Insertion(key, -1);
		GetNode(nodeList.back())->Insertion(key, -1);
	}
	else if (nodeList.size() > 1 && nodeList.size() <= 10)
	{
		// handle insertion for a list that has multiple nodes in it
		nodeList.back().Insertion(key, -1);
		nodeList.at(nodeList.size() - 2).SetNext(nodeList.back().GetName());

		GetNode(nodeList.back())->Insertion(key, -1);
		GetNode(nodeList.at(nodeList.size() - 2))->SetNext(nodeList.back().GetName());
	}
}

void Delete(vector<Node> &nodeList, int key)
{
	//  This function deletes a node from the referenced list. Node values are searched by the passed key.

	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList.at(i).GetKey() == key)
		{
			// found node to remove

			if (i > 0 && i < nodeList.size() - 1)
			{
				// set new next and previous nodes for the nodes before and after deleted node.
				nodeList.at(i - 1).SetNext(nodeList.at(i + 1).GetName());
				GetNode(nodeList.at(i - 1))->SetNext(nodeList.at(i + 1).GetName());
			}
			else if (i == nodeList.size() - 1 && nodeList.size() > 1)
			{
				// case for deleting last node in the list
				nodeList.at(i - 1).SetNext(-1);
				GetNode(nodeList.at(i - 1))->SetNext(-1);
			}

			nodeList.erase(nodeList.begin() + i);  // erase the node from the referenced list
		}
	}
}

void PrintMemory()
{
	// prints the current memory configuration of all the nodes in the system

	system("CLS"); // clear screen
	cout << endl << "\t";

	for (auto& node : allNodes)
		cout << node.GetName() << "\t"; // print names

	cout << endl << "Key\t";

	for (auto& node : allNodes)
		cout << node.GetKey() << "\t"; // print keys

	cout << endl << "Next\t";

	for (auto& node : allNodes)
		cout << node.GetNext() << "\t"; // print next nodes

	cout << endl << "Mark\t";

	for (auto& node : allNodes)
		cout << node.GetMark() << "\t"; // print marks

	cout << endl << endl;
}

void InsertMenu()
{
	// Creates the menu selection for the Insert functionality. 
	int listSelection, newValue;

	PrintMemory();

	while (true)
	{
		// loop this output until the user inputs a valid selection.

		cout << endl << "Select a list to insert into" << endl;
		cout << "1. L1\n2. L2" << endl;
		cin >> listSelection; // user selects list to insert into
		if (listSelection > 2 || listSelection < 1)
		{
			// only 1 and 2 are valid selections, loop again if 1 or 2 is not detected
			PrintMemory();
			cout << "INVALID INPUT" << endl;
			continue;
		}
		break; // break the loop when valid input is found
	}

	cout << endl << "Input a value to insert into the list" << endl;
	cin >> newValue; // user inputs value to insert into node

	if (listSelection == 1)
		Insert(L1, newValue); // insertion on L1
	else if (listSelection == 2)
		Insert(L2, newValue); // insertion on L2

	cout << endl;
}

void DeleteMenu()
{
	// Handles the menu for deleting Nodes from lists.
	int listSelection, value;
	
	PrintMemory();

	while (true)
	{
		// Loop until we receive a valid input
		cout << endl << "Select a list to delete from" << endl;
		cout << "1. L1\n2. L2" << endl;
		cin >> listSelection; // User input for list to delete from
		if (listSelection > 2 || listSelection < 1)
		{
			// If we don't receive a valid input, loop again.
			PrintMemory();
			cout << "INVALID INPUT" << endl;
			continue;
		}
		break; // We've received valid input. Break loop
	}

	cout << endl << "Input a value to delete" << endl;
	cin >> value; // User inputs value of node they want to delete.

	if (listSelection == 1)
		Delete(L1, value); // Delete from L1
	else if (listSelection == 2)
		Delete(L2, value); // Delete from L2

	cout << endl;
}

void ParseInput(int selection)
{
	// CAlled on to figure out what option user selected on main menu.
	if (selection == 1)
		InsertMenu();
	else if (selection == 2)
		DeleteMenu();
	else if (selection == 3)
		GarbageCollect();
	else if (selection == 4)
		exit(1);
}


int main(int argc, const char **argv)
{
	InitializeNodes();
	int input = 0; // will be used for user input

	system("CLS"); // clear screen
	PrintMemory();

	while (!cin.eof())
	{
		// loop until user selects to quit or until cin is invalid
		cout << "Select an Option:" << endl;
		cout << "1. Insert" << endl << "2. Delete" << endl << "3. Garbage Collect"  << endl << "4. Quit" << endl;
		cin >> input; // Get user input

		if (input > 4 || input < 1)
		{
			// If our input is invalid, loop again
			PrintMemory();
			cout << "INVALID INPUT." << endl << endl;
			continue;
		}

		ParseInput(input); // Figure out user's selection
		PrintMemory();
	}
}