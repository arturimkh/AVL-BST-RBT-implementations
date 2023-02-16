#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "Control.h"
#include "BST.h"
#include "AVL.h"
#include "RBT.h"

using namespace std;

int main()
{
	// The following 3 functions read the input file to build the respective trees
	void insertBST(char delimiters[11], string inputFilePath, BST & tree, Control control);
	void insertAVL(char delimiters[11], string inputFilePath, AVL & tree, Control control);
	void insertRBT(char delimiters[11], string inputFilePath, RBT & tree, Control control);

	// This is the file that will be read from to build the trees
	string inputFilePath = "C:\\Users\\Артур\\Desktop\\Что-то\\Новый текстовый документ.txt";

	// set of delimiters for reading in the file
	char delimiters[11] = { 9 , 10 , 13 , 32 , '.' , ',' , '!' , ';' , ':' , '(' , ')' };

	// Control is used to establish a base line read time so we are later able to know the exact insertion time
	Control control;

	// sets the time it takes to run the insert functions from above without actually inserting
	control.baseLineFileRead(delimiters, inputFilePath); 


	RBT rbt; // instantiate each of the trees
	AVL avl;
	BST bst;
	
	// read the file one word at a time and insert that word into the respective tree
	insertBST(delimiters, inputFilePath, bst, control);
	insertAVL(delimiters, inputFilePath, avl, control);
	insertRBT(delimiters, inputFilePath, rbt, control);
	
	// print the statistics collected from the insert with some other calculated ones for each of the trees
	bst.printStats();
	avl.printStats();
	rbt.printStats();

	// then we wait for the user to finish viewing the data
	cout << "Processing finished. Press ENTER to exit" << endl;
	char waitChar;
	cin.get(waitChar);
    return 0;
}

// each of the following insert function are exactly the same except for the type of tree used
// this function opens and reads the file one char at a time. if the char is a delimiter the current 
// word is sent to the tree's insert function which will create a node that is added to the tree
// if the file cannot be opened the user will be notified
void insertBST(char delimiters[11], string filePath, BST & tree, Control control)
{
	std::ifstream inputStream;
	inputStream.open(filePath, std::ios::binary); // binary flag is set to read the file one byte at a time

	// if we couldn't open the file, let the user know and return
	if (inputStream.fail())
	{
		std::cout << "Could not open file" << std::endl;
		return;
	}

	// this will keep track of the time it takes to insert all the words in the tree down to milli second accuracy 
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	std::string nextWord = "";

	bool isDelimiter = false;
	char nextChar;
	inputStream.get(nextChar);

	// keep getting bytes until we have reached the end of the file
	while (!inputStream.eof())
	{
		// loop through the delimiters to check if the character read is one of them
		for each (char delimiter in delimiters)
		{
			// if the character is a delimiter we check to see if the word is empty
			// if the word is not empty it is sent to the trees insert function
			if (nextChar == delimiter)
			{
				if (nextWord != "")
					tree.insert(nextWord);

				nextWord = ""; // then next word is reset
				isDelimiter = true;
			}
		}

		// if the character was not a delimiter we need to append it to next word
		if (!isDelimiter)
			nextWord.push_back((unsigned char)nextChar);

		isDelimiter = false; // reset is delimiter

		inputStream.get(nextChar); // try to read the next character
	}

	// now that the insert is finished we need to set the time it took to complete
	end = std::chrono::system_clock::now();
	tree.setInsertTime(end - start - control.getFileReadTime());
}

void insertAVL(char delimiters[11], string filePath, AVL & tree, Control control)
{
	std::ifstream inputStream;
	inputStream.open(filePath, std::ios::binary);

	if (inputStream.fail())
	{
		std::cout << "Could not open file" << std::endl;
		return;
	}

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	std::string nextWord = "";

	bool isDelimiter = false;
	char nextChar;
	inputStream.get(nextChar);

	while (!inputStream.eof())
	{
		for each (char delimiter in delimiters)
		{
			if (nextChar == delimiter)
			{
				if (nextWord != "")
					tree.insert(nextWord);

				nextWord = "";
				isDelimiter = true;
			}
		}

		if (!isDelimiter)
			nextWord.push_back((unsigned char)nextChar);

		isDelimiter = false;

		inputStream.get(nextChar);
	}

	end = std::chrono::system_clock::now();
	tree.setInsertTime(end - start - control.getFileReadTime());
}

void insertRBT(char delimiters[11], string filePath, RBT & tree, Control control)
{
	std::ifstream inputStream;
	inputStream.open(filePath, std::ios::binary);

	if (inputStream.fail())
	{
		std::cout << "Could not open file" << std::endl;
		return;
	}

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	std::string nextWord = "";

	bool isDelimiter = false;
	char nextChar;
	inputStream.get(nextChar);

	while (!inputStream.eof())
	{
		for each (char delimiter in delimiters)
		{
			if (nextChar == delimiter)
			{
				if (nextWord != "")	
					tree.insert(nextWord);
				
				nextWord = "";
				isDelimiter = true;
			}
		}

		if (!isDelimiter)
			nextWord.push_back((unsigned char)nextChar);

		isDelimiter = false;

		inputStream.get(nextChar);
	}

	end = std::chrono::system_clock::now();
	tree.setInsertTime(end - start - control.getFileReadTime());

}