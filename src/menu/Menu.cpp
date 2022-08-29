#include "menu/Menu.hpp"

#include <varargs.h>

#include "utils.hpp"
#include "handlers/Level.hpp"
#include "helpers/Constants.hpp"
#include "helpers/math.hpp"
#include "n2DLib/n2DLib.hpp"

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

Menu::Menu(int _num, ...)
{
	va_list argstack;
	va_start(argstack, _num);
	num = _num;

	for (int k = 0; k < num; k++)
	{
		MenuItem item = va_arg(argstack, MenuItem);
		options.push_back(item.name);
		values.push_back(item.value);
		std::vector<char*> t;
		if (item.labels)
		{
			for (int j = 0; j < item.labelsNb; j++)
				t.push_back(item.labels[j]);
		}
		labels.push_back(t);
	}
	va_end(argstack);

	num = _num;
	height = num * VSPACE + 5 * 2;
	maxOptionWidth = 0;
	for (uint8_t i = 0; i < options.size(); i++)
	{
		int b = stringWidth(options[i] + 1);
		for (char* k : labels[i])
		{
			int c = b + stringWidth(k);
			maxOptionWidth = max(c, maxOptionWidth);
		}
	}
	width = maxOptionWidth + 8 * 2;
}

Menu::~Menu()
{
}

// Preserves the buffer
int Menu::run()
{
	bool pressed[3] = { false };
	std::vector<int> x = std::vector<int>(num), y = std::vector<int>(num);

	fillRect((320 - width) / 2, (240 - height) / 2, width, height, 0);

	int choice = 0;
	while (1)
	{
		fillRect((320 - width) / 2 + 1, (240 - height) / 2 + 1, width - 2, height - 2, 0xffff);
		// Draw the options
		for (int i = 0; i < num; i++)
		{
			y[i] = (240 - (num / 2 - i) * VSPACE * 2) / 2;
			int _x, _y;
			const char *v;
			switch (options[i][0])
			{
				// Normal
			case 'n':
				x[i] = (320 - stringWidth(options[i] + 1)) / 2;
				_x = x[i];
				_y = y[i];
				drawString(&_x, &_y, _x, options[i] + 1, 0xffff, 0);
				break;
				// Boolean
			case 'b':
				v = labels[i][*(bool*)values[i]];
				x[i] = (320 - stringWidth(options[i] + 1) - stringWidth(v)) / 2;
				_x = x[i];
				_y = y[i];
				drawString(&_x, &_y, x[i], options[i] + 1, 0xffff, 0);
				drawString(&_x, &_y, x[i], v, 0xffff, 0);
				break;
				// Integer
			case 'i':
				v = labels[i][*(int*)values[i]];
				x[i] = (320 - stringWidth(options[i] + 1) - stringWidth(v)) / 2;
				_x = x[i];
				_y = y[i];
				drawString(&_x, &_y, _x, options[i] + 1, 0xffff, 0);
				drawString(&_x, &_y, _x, v, 0xffff, 0);
				break;
			}
		}

		drawSprite(image_cursor, x[choice] - 7, y[choice], 0, 0);
		updateScreen();
		updateKeys();
		drawSprite(image_cursor, x[choice] - 7, y[choice], 1, 0xffff);

		// Let the user pick an option or fiddle with the values
		if (isKeyPressed(G_downKey))
		{
			if (!pressed[0] && choice < num - 1)
			{
				Level::soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_CURSOR));
				pressed[0] = true;
				choice++;
			}
		}
		else
			pressed[0] = false;
		
		if (isKeyPressed(G_upKey))
		{
			if (!pressed[1] && choice > 0)
			{
				Level::soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_CURSOR));
				pressed[1] = true;
				choice--;
			}
		}
		else
			pressed[1] = false;

		if (isKeyPressed(SDL_SCANCODE_RETURN))
		{
			if (!pressed[2])
			{
				pressed[2] = true;
				if (apply(choice))
				{
					Level::soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_SELECT));
					return choice;
				}
			}
		}
		else
			pressed[2] = false;

		if (isKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			Level::soundSystem->quickPlaySFX(LUTs::sound(LUTs::SoundId::MENU_BACK));
			return -1;
		}
	}
}

int Menu::apply(int choice)
{
	switch (options[choice][0])
	{
	case 'n':
		return 1;
	case 'b':
		*(bool*)(values[choice]) ^= 1;
		break;
	case 'i':
		int *v = (int*)(values[choice]);
		*v = (*v + 1) % labels[choice].size();
	}
	return 0;
}