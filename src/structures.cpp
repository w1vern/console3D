#include "structures.h"

#include <Windows.h>

//

void Console::reinterpret_size()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	Console::width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	Console::height = csbi.srWindow.Bottom - csbi.srWindow.Top;
	Console::count_of_pixels = Console::width * Console::height;
}

//



//
