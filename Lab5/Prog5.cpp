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

int SetNext(int newVal)
{
	next = newVal;
}

void Insertion(int newKey, int newNext, int newPrev)
{
	// called when insertion happens. Updates variables appropriately.
	key = newKey;
	next = newNext;
	previous = newPrev;
}

};

vector<Node> freeList, L1, L2;

void InitializeNodes()
{
	for (int i = 1; i <= 10; i++)
	{
		if (i == 1) // handle first node creation
		{
			Node newNode(i, 0, i + 1, -1, 0);
			freeList.push_back(newNode);
		}
		else if (i < 10) // handle node creation between 1 and 10
		{
			Node newNode(i, 0, i + 1, i - 1, 0);
			freeList.push_back(newNode);
		}
		else // handle 10th node creation
		{
			Node newNode(i, 0, -1, i - 1, 0);
			freeList.push_back(newNode);
		}

		//cout << freeList.at(i - 1).GetName() << "\t" << freeList.at(i-1).GetKey() << "\t" << freeList.at(i-1).GetNext() << "\t" << freeList.at(i-1).GetMark() << endl;
	}
}

void GarbageCollect()
{

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

	if (nodeList.size() == 0)
		nodeList.back().Insertion(key, -1, -1);
	else if (nodeList.size() > 1 && nodeList.size() < 10)
	{
		nodeList.back().Insertion(key, -1, nodeList.at(nodeList.size() - 2).GetName());
		nodeList.at(nodeList.size() - 2).SetNext(nodeList.back().GetName());
	}
	else if (nodeList.size() > 1 && nodeList.size() == 10)
	{
		nodeList.back().Insertion(key, -1, nodeList.at(nodeList.size() - 2).GetName());
		nodeList.at(nodeList.size() - 2).SetNext(nodeList.back().GetName());
	}
}

void PrintMemory()
{
	cout << "\t";

	for (int i = 1; i <= 10; i++)
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

	cout << endl;
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

	/*for (auto& i : freeList)
		cout << i.GetName() << endl;

	for (auto& i : L1)
		cout << i.GetName() << endl;

	for (auto& i : L2)
		cout << i.GetName() << endl;*/
	PrintMemory();
}