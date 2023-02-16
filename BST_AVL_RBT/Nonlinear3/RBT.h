
// This class is used to build a red black tree which is a type height balancing tree
// this tree uses coloring of nodes to keep the height under 2lg(n)
#pragma once
#include <string>
#include <chrono>

class RBT
{
public:
	RBT();
	~RBT();
	void insert(std::string input);
	void list(); 
	void printStats(); 
	void setInsertTime(std::chrono::duration<double> insertTime);
private:
	struct Node {
		Node * leftChild = nullptr;
		Node * rightChild = nullptr;
		Node * parent = nullptr;
		std::string value;
		int count = 1;
		bool color = true; // red = True, black = False
	};
	int treeHeight = 0;
	long itemsInTree = 0;
	long uniqueItemsInTree = 0;
	long keyComparisons = 0;
	long nodePointerChanges = 0;
	long recolorings = 0;
	std::chrono::duration<double> totalInsertTime;
	void setStats(); // sets and calculates some of the important
	void fixup(Node * node);
	void leftRotate(Node * node);
	void rightRotate(Node * node);
	void traverseSetStats(Node* node, int nodeDepth);
	Node * Root;
	Node * nil;
	void traverseAndPrint(Node * node);
	void printNodeInfo(Node * node);
};

