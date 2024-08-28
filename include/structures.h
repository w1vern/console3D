#pragma once
#include <vector>
#include <set>
#include <include/vector3.h>

enum Moveset
{
    step_forward,
    step_back,
    step_left,
    step_right,
    yaw_plus,
    yaw_minus,
    pitch_plus,
    pitch_minus,
    roll_plus,
    roll_minus
};

class Triangle
{
public:
    Triangle(mth::Vector3 v1, mth::Vector3 v2, mth::Vector3 v3);

private:
    mth::Vector3 v1;
    mth::Vector3 v2;
    mth::Vector3 v3;
};

class Location
{
public:
    static void add_triangle(Triangle triangle);

private:
    Location();
    static std::vector<Triangle> polygons;
};

class Ray
{
    friend class Camera;

public:
    Ray(mth::Vector3 direction);
    void compute_collisions();
    mth::Vector3 get_touch_coord();
    double get_bright_level();
    bool is_touch();

private:
    mth::Vector3 direction;
    std::vector<mth::Vector3> points_of_touch;
    double bright_level;
    mth::Vector3 point_of_touch;
    bool touch;
};

class Camera
{
public:
    static void compute_rays();

private:
    Camera();
    static std::vector<Ray> rays;
};

class Global_params
{
public:
    static void reinterpret_size();
    static std::uint8_t width;
    static std::uint8_t height;
    static std::uint16_t count_of_pixels;

    static std::set<Moveset> current_moves;

    static const char gradient[];
    static const uint8_t gradient_size;
	static const double fov;
	static const uint32_t frame_rate;
	static const uint32_t frame_time;
	static const double attenuation;
	static const double speed_of_moving;
	static const double speed_of_rotate;

private:
    Global_params();
};