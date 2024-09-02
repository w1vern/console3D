#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <thread>
#include <Windows.h>

#include <clean_console.h>
#include <math_cpp.h>

#include <structures.h>
#include <input.h>

void compute_some_rays(std::uint64_t index_of_first_ray, std::uint64_t index_of_last_ray, bool &ready_to_word, bool &thread_ready)
{
	while (true)
	{
		if (ready_to_word)
		{
			for (std::uint64_t i = index_of_first_ray; i < index_of_last_ray; ++i)
				Camera::rays[i].compute_collisions();
			thread_ready = true;
			ready_to_word = false;
		}
	}
}

int main()
{
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	clean_console(console_handle, true);
	Sleep(1000);

	Global_params::reinterpret_console_size();
	std::cout << Global_params::height << '\t' << Global_params::width;
	Camera::compute_rays();

	std::thread input_thread(update_input_state);

	clock_t start_time, finish_time;
	char *display_buffer = new char[Global_params::count_of_pixels + 1];
	for (std::uint32_t i = 0; i < Global_params::count_of_pixels; ++i)
		display_buffer[i] = ' ';
	display_buffer[Global_params::count_of_pixels] = '\0';
	bool game_cycle = true;

	std::uint32_t count_of_threads = std::thread::hardware_concurrency();
	std::uint32_t step_of_compute = Global_params::count_of_pixels / (count_of_threads - 1);
	std::vector<std::thread> threads;

	while (false)
		if (GetAsyncKeyState('W') & 0x8000)
		{
			printf("1");
			Global_params::current_moves.insert(Moveset::step_forward);
		}

	Location::add_light_source(Light_source(mth::Vector3(-10, 0, 0), 1));

	bool use_multiprocessing = true;
	bool load = false;

	if (!load)
	{
		Location::add_polygon(Triangle(mth::Vector3{5, -2, 2}, mth::Vector3{5, 2, 2}, mth::Vector3{5, 1, -2}));
		// Location::add_polygon(Triangle(mth::Vector3{10, -2, 2}, mth::Vector3{10, -2, -2}, mth::Vector3{10, 2, -2}));
		// Location::add_polygon(Triangle(mth::Vector3{20, 2, -2}, mth::Vector3{20, -2, -2}, mth::Vector3{20, -2, 2}));
		// Location::add_polygon(Triangle(mth::Vector3{20, 2, -2}, mth::Vector3{20, 2, 2}, mth::Vector3{20, -2, 2}));
	}
	if (load)
	{
		std::string input;
		float f1, f2, f3;
		int i1, i2, i3;

		std::ifstream in;
		in.open("resource/monkey_mega_trunc.obj");
		std::vector<mth::Vector3> vector3;

		for (; std::getline(in, input);)
		{
			if ((input[0] == 'v') && (input[1] == ' '))
			{
				sscanf_s(input.data(), "v %f %f %f", &f1, &f2, &f3);
				vector3.push_back(mth::Vector3(f1, f2, f3));
				continue;
			}
			if (input[0] == 'f')
			{
				int temp;
				sscanf_s(input.data(), "f %d//%d %d//%d %d//%d", &i1, &temp, &i2, &temp, &i3, &temp);
				Location::add_polygon(Triangle(vector3[i1 - 1] + mth::Vector3{2, 0, 0}, vector3[i2 - 1] + mth::Vector3{2, 0, 0}, vector3[i3 - 1] + mth::Vector3{2, 0, 0}));
			}
		}

		in.close();
	}

	bool *thread_ready = new bool[count_of_threads];
	bool *ready_to_work = new bool[count_of_threads];
	for (std::uint32_t i = 0; i < count_of_threads; ++i)
	{
		thread_ready[i] = false;
		ready_to_work[i] = false;
	}

	if (use_multiprocessing)
	{
		for (std::uint32_t i = 0; i < count_of_threads - 1; ++i)
		{
			threads.push_back(std::thread(compute_some_rays, i * step_of_compute, (i + 1) * step_of_compute, std::ref(ready_to_work[i]), std::ref(thread_ready[i])));
		}
		threads.push_back(std::thread(compute_some_rays, (count_of_threads - 1) * step_of_compute, Global_params::count_of_pixels, std::ref(ready_to_work[count_of_threads - 1]), std::ref(thread_ready[count_of_threads - 1])));
	}

	while (game_cycle)
	{
		start_time = clock();
		SetConsoleCursorPosition(console_handle, {0, 0});

		Location::compute_polygons();

		if (use_multiprocessing)
		{
			for (std::uint32_t i = 0; i < count_of_threads; ++i)
			{
				thread_ready[i] = false;
				ready_to_work[i] = true;
			}
			bool ready = false;
			while (!ready)
				for (std::uint32_t i = 0; i < count_of_threads; ++i)
				{
					ready = true;
					ready &= thread_ready[i];
				}
		}
		else
			for (std::uint16_t i = 0; i < Global_params::count_of_pixels; ++i)
				Camera::rays[i].compute_collisions();

		finish_time = clock();

		Camera::draw_polygons(display_buffer);
		printf("%s", display_buffer);
		SetConsoleCursorPosition(console_handle, {0, 0});
		printf("%d", 1000 / (finish_time - start_time));
	}

	return 0;
}