#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <thread>
#include <Windows.h>
#include <conio.h>

#include <include/clean_console.h>
#include <include/math_cpp.h>

#include "structures.h"
#include "input.h"

int main()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    clean_console(console_handle, true);
    Sleep(3000);

    Global_params::reinterpret_console_size();
    
    std::thread input_thread(update_input_state);
    
    return 0;
}