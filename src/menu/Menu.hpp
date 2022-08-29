#pragma once

#include <vector>

// General purpose menu handler
typedef struct
{
	const char *name;
	void *value;
	const char* const *labels;
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
	std::vector<const char*> options;
	std::vector<void*> values;
	std::vector<std::vector<const char*>> labels;
	int num;
	int maxOptionWidth;
	int width;
	int height;
	// Returns 0 if the choice was a value modifier, else 1
	int apply(int choice);
};
