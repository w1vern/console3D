#include <structures.h>
#include <input.h>

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
        if (GetAsyncKeyState('W') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_forward);
        }
        if (GetAsyncKeyState('A') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_left);
        }
        if (GetAsyncKeyState('S') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_back);
        }
        if (GetAsyncKeyState('D') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_right);
        }
        if (GetAsyncKeyState('Q') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_up);
        }
        if (GetAsyncKeyState('E') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::step_down);
        }
        if (GetAsyncKeyState('R') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::roll_minus);
        }
        if (GetAsyncKeyState('F') & 0x8000)
        {
            Global_params::current_moves.insert(Moveset::roll_plus);
        }
        Global_params::input_mutex.unlock();
    }
}