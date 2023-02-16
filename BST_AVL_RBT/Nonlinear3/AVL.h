
// This class is used to create a type of height balanced tree called an AVL tree
// this tree uses balance factors to keep the trees height under 1.44lg(n)
#pragma once
#include <string>
#include <chrono>

class AVL
{
public:
	AVL();
	~AVL();
	void insert(std::string input);
	void list(); 
	void printStats();
	void setInsertTime(std::chrono::duration<double> insertTime);
private:
	struct Node {
		Node * leftChild = nullptr;
		Node * rightChild = nullptr;
		std::string value;
		int count = 1;
		int balanceFactor = 0;
	};
	int treeHeight = 0;
	long itemsInTree = 0;
	long uniqueItemsInTree = 0;
	long keyComparisons = 0;
	long nodePointerChanges = 0;
	long balanceFactorChanges = 0;
	std::chrono::duration<double> totalInsertTime;
	void setStats();
	void traverseSetStats(Node* node, int nodeHeight);
	Node * Root;
	void traverseAndPrint(Node * node);
	void printNodeInfo(Node * node);
};

