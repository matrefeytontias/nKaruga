#pragma once

#include <vector>

// General purpose menu handler
typedef struct
{
	char* name;
	void* value;
	char** labels;
	int labelsNb;
} MenuItem;

class Menu
{
public:
	Menu(int num, ...);
	~Menu();
	// Returns the index of the chosen option, or -1 if the menu was exitted
	int run();
private:
	std::vector<char*> options;
	std::vector<void*> values;
	std::vector<std::vector<char*>> labels;
	int num;
	int maxOptionWidth;
	int width;
	int height;
	// Returns 0 if the choice was a value modifier, else 1
	int apply(int choice);
};
