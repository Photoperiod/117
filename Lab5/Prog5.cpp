#include <list>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Node 
{
private:
	int key, next, previous, name; // name will be used to identify which node this is
	bool mark;

public:
	Node()
	{
		name = 0;
		key = 0;
		next = 0;
		previous = 0;
		mark = false;
	}

	Node(int newName, int newKey, int nextNode, int previousNode, bool newMark)
	{
		name = newName;
		key = newKey;
		next = nextNode;
		previous = previousNode;
		mark = newMark;
	}

	int GetName()
	{
		return name;
	}

	int GetKey()
	{
		return key;
	}

	void SetKey(int newKey)
	{
		key = newKey;
	}

	int GetNext()
	{
		return next;
	}

	int GetPrevious()
	{
		return previous;
	}

	bool GetMark()
	{
		return mark;
	}

	void SetMark(int newMark)
	{
		mark = newMark;
	}

	int SetNext(int newVal)
	{
		next = newVal;
	}

	int SetPrevious(int newVal)
	{
		previous = newVal;
	}

	void Insertion(int newKey, int newNext, int newPrev)
	{
		// called when insertion happens. Updates variables appropriately.
		key = newKey;
		next = newNext;
		previous = newPrev;
	}

	void Deletion()
	{
		// called when a delete happens. Removes previous node
		previous = -1;
	}

	/*bool operator== (Node& rhs)
	{
		return (this->GetKey() == rhs.GetKey());
	}*/
};

vector<Node> freeList, L1, L2, allNodes; // allNodes will be used to check for garbage nodes. will compare all lists to allNodes. If we don't find a node from allNodes in any of the other lists, then we know it's garbage

void InitializeNodes()
{
	for (int i = 1; i <= 10; i++)
	{
		if (i == 1) // handle first node creation
		{
			Node newNode(i, 0, i + 1, -1, 0);
			freeList.push_back(newNode);
			allNodes.push_back(newNode);
		}
		else if (i < 10) // handle node creation between 1 and 10
		{
			Node newNode(i, 0, i + 1, i - 1, 0);
			freeList.push_back(newNode);
			allNodes.push_back(newNode);
		}
		else // handle 10th node creation
		{
			Node newNode(i, 0, -1, i - 1, 0);
			freeList.push_back(newNode);
			allNodes.push_back(newNode);
		}

		//cout << freeList.at(i - 1).GetName() << "\t" << freeList.at(i-1).GetKey() << "\t" << freeList.at(i-1).GetNext() << "\t" << freeList.at(i-1).GetMark() << endl;
	}
}

Node* GetNode(Node& nodeToFind)
{
	// This function is used for finding a node in the allNodes list that is equal to the node we're looking for.
	// Primarily will be used to update nodes in the allNodes list when inserting and deleting happens
	for (auto& node : allNodes)
	{
		if (node.GetName() == nodeToFind.GetName())
			return &node;
	}

	return NULL;
}

void ResetMarks()
{
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
		node.SetMark(1);
		GetNode(node)->SetMark(1); // set node mark in allNodes list
	}
	for (auto& node : L1)
	{
		node.SetMark(1);
		GetNode(node)->SetMark(1);
	}
	for (auto& node : L2)
	{
		node.SetMark(1);
		GetNode(node)->SetMark(1);
	}
}

void SweepPhase()
{
	// iterate through allNodes and check their marks.
	// All nodes with mark value of 0 will be pushed into the beginning of free list.

	for (int i = allNodes.size() - 1; i > -1; i--)
	{
		// reverse iterate through the list so we can easily create LIFO behavior using vector push_back
		if (allNodes.at(i).GetMark() == 0)
			freeList.push_back(allNodes.at(i));
	}

	// now that the free list is populated, point each node to next and previous

	for (int i = 0; i < freeList.size(); i++)
	{
		if (i == 0)
		{
			freeList.at(i).Insertion(freeList.at(i).GetKey(), freeList.at(i + 1).GetName(), -1);
			GetNode(freeList.at(i))->Insertion(freeList.at(i).GetKey(), freeList.at(i + 1).GetName(), -1);
		}
		else if (i > 0 && i < freeList.size() - 1)
		{
			freeList.at(i).Insertion(freeList.at(i).GetKey(), freeList.at(i + 1).GetName(), freeList.at(i - 1).GetName());
			GetNode(freeList.at(i))->Insertion(freeList.at(i).GetKey(), freeList.at(i + 1).GetName(), freeList.at(i - 1).GetName());
		}
		else if (i == freeList.size() - 1)
		{
			freeList.at(i).Insertion(freeList.at(i).GetKey(), -1, freeList.at(i - 1).GetName());
			GetNode(freeList.at(i))->Insertion(freeList.at(i).GetKey(), -1, freeList.at(i - 1).GetName());
		}
	}

	ResetMarks();
}

void GarbageCollect()
{
	MarkPhase();
	SweepPhase();
}

void Insert(vector<Node> &nodeList, int key)
{
	if (freeList.size() == 0)
	{
		GarbageCollect();
		return;
	}

	nodeList.push_back(freeList.at(0));
	freeList.erase(freeList.begin());

	if (nodeList.size() == 1)
	{
		nodeList.back().Insertion(key, -1, -1);
		GetNode(nodeList.back())->Insertion(key, -1, -1);
	}
	else if (nodeList.size() > 1 && nodeList.size() <= 10)
	{
		nodeList.back().Insertion(key, -1, nodeList.at(nodeList.size() - 2).GetName());
		nodeList.at(nodeList.size() - 2).SetNext(nodeList.back().GetName());

		GetNode(nodeList.back())->Insertion(key, -1, nodeList.at(nodeList.size() - 2).GetName());
		GetNode(nodeList.at(nodeList.size() - 2))->SetNext(nodeList.back().GetName());
	}
}

void Delete(vector<Node> &nodeList, int key)
{
	/*if (freeList.size() == 0)
	{
		GarbageCollect();
		return;
	}*/

	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList.at(i).GetKey() == key)
		{
			// found node to remove
			nodeList.at(i).Deletion();
			GetNode(nodeList.at(i))->Deletion();

			if (i == 0 && nodeList.size() > 1)
			{
				nodeList.at(i + 1).SetPrevious(-1);
				GetNode(nodeList.at(i + 1))->SetPrevious(-1);
			}
			else if (i > 0 && i < nodeList.size() - 1)
			{
				// set new next and previous nodes for the nodes before and after deleted node.
				nodeList.at(i - 1).SetNext(nodeList.at(i + 1).GetName());
				nodeList.at(i + 1).SetPrevious(nodeList.at(i - 1).GetName());

				GetNode(nodeList.at(i - 1))->SetNext(nodeList.at(i + 1).GetName());
				GetNode(nodeList.at(i + 1))->SetPrevious(nodeList.at(i - 1).GetName());
			}
			else if (i == nodeList.size() - 1 && nodeList.size() > 1)
			{
				// case for deleting last node in the list
				nodeList.at(i - 1).SetNext(-1);
				GetNode(nodeList.at(i - 1))->SetNext(-1);
			}

			nodeList.erase(nodeList.begin() + i);
		}
	}
}

void PrintMemory()
{
	cout << "\t";

	for (auto& node : allNodes)
		cout << node.GetName() << "\t";

	cout << endl << "Key\t";

	for (auto& node : allNodes)
		cout << node.GetKey() << "\t";

	cout << endl << "Next\t";

	for (auto& node : allNodes)
		cout << node.GetNext() << "\t";

	cout << endl << "Mark\t";

	for (auto& node : allNodes)
		cout << node.GetMark() << "\t";

	cout << endl;
	/*for (int i = 1; i <= 10; i++)
	{
		cout << i << "\t";
	}

	cout << endl << "Key\t";

	for (int i = 1; i <= 10; i++)
	{
		// Print out the keys by iterating over each list and finding the proper node names
		if (freeList.size() > 0)
		{
			for (auto& node : freeList)
			{
				if (node.GetName() == i)
				{
					cout << node.GetKey() << "\t";
					break;
				}
			}

			for (auto& node : L1)
			{
				if (node.GetName() == i)
				{
					cout << node.GetKey() << "\t";
					break;
				}
			}

			for (auto& node : L2)
			{
				if (node.GetName() == i)
				{
					cout << node.GetKey() << "\t";
					break;
				}
			}
		}
	}

	cout << endl << "Next\t";

	for (int i = 1; i <= 10; i++)
	{
		// Print out the next values for each node
		if (freeList.size() > 0)
		{
			for (auto& node : freeList)
			{
				if (node.GetName() == i)
				{
					cout << node.GetNext() << "\t";
					break;
				}
			}

			for (auto& node : L1)
			{
				if (node.GetName() == i)
				{
					cout << node.GetNext() << "\t";
					break;
				}
			}

			for (auto& node : L2)
			{
				if (node.GetName() == i)
				{
					cout << node.GetNext() << "\t";
					break;
				}
			}
		}
	}

	cout << endl << "Mark\t";

	for (int i = 1; i <= 10; i++)
	{
		// Print out the Mark
		if (freeList.size() > 0)
		{
			for (auto& node : freeList)
			{
				if (node.GetName() == i)
				{
					cout << node.GetMark() << "\t";
					break;
				}
			}

			for (auto& node : L1)
			{
				if (node.GetName() == i)
				{
					cout << node.GetMark() << "\t";
					break;
				}
			}

			for (auto& node : L2)
			{
				if (node.GetName() == i)
				{
					cout << node.GetMark() << "\t";
					break;
				}
			}
		}
	}

	cout << endl;*/
}


int main(int argc, const char **argv)
{
	InitializeNodes();
	Insert(L1, 3); //insert (attach) a node with key value 3 into list1
	Insert(L1, 1); //insert (attach) a node with key value 1 into list1
	Insert(L2, 4);
	Insert(L1, 5);
	Insert(L2, 2);
	Insert(L2, 9);
	Insert(L2, 8);
	Insert(L1, 4);

	Delete (L1, 4);
	Delete (L2, 8);
	Delete (L1, 1);
	Delete (L2, 4);
	Delete (L1, 5);
	/*for (auto& i : freeList)
		cout << i.GetName() << endl;

	for (auto& i : L1)
		cout << i.GetName() << endl;

	for (auto& i : L2)
		cout << i.GetName() << endl;*/
	GarbageCollect();
	PrintMemory();
}