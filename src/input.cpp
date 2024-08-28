
#include "input.h"
#include "structures.h"

#include <Windows.h>

void update_input_state()
{
    while (true)
    {
        Sleep(10);
        Global_params::input_mutex.lock();
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::yaw_minus);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::yaw_plus);
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::pitch_plus);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::pitch_minus);
        }
        if (GetAsyncKeyState('w') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_forward);
        }
        if (GetAsyncKeyState('a') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_left);
        }
        if (GetAsyncKeyState('s') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_back);
        }
        if (GetAsyncKeyState('d') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_right);
        }
        Global_params::input_mutex.unlock();
    }
}