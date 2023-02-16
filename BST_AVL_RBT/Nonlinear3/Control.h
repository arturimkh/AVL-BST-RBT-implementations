
// this class executes a trial insert run without inserting to any trees allowing us to get a better
// idea of the time it takes to insert into the trees

#pragma once
#include <string>
#include <chrono>

class Control
{
public:
	Control();
	~Control();
	void baseLineFileRead(char delimiters[11], std::string filePath);
	std::chrono::duration<double> getFileReadTime();
private:
	std::chrono::duration<double> fileReadTime;
};

