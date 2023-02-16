
#include "stdafx.h"
#include "Control.h"
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>


Control::Control()
{
}


Control::~Control()
{
}

// This function runs the same code as in main for the insert functions, however it does not
// insert the words read into any tree. It is used for metrics only and the functionality has
// already been described. If you need more information or are confused on how this works please
// refer to the bstInsert() function in main.cpp that goes over the function in more detail
void Control::baseLineFileRead(char delimiters[11], std::string filePath)
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
	fileReadTime = end - start;
}

// returns the time taken for the mock insert to run
std::chrono::duration<double> Control::getFileReadTime()
{
	return fileReadTime;
}

