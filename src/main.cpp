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
    Camera::compute_rays();

    std::thread input_thread(update_input_state);

    clock_t start_time, finish_time;
    char *display_buffer = new char[Global_params::count_of_pixels + 1];
    display_buffer[Global_params::count_of_pixels] = '\0';
    bool game_cycle = true;

    std::uint8_t count_of_threads = std::thread::hardware_concurrency() - 2;
    std::uint16_t step_of_compute = Global_params::count_of_pixels/count_of_threads;

    Location::add_triangle(Triangle(mth::Vector3{10, -2, 2}, mth::Vector3{10, 2, 2}, mth::Vector3{-1, -1, -1}));
    Location::add_triangle(Triangle(mth::Vector3{10, -2, 2}, mth::Vector3{10, 2, 2}, mth::Vector3{10, 2, -2}));
    Location::add_triangle(Triangle(mth::Vector3{20, 2, -2}, mth::Vector3{20, -2, -2}, mth::Vector3{20, -2, 2}));
    Location::add_triangle(Triangle(mth::Vector3{20, 2, -2}, mth::Vector3{20, 2, 2}, mth::Vector3{20, -2, 2}));

    while (game_cycle)
    {
        start_time = clock();
        SetConsoleCursorPosition(console_handle, {0, 0});

        Location::compute_polygons();

        for (std::uint8_t i = 0;i<Global_params::count_of_pixels;++i)
            Camera::rays[i].compute_collisions();

        Camera::draw_polygons(display_buffer);
        printf("%s", display_buffer);
    }

    return 0;
}