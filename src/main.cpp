#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <Windows.h>
#include <conio.h>

#include <include/clean_console.h>
#include <include/math_cpp.h>

#include "structures.h"

int main() 
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	clean_console(console_handle, true);
    Sleep(3000);

    Console::reinterpret_size();



    return 0;
}