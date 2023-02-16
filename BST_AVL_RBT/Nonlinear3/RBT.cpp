
#include "stdafx.h"
#include "RBT.h"
#include <string>
#include <iostream>

// when constructing a new tree we need to create our nil node that is the leaf
// to all bottom nodes
RBT::RBT()
{
	nil = new Node();
	nil->color = false; // red = true black = false
	nil->count = 0;
	nil->leftChild = Root;
	nil->rightChild = Root;
}


RBT::~RBT()
{
}

// this function takes a word and adds it to the tree
// once the node is in the tree we make sure that we have not broken any of the
// rules of a red black tree
void RBT::insert(std::string input)
{
	// if the the root is null then we have no nodes in the tree except for nil
	// so we need to make a new node and set it as the root
	if (Root == nullptr)
	{
		Root = new Node();
		Root->value = input;
		Root->leftChild = nil;
		Root->rightChild = nil;
		Root->parent = nil;
		Root->color = false;
		nodePointerChanges += 3;
		return;
	}

	Node * currentNode = Root; // this node will traverse down the tree to find the insertion point of the next node
	Node * previousNode = nil; // this node lags behind the current node

	// traverse down the tree until the current node reaches the leaf
	while (currentNode != nil)
	{
		previousNode = currentNode;

		// if the input is less than the current node value go left
		if (input.compare(currentNode->value) < 0)
		{
			currentNode = currentNode->leftChild;
			keyComparisons++;
		}
		// else if the input is greater than the current node go right
		else if (input.compare(currentNode->value) > 0)
		{
			currentNode = currentNode->rightChild;
			keyComparisons++;
		}
		// else the current node value equals that of the input so the only thing left to do is
		// increment the count of that node and then return
		else
		{
			currentNode->count++;
			return;
		}
	}

	// if we have reached this point then the current node is nil and the previous node is the parent for the
	// node we need to insert
	Node * nodeToInsert = new Node();
	nodeToInsert->value = input;
	nodeToInsert->parent = previousNode;
	nodePointerChanges++;

	// now we figure out which child the node to insert is supposed to be by comparing the input value to that of the
	// previous nodes value
	if (input.compare(previousNode->value) < 0)
		previousNode->leftChild = nodeToInsert;
	else
		previousNode->rightChild = nodeToInsert;

	keyComparisons++;
	nodePointerChanges++;
	
	// then we make sure the new node we inserted has both its children point to the leaf of the tree
	nodeToInsert->leftChild = nil;
	nodeToInsert->rightChild = nil;
	nodePointerChanges += 2;

	// now that the insert is complete we need to make sure we have not violated any of the RB tree's properties
	fixup(nodeToInsert);
}

// checks that we did not violate properites of the RB tree
// if we have this will fix the tree 
void RBT::fixup(Node * node)
{
	// while the nodes parent is red
	while (node->parent->color)
	{
		// if the current nodes parent is equal to the current nodes grandparents left child then
		// the current nodes uncle is the current nodes grandparents right child
		if (node->parent == node->parent->parent->leftChild)
		{
			Node * uncle = node->parent->parent->rightChild;

			// if the uncle is red we have violated the no two reds in a row rule and fix it by recoloring
			// the parent, uncle, and grandparent
			if (uncle->color)
			{
				node->parent->color = false;
				uncle->color = false;
				node->parent->parent->color = true;
				recolorings += 3;

				// now that we have solved the issue at this level we move up to the next location with a
				// potential issue
				node = node->parent->parent;
			}
			else
			{
				// if the current node is the right child of its parent then the current nodes sibling is black
				// this is solved by performing a left rotate on the current nodes parent
				if (node == node->parent->rightChild)
				{
					node = node->parent;
					leftRotate(node);
				}
				// now that the current node is a left child we can recolor the current nodes parents and perform a 
				// right rotation on the current nodes grandparent to finish fixing 
				node->parent->color = false;
				node->parent->parent->color = true;
				recolorings += 2;

				rightRotate(node->parent->parent);
			}
		}
		else
		{
			// this next section is symmetric to the section above
			// for clarification please refer to the previous section
			Node * uncle = node->parent->parent->leftChild;
			if (uncle->color)
			{
				node->parent->color = false;
				uncle->color = false;
				node->parent->parent->color = true;
				recolorings += 3;

				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->leftChild)
				{
					node = node->parent;
					rightRotate(node);
				}
				node->parent->color = false;
				node->parent->parent->color = true;
				recolorings += 2;
				leftRotate(node->parent->parent);
			}
		}
	}

	// now the only thing left to do is set the root to black just in case it was changed earlier
	Root->color = false;
	recolorings++;
}

// this function performs a left rotate on the node passed in
void RBT::leftRotate(Node * node)
{
	// the temp node is node’s right (non-nil) child
	Node * tempNode = node->rightChild;
	
	//Turn temp node’s left subtree into node’s right subtree
	node->rightChild = tempNode->leftChild;
	nodePointerChanges++;

	if (tempNode != nil)
	{
		tempNode->leftChild->parent = node;
		nodePointerChanges++;
	}
	// Link node’s parent to temp node
	tempNode->parent = node->parent;
	nodePointerChanges++;

	// If node has no parent, node was the root so temp node becomes the new root
	if (node->parent == nil)
		Root = tempNode;
	// Otherwise (node has a parent), the spot node used to occupy now gets taken by temp node
	else
	{
		if (node == node->parent->leftChild)
			node->parent->leftChild = tempNode;
		else
			node->parent->rightChild = tempNode;

		nodePointerChanges++;
	}
	
	// put node on temp node’s left, which makes node’s parent be temp node
	tempNode->leftChild = node;
	node->parent = tempNode;

	nodePointerChanges += 2;
}

// this function is symmetric to the one above
// if clarification is needed please refer to it
void RBT::rightRotate(Node * node)
{
	Node * tempNode = node->leftChild;
	node->leftChild = tempNode->rightChild;
	nodePointerChanges++;

	if (tempNode != nil)
	{
		tempNode->rightChild->parent = node;
		nodePointerChanges++;
	}

	tempNode->parent = node->parent;
	nodePointerChanges++;

	if (node->parent == nil)
		Root = tempNode;
	else
	{
		if (node == node->parent->rightChild)
			node->parent->rightChild = tempNode;
		else
			node->parent->leftChild = tempNode;

		nodePointerChanges++;
	}

	tempNode->rightChild = node;
	node->parent = tempNode;

	nodePointerChanges += 2;
}

// prints the value and count of a node
void RBT::printNodeInfo(Node* node)
{
	std::cout << node->value << " " << node->count << std::endl;
}

// clears previous stats and re-calculates them by recursively traversing
void RBT::setStats()
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
void RBT::traverseSetStats(Node* node, int nodeDepth)
{
	// when this function is called we are at a node and a node is a unique item and its count should
	// be added to the running sum of the items in the tree
	uniqueItemsInTree++;
	itemsInTree += node->count;

	// if you can go left, then go left
	if (node->leftChild != nil)
		traverseSetStats(node->leftChild, nodeDepth + 1);

	// if you can go right, go right
	if (node->rightChild != nil)
		traverseSetStats(node->rightChild, nodeDepth + 1);

	// now we are at a leaf so we so if our current depth is greater than the tree height we replace the 
	// current height with the depth. this is because the greatest depth is equal to the height of the tree
	if (nodeDepth > treeHeight)
		treeHeight = nodeDepth;
}

// prints all nodes in order
void RBT::list()
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
void RBT::traverseAndPrint(Node* node)
{
	// if the nodes left child is not null keep going left and then print going back up until you can 
	// go right. then keep trying to go left again and repeat
	if (node->leftChild != nil)
		traverseAndPrint(node->leftChild);

	printNodeInfo(node);

	if (node->rightChild != nil)
		traverseAndPrint(node->rightChild);
}

// sets the time it takes for the insert process
void RBT::setInsertTime(std::chrono::duration<double> insertTime)
{
	totalInsertTime = insertTime;
}

// sets the statistics and then prints out the needed metrics
void RBT::printStats()
{
	setStats();

	std::cout
		<< "<----------RBT Statistics---------->" << std::endl
		<< "Tree height : " << treeHeight << std::endl
		<< "Total items : " << itemsInTree << std::endl
		<< "Unique items : " << uniqueItemsInTree << std::endl
		<< "Key comparisons : " << keyComparisons << std::endl
		<< "Node pointer changes : " << nodePointerChanges << std::endl
		<< "Node recolorings : " << recolorings << std::endl;
	printf("Insert time : %.3f s\n", totalInsertTime.count());
	std::cout
		<< "<---------------------------------->" << std::endl << std::endl;
}