#include "structures.h"

#include <Windows.h>

#include <include/math_cpp.h>

// Triangle

Triangle::Triangle(mth::Vector3 v1, mth::Vector3 v2, mth::Vector3 v3)
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;

    n = (vertices[0] - vertices[1]) ^ (vertices[0] - vertices[2]);
    n /= n.length();

    A = n.getX();
	B = n.getY();
	C = n.getZ();
	D = -(A * vertices[0].getX() + B * vertices[0].getY() + C * vertices[0].getZ());

    AB = vertices[1] - vertices[0];
    BC = vertices[1] - vertices[2];
    CA = vertices[2] - vertices[0];

    nAB = AB ^ n;
	nBC = BC ^ n;
	nCA = CA ^ n;

    mAB = (vertices[0] + vertices[1]) / 2;
	mBC = (vertices[2] + vertices[0]) / 2;
	mCA = (vertices[1] + vertices[2]) / 2;

    lAB = AB.length();
	lCA = CA.length();
	lBC = BC.length();

    float p = (lAB + lCA + lBC) / 2;

    S = sqrt(p * (p - lAB) * (p - lCA) * (p - lBC));
}

// Location

void Location::add_triangle(Triangle triangle)
{
    Location::polygons.push_back(triangle);
}

// Ray

Ray::Ray(mth::Vector3 direction) : direction(direction) {}

void Ray::compute_collisions()
{
}

// Camera

void Camera::compute_rays()
{
    Camera::rays.clear();
    for (std::uint8_t i = 0; i < Global_params::height; ++i)
    {
        double vertical_angle = Global_params::fov * ((double)i / Global_params::height - 0.5);
        for (std::uint8_t j = 0; j < Global_params::width; ++j)
        {
            double horizontal_angle = Global_params::fov * ((double)j / Global_params::width - 0.5);
            mth::Vector3 tmp_ray(mth::cos(horizontal_angle), mth::sin(horizontal_angle), cos(vertical_angle));
            tmp_ray /= tmp_ray.length();
            Camera::rays.push_back(Ray(tmp_ray));
        }
    }
}

// Global_params

void Global_params::reinterpret_console_size()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Global_params::width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    Global_params::height = csbi.srWindow.Bottom - csbi.srWindow.Top;
    Global_params::count_of_pixels = Global_params::width * Global_params::height;
}

const char gradient[] = " .:!/r(l1Z4H9W8$@";
const uint8_t gradient_size = sizeof(gradient);
const double fov = 103;
const uint32_t frame_rate = 120;
const uint32_t frame_time = 1000 / frame_rate;
const double attenuation = 0.1;
const double speed_of_moving = 0.1;
const double speed_of_rotate = 1.;

//
