#pragma once

#include <vector>

#include "types.h"

// General purpose menu handler
struct MenuItem
{
	MenuItem(const char* text);
	virtual void select() = 0;
	int getTextWidth() const { return textWidth; }
	virtual const char* getOptionText() const = 0;
	virtual unsigned int getMaxOptionTextWidth() const = 0;

	const char* text;

protected:
	int textWidth;
};

struct BoolMenuItem : public MenuItem
{
	BoolMenuItem(const char* text, bool &flag, const char* yesText, const char* noText) : MenuItem(text), flag(flag), yesText(yesText), noText(noText) { }
	void select() override { flag = !flag; }
	const char* getOptionText() const override { return flag ? yesText : noText; }
	unsigned int getMaxOptionTextWidth() const override;

protected:
	bool& flag;
	const char* yesText;
	const char* noText;
};

struct IntMenuItem : public MenuItem
{
	IntMenuItem(const char* text, int& value, int maxValue, const char* const* options) : MenuItem(text), value(value), maxValue(maxValue), options(options) { }
	void select() override { value = (value + 1) % (maxValue + 1); }
	const char* getOptionText() const override { return options[value]; }
	unsigned int getMaxOptionTextWidth() const override;

protected:
	int& value;
	int maxValue;
	const char* const* options;
};

struct DeepMenuItem : public MenuItem
{
	DeepMenuItem(const char* text, bool& picked) : MenuItem(text), picked(picked) { }
	void select() override { picked = true; }
	const char* getOptionText() const override { return nullptr; }
	unsigned int getMaxOptionTextWidth() const override { return 0; }

protected:
	bool& picked;
};

class Menu
{
public:
	Menu(int numItems, MenuItem* items[]);
	// Returns true if the menu was backed out of.
	bool run(KeyEvent k);
private:
	int numItems;
	int width;
	int height;
	MenuItem** items;
	// Cursor position in terms of items.
	int cursorPosition;
	// Handle key repeat.
	// TODO : decide if it's better to let the backend do that.
	bool pressed[3];
};
