
#include "stdafx.h"
#include "AVL.h"
#include <string>
#include <iostream>

AVL::AVL()
{
}


AVL::~AVL()
{
}

// inserts a node into the tree and then checks balance factors to decide if a rotation is needed
// to maintain the balance of the tree
void AVL::insert(std::string input)
{
	// If the root is null we only need to do a dumb insert and nothing else
	if (Root == nullptr)
	{
		Root = new Node();
		Root->value = input;
		return;
	}

	Node * p; // our current location on the tree
	Node * q; // lags behind current node
	Node * a; // the last parent of current node that had a balance factor of +- 1
	Node * f; // lags behind recent nonzero

	p = a = Root;
	q = f = nullptr;

	// while the current node is not at the child of a leaf or a duplicate value keep traversing through the tree
	// keeping track of the most recent nonzero balance factor node
	while (p != nullptr)
	{
		if (input.compare(p->value) == 0)
		{
			keyComparisons++;
			p->count++;
			return;
		}

		if (p->balanceFactor != 0)
		{
			a = p;
			f = q;
		}

		q = p;
		p = (input.compare(p->value) < 0) ? p->leftChild : p->rightChild;
	}

	// Now the previous node is a leaf and the current node is the child of that leaf so we need
	// to create a new node to insert

	// node to insert is y
	Node * y = new Node();
	y->value = input;
	Node * b; // this is the child of the most recent nonzero balance factor in the direction of the potential rotation
	int displacement; // this is used to correct balance factors later

	// we need to know if the new node we are inserting is the left or the right child of the previous node so we
	// can have the correct child pointer point to the new node
	if (input.compare(q->value) < 0)
		q->leftChild = y;
	else
		q->rightChild = y;
	keyComparisons++;

	// if the value of the node we just inserted is less than that of the most recent nonzero balance factor node
	// then we went left so the pivot needs to be the left child else its the right child
	// the displacement is set based on the direction taken

	if (input.compare(a->value) > 0)
	{
		p = a->rightChild;
		b = p;
		displacement = -1;
	}
	else
	{
		p = a->leftChild;
		b = p;
		displacement = 1;
	}
	keyComparisons++;

	// then we traverse down from the most recent nonzero balance factor node to the node we inserted setting balance factors
	// on the way down
	while (p != y)
	{
		if (input.compare(p->value) > 0)
		{
			p->balanceFactor = -1;
			p = p->rightChild;
		}
		else
		{
			p->balanceFactor = 1;
			p = p->leftChild;
		}
		keyComparisons++;
		balanceFactorChanges++;
	}

	// if the tree was completely balanced recentNonZero will be at the root of the tree and our insert will
	// have pushed the tree slightly out of balance
	if (0 == a->balanceFactor)
	{
		a->balanceFactor = displacement;
		balanceFactorChanges++;
		return;
	}

	// if the most reent nonzero balance factor is opposite the displacement then we put the tree back in balance 
	if (a->balanceFactor == -displacement)
	{
		a->balanceFactor = 0;
		balanceFactorChanges++;
		return;
	}

	// At this point we have thrown the tree out of balance by inserting
	// The displacement tells us the first direction of the rotation and the most recent non-zero balance factor node (b) 
	// tells us the second direction
	if (displacement == 1)
	{
		if (b->balanceFactor == 1) //LL
		{
			// set a's left child to b's right and b's right to a
			// then fix balance factors
			a->leftChild = b->rightChild;
			b->rightChild = a;
			nodePointerChanges += 2;
			a->balanceFactor = b->balanceFactor = 0;
			balanceFactorChanges += 2;
		}
		else //LR
		{
			// create a new node c that is b's right child
			Node * c = b->rightChild;

			// for this rotation the order of a, b, and c are b < c < a
			// so c gets pulled up to the middle and sets its children to b (left) and a (right)
			// this cuts off c's children though so prior to this c's left needs to be attached as b's right
			// and c's right is attached as a's left

			// then attach c's children to a and b
			a->leftChild = c->rightChild;
			b->rightChild = c->leftChild;

			// then set c's children to b and a
			c->leftChild = b;
			c->rightChild = a;
			nodePointerChanges += 4;

			// then we set a and b's balance factors to 0 and correct one of them depending on what
			// c's balance factor
			a->balanceFactor = b->balanceFactor = 0;
			balanceFactorChanges += 2;
			
			if (c->balanceFactor == 1)
				a->balanceFactor = -1;
			else if (c->balanceFactor == -1)
				b->balanceFactor = 1;
			balanceFactorChanges++;
			
			// then c's balance factor is fixed and set to 0
			c->balanceFactor = 0;
			balanceFactorChanges++;
			b = c; // this is for reattaching the subtree to the proper parent
		}
	}
	else // again the next parts are symmetric so almost all the operations are just flipped
	{
		if (b->balanceFactor == -1) //RR
		{
			a->rightChild = b->leftChild;
			b->leftChild = a;
			nodePointerChanges += 2;

			a->balanceFactor = b->balanceFactor = 0;
			balanceFactorChanges += 2;
		}
		else //RL
		{
			Node * c = b->leftChild;
			a->rightChild = c->leftChild;
			b->leftChild = c->rightChild;
			c->rightChild = b;
			c->leftChild = a;
			nodePointerChanges += 4;

			a->balanceFactor = b->balanceFactor = 0;
			balanceFactorChanges += 2;

			if (c->balanceFactor == 1)
				b->balanceFactor = -1;
			else if (c->balanceFactor == -1)
				a->balanceFactor = 1;
			balanceFactorChanges++;

			c->balanceFactor = 0;
			balanceFactorChanges++;
			b = c;
		}
	}

	// if the parent of the recent non zero balance factor node was null then there were no nodes with a nonzero balance
	// or the only one was the root. in either case the recent non zero was the root so whatever is in position b needs to 
	// be set as the root
	if (f == nullptr)
	{
		Root = b;
		return;
	}
		
	// if the left child of the recent nonzero BF parent node is the recent nonzero node we need to attach the new
	// root of the subtree (b) in a's place
	if (f->leftChild == a)
		f->leftChild = b;
	else // otherwise its the right child that needs reattached
		f->rightChild = b;
	nodePointerChanges++;
}

// list all nodes in order with counts
void AVL::list()
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
void AVL::traverseAndPrint(Node * node)
{
	// if the nodes left child is not null keep going left and then print going back up until you can 
	// go right. then keep trying to go left again and repeat
	if (node->leftChild != nullptr)
		traverseAndPrint(node->leftChild);

	printNodeInfo(node);

	if (node->rightChild != nullptr)
		traverseAndPrint(node->rightChild);
}

void AVL::printNodeInfo(Node * node)
{
	std::cout << node->value << " " << node->count << std::endl;
}

// clears previous stats and re-calculates them by recursively traversing
void AVL::setStats()
{
	treeHeight = 0;
	uniqueItemsInTree = 0;
	itemsInTree = 0;
	if (Root == nullptr)
		return;

	// if the set is not empty traverse the list in order and output the node values and counts
	traverseSetStats(Root, treeHeight);
}

// recurse through the tree setting important statistics (RBT.cpp has a more detailed explanation of the same code)
void AVL::traverseSetStats(Node* node, int nodeHeight)
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
void AVL::setInsertTime(std::chrono::duration<double> insertTime)
{
	totalInsertTime = insertTime;
}

// sets the statistics and then prints out the needed metrics
void AVL::printStats()
{
	setStats();
	std::cout
		<< "<----------AVL Statistics---------->" << std::endl
		<< "Tree height : " << treeHeight << std::endl
		<< "Total items : " << itemsInTree << std::endl
		<< "Unique items : " << uniqueItemsInTree << std::endl
		<< "Key comparisons : " << keyComparisons << std::endl
		<< "Node pointer changes : " << nodePointerChanges << std::endl
		<< "Balance factor changes : " << balanceFactorChanges << std::endl;
	printf("Insert time : %.3f s\n", totalInsertTime.count());
	std::cout
		<< "<---------------------------------->" << std::endl << std::endl;
}