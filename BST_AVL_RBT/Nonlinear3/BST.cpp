
#include "stdafx.h"
#include "BST.h"
#include <iostream>
#include <string>


BST::BST()
{
	Root = nullptr;
}


BST::~BST()
{
}

// this will insert a new node to the tree if the value is not already in the tree
// if the input value is in the tree then the count will be incremented
void BST::insert(std::string input)
{
	Node* currentNode = Root;
	Node* previousNode = nullptr;

	// keep getting the next node until we hit a null leaf or the input value matches the value of a node
	// if the values match then the count gets incremented and we return from the function
	while (currentNode != nullptr)
	{
		previousNode = currentNode;
		if (input.compare(currentNode->value) < 0)
		{
			currentNode = currentNode->leftChild;
			keyComparisons++;
		}
		else if (input.compare(currentNode->value) > 0)
		{
			currentNode = currentNode->rightChild;
			keyComparisons++;
		}
			
		else
		{
			currentNode->count = currentNode->count++;
			return;
		}
	}

	// since we are at a null leaf we need to create a new node and set its value to the input
	currentNode = new Node();
	currentNode->value = input;

	// if the previous node is null then it must be the root so we need to set the root node
	// to the current node
	// else we need to set the current nodes parent and the parents child to the current node
	if (previousNode == nullptr)
		Root = currentNode;
	else
	{
		currentNode->parent = previousNode;
		nodePointerChanges++;

		if (currentNode->value.compare(previousNode->value) < 0)
			previousNode->leftChild = currentNode;
		else
			previousNode->rightChild = currentNode;
		keyComparisons++;
		nodePointerChanges++;
	}	
}

// prints the value and count of a node
void BST::printNodeInfo(Node* node)
{
	std::cout << node->value << " " << node->count << std::endl;
}

void BST::list()
{
	// if the root is null then the set is empty and we need to output a blank line and return
	if (Root == nullptr)
	{
		std::cout << std::endl;
		return;
	}
	// if the set is not empty traverse the list in order and output the node values and counts
	traverseAndPrint(Root);
}

// recurse through the tree printing the nodes in order
void BST::traverseAndPrint(Node* node)
{
	// if the nodes left child is not null keep going left and then print going back up until you can 
	// go right. then keep trying to go left again and repeat
	if (node->leftChild != nullptr)
		traverseAndPrint(node->leftChild);

	printNodeInfo(node);

	if (node->rightChild != nullptr)
		traverseAndPrint(node->rightChild);
}

// clears previous stats and re-calculates them by recursively traversing
void BST::setStats()
{
	treeHeight = 0;
	uniqueItemsInTree = 0;
	itemsInTree = 0;
	if (Root == nullptr)
		return;

	// if the set is not empty traverse the list in order and output the node values and counts
	traverseSetStats(Root, treeHeight);
}

// recurse through the tree setting important statistics
void BST::traverseSetStats(Node* node, int nodeHeight)
{
	uniqueItemsInTree++;
	itemsInTree += node->count;
	if (node->leftChild != nullptr)	
		traverseSetStats(node->leftChild, nodeHeight + 1);

	if (node->rightChild != nullptr)
		traverseSetStats(node->rightChild, nodeHeight + 1);
	
	if (nodeHeight > treeHeight)
		treeHeight = nodeHeight;
}

// sets the time it takes for the insert process
void BST::setInsertTime(std::chrono::duration<double> insertTime)
{
	totalInsertTime = insertTime;
}

// sets the statistics and then prints out the needed metrics
void BST::printStats()
{
	setStats();
	std::cout
		<< "<----------BST Statistics---------->" << std::endl
		<< "Tree height : " << treeHeight << std::endl
		<< "Total items : " << itemsInTree << std::endl
		<< "Unique items : " << uniqueItemsInTree << std::endl
		<< "Key comparisons : " << keyComparisons << std::endl
		<< "Node pointer changes : " << nodePointerChanges << std::endl;
	printf("Insert time : %.3f s\n", totalInsertTime.count());
	std::cout
		<< "<---------------------------------->" << std::endl << std::endl;
}