#include "../include/structures.h"

#include <Windows.h>
#include <cmath>

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

    double p = (lAB + lCA + lBC) / 2;

    S = sqrt(p * (p - lAB) * (p - lCA) * (p - lBC));
}

// Location

void Location::add_triangle(Triangle triangle)
{
    Location::origin_polygons.push_back(triangle);
    Location::old_polygons.push_back(triangle);
}

void Location::compute_polygons()
{
    double roll = 0, pitch = 0, yaw = 0;
    mth::Vector3 offset;
    if (!Global_params::current_moves.empty())
    {
        double move_roll = roll, move_pitch = pitch, move_yaw = yaw;
        bool is_move = false;
        if (Global_params::current_moves.count(step_forward))
        {
            Global_params::current_moves.erase(step_forward);
            bool is_move = true;
        }
        if (Global_params::current_moves.count(step_back))
        {
            Global_params::current_moves.erase(step_back);
            move_yaw += 180;
            bool is_move = true;
        }
        if (Global_params::current_moves.count(step_left))
        {
            Global_params::current_moves.erase(step_left);
            move_yaw -= 90;
            bool is_move = true;
        }
        if (Global_params::current_moves.count(step_right))
        {
            Global_params::current_moves.erase(step_right);
            move_yaw += 90;
            bool is_move = true;
        }
        if (Global_params::current_moves.count(step_up))
        {
            Global_params::current_moves.erase(step_up);
            move_pitch -= 90;
            bool is_move = true;
        }
        if (Global_params::current_moves.count(step_down))
        {
            Global_params::current_moves.erase(step_down);
            move_pitch += 90;
            bool is_move = true;
        }
        if (is_move)
        {
            mth::Vector3 move(mth::cos(move_pitch) * mth::cos(move_yaw), mth::sin(move_roll) * mth::sin(move_pitch) * mth::cos(move_yaw) + mth::sin(move_yaw) * mth::cos(move_roll), mth::sin(move_roll) * mth::sin(move_yaw) - mth::sin(move_pitch) * mth::cos(move_roll) * mth::cos(move_yaw));
            offset += move * Global_params::speed_of_moving;
        }
        if (!Global_params::current_moves.empty())
        {
            roll -= (Global_params::current_moves.count(roll_minus)) * Global_params::speed_of_rotate;
            roll += (Global_params::current_moves.count(roll_plus)) * Global_params::speed_of_rotate;
            pitch -= (Global_params::current_moves.count(pitch_minus)) * Global_params::speed_of_rotate;
            pitch += (Global_params::current_moves.count(pitch_plus)) * Global_params::speed_of_rotate;
            pitch -= (Global_params::current_moves.count(yaw_minus)) * Global_params::speed_of_rotate;
            pitch += (Global_params::current_moves.count(yaw_plus)) * Global_params::speed_of_rotate;
            Global_params::current_moves.clear();
        }
    }

    computed_polygons.clear();
    mth::Matrix pitch_rotate(3, 3, {1, 0, 0, 0, mth::cos(pitch), -mth::sin(pitch), 0, mth::sin(pitch), mth::cos(pitch)});
    mth::Matrix yaw_rotate(3, 3, {mth::cos(yaw), 0, mth::sin(yaw), 0, 1, 0, -mth::sin(yaw), 0, mth::cos(yaw)});
    mth::Matrix roll_rotate(3, 3, {mth::cos(roll), -mth::sin(roll), 0, mth::sin(roll), mth::cos(roll), 0, 0, 0, 1});
    mth::Matrix rotate = roll_rotate * yaw_rotate * pitch_rotate;
    for (Triangle origin : old_polygons)
    {
        mth::Matrix vertices[3];
        for (std::uint8_t i = 0; i < 3; ++i)
        {
            vertices[i] = mth::Matrix(1, 3, {origin.vertices[i].getX(), origin.vertices[i].getY(), origin.vertices[i].getZ()});
            vertices[i] *= rotate;
        }
        computed_polygons.push_back(Triangle(mth::Vector3(vertices[0].getEl(0, 0), vertices[0].getEl(1, 0), vertices[0].getEl(2, 0)) + offset, mth::Vector3(vertices[1].getEl(0, 0), vertices[1].getEl(1, 0), vertices[1].getEl(2, 0)) + offset, mth::Vector3(vertices[2].getEl(0, 0), vertices[2].getEl(1, 0), vertices[2].getEl(2, 0)) + offset));
    }
    old_polygons = computed_polygons;
}

std::vector<Triangle> Location::origin_polygons;
std::vector<Triangle> Location::old_polygons;
std::vector<Triangle> Location::computed_polygons;

// Ray

Ray::Ray(mth::Vector3 direction) : direction(direction) {}

void Ray::compute_collisions()
{
    bright_level = 0;
    std::vector<mth::Vector3> points_of_touch;
    std::vector<double> lengths;
    for (Triangle triangle : Location::computed_polygons)
    {
        if (abs(direction * triangle.n) < 0.001)
            continue;
        double z = -triangle.D / ((triangle.A * direction.getX() + triangle.B * direction.getY()) / direction.getZ() + triangle.C);
        double x = z * direction.getX() / direction.getZ();
        double y = z * direction.getY() / direction.getZ();

        mth::Vector3 tmp{x, y, z};
        if (tmp * direction < 0)
            continue;

        double a1 = (triangle.vertices[0] - tmp).length(), a2 = (triangle.vertices[1] - tmp).length(), a3 = triangle.lAB;
        double b1 = a2, b2 = (triangle.vertices[2] - tmp).length(), b3 = triangle.lBC;
        double c1 = b2, c2 = a1, c3 = triangle.lCA;

        double p1 = (a1 + a2 + a3) / 2;
        double p2 = (b1 + b2 + b3) / 2;
        double p3 = (c1 + c2 + c3) / 2;

        double s1 = sqrt(p1 * (p1 - a1) * (p1 - a2) * (p1 - a3));
        double s2 = sqrt(p2 * (p2 - b1) * (p2 - b2) * (p2 - b3));
        double s3 = sqrt(p3 * (p3 - c1) * (p3 - c2) * (p3 - c3));

        if (s1 + s2 + s3 - 0.01 > triangle.S)
            continue;

        points_of_touch.push_back(tmp);
        lengths.push_back(tmp.length());
    }
    if (!(lengths.size()))
    {
        bright_level = 0;
        return;
    }
    touch = true;
    std::uint16_t index_of_min = 0;
    double r = lengths[0];
    for (std::uint16_t i = 1; i < lengths.size(); ++i)
        if (r > lengths[i])
            index_of_min = i;
    point_of_touch = points_of_touch[index_of_min];

    bright_level = Global_params::gradient_size - 1 - pow(point_of_touch.length(), 2) * Global_params::attenuation;

    bright_level = (bright_level > 0) ? bright_level : 1;
}

std::uint8_t Ray::get_bright_level()
{
    return bright_level;
}

std::vector<Ray> Camera::rays;

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
            mth::Vector3 tmp_ray(mth::cos(horizontal_angle), mth::sin(horizontal_angle), mth::cos(vertical_angle));
            tmp_ray /= tmp_ray.length();
            Camera::rays.push_back(Ray(tmp_ray));
        }
    }
}

void Camera::draw_polygons(char* display_buffer)
{
    for (std::uint16_t i = 0;i<Global_params::count_of_pixels;++i)
        display_buffer[i] = Global_params::gradient[rays[i].get_bright_level()];
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

std::uint8_t Global_params::width;
std::uint8_t Global_params::height;
std::uint16_t Global_params::count_of_pixels;

std::set<Moveset> Global_params::current_moves;
std::mutex Global_params::input_mutex;

const char Global_params::gradient[] = " .:!/r(l1Z4H9W8$@";
const uint8_t Global_params::gradient_size = sizeof(Global_params::gradient);
const double Global_params::fov = 103;
const uint32_t Global_params::frame_rate = 120;
const uint32_t Global_params::frame_time = 1000 / frame_rate;
const double Global_params::attenuation = 0.1;
const double Global_params::speed_of_moving = 0.1;
const double Global_params::speed_of_rotate = 1.;

//
