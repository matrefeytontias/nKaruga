#include "menu/Menu.hpp"

#include "utils.hpp"
#include "level/Level.hpp"
#include "handlers/SoundHandler.hpp"
#include "helpers/Constants.hpp"
#include "n2DLib/n2DLib.h"
#include "n2DLib/n2DLib_math.h"

#define VSPACE 9

const static unsigned short image_cursor[] = { 5, 8, 1,
0, 0, 1, 1, 1,
1, 0, 0, 1, 1,
1, 1, 0, 0, 1,
1, 1, 1, 0, 0,
1, 1, 1, 0, 0,
1, 1, 0, 0, 1,
1, 0, 0, 1, 1,
0, 0, 1, 1, 1
};

//
// MenuItem
//

MenuItem::MenuItem(const char* text) : text(text), textWidth(stringWidth(text)) { }

//
// BoolMenuItem
//

unsigned int BoolMenuItem::getMaxOptionTextWidth() const
{
	return max(stringWidth(yesText), stringWidth(noText));
}

//
// IntMenuItem
//

unsigned int IntMenuItem::getMaxOptionTextWidth() const
{
	int maxWidth = 0;
	for (int i = 0; i < maxValue; i++)
		maxWidth = max(maxWidth, stringWidth(options[i]));
	return maxWidth;
}

//
// Menu
//

Menu::Menu(int numItems, MenuItem* items[]) : numItems(numItems), width(0), height(numItems * VSPACE + 5 * 2), items(items), cursorPosition(0),
pressed()
{
	for (int i = 0; i < numItems; i++)
		width = max(width, items[i]->getMaxOptionTextWidth() + items[i]->getTextWidth());
	width += 8 * 2;
	pressed[0] = pressed[1] = pressed[2] = false;
}

bool Menu::run(KeyEvent k)
{
	fillRect((320 - width) / 2, (240 - height) / 2, width, height, 0);
	fillRect((320 - width) / 2 + 1, (240 - height) / 2 + 1, width - 2, height - 2, 0xffff);

	int cursorX = 0, cursorY = 0;

	// Draw the options
	for (int i = 0; i < numItems; i++)
	{
		MenuItem* item = items[i];
		const char* optionText = item->getOptionText();
		int x = (320 - item->getTextWidth() - (optionText != nullptr ? stringWidth(optionText) : 0)) / 2;
		int y = 120 - (numItems / 2 - i) * VSPACE;
		if (i == cursorPosition)
		{
			cursorX = x - 7;
			cursorY = y;
		}

		drawString(&x, &y, 0, item->text, 0xffff, 0);
		if(item->getOptionText())
			drawString(&x, &y, 0, item->getOptionText(), 0xffff, 0);
	}

	drawSprite(image_cursor, cursorX, cursorY, 0, 0);

	// Process input
	if (KDOWN(k))
	{
		if (!pressed[0] && cursorPosition < numItems - 1)
		{
			GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_CURSOR));
			pressed[0] = true;
			cursorPosition++;
		}
	}
	else
		pressed[0] = false;
		
	if (KUP(k))
	{
		if (!pressed[1] && cursorPosition > 0)
		{
			GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_CURSOR));
			pressed[1] = true;
			cursorPosition--;
		}
	}
	else
		pressed[1] = false;

	if (KFIRE(k))
	{
		if (!pressed[2])
		{
			pressed[2] = true;
			items[cursorPosition]->select();
		}
	}
	else
		pressed[2] = false;

	if (KPOLARITY(k))
	{
		GS->soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_BACK));
		return true;
	}

	return false;
}
