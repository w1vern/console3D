#pragma once
#include <vector>
#include <set>
#include <include/vector3.h>

enum Moveset {
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
    Location &get_instance();
    void add_triangle(const Triangle &triangle);

private:
    Location();
    std::vector<Triangle> polygons;
};

class Ray
{
public:
    void compute(const Location &location);
    mth::Vector3 get_touch_coord();
    float get_bright_level();
    bool is_touch();

private:
    mth::Vector3 direction;
    std::vector<mth::Vector3> points_of_touch;
    float bright_level;
    mth::Vector3 point_of_touch;
    bool touch;
};

class Camera
{
public:
    Camera &get_instance();
    void compute_rays();
private:
    Camera();
    std::vector<Ray> rays;
    float yaw;
    float pitch;
    float roll;
};

class Console
{
public:
    static void reinterpret_size();
    static std::uint8_t width;
    static std::uint8_t height;
    static std::uint16_t count_of_pixels;

    static std::set<Moveset> current_moves;

private:
    Console();
};