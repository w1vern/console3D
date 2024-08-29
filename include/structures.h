#pragma once
#include <vector>
#include <set>
#include <mutex>
#include <include/vector3.h>

enum Moveset
{
    step_forward,
    step_back,
    step_left,
    step_right,
    step_up,
    step_down,
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

    float A;
    float B;
    float C;
    float D;

    float aABC;
    float aACB;
    float aBAC;

    float lAB;
    float lCA;
    float lBC;

    float S;

    mth::Vector3 AB, CA, BC;

    mth::Vector3 nAB, nCA, nBC;

    mth::Vector3 mAB, mBC, mCA;

    mth::Vector3 vertices[3];
    mth::Vector3 n;
};

class Location
{
    friend class Ray;
public:
    static void add_triangle(Triangle triangle);
    static void compute_polygons();
private:
    Location();
    static std::vector<Triangle> origin_polygons;
    static std::vector<Triangle> computed_polygons;
    static std::vector<Triangle> old_polygons;
};

class Ray
{
public:
    Ray(mth::Vector3 direction);
    void compute_collisions();
    std::uint8_t get_bright_level();

private:
    mth::Vector3 direction;
    std::uint8_t bright_level;
    mth::Vector3 point_of_touch;
    bool touch;
};

class Camera
{
friend int main();
public:
    static void compute_rays();
    static void draw_polygons(char*);

private:
    Camera();
    static std::vector<Ray> rays;
};

class Global_params
{
public:
    static void reinterpret_console_size();
    static std::uint8_t width;
    static std::uint8_t height;
    static std::uint16_t count_of_pixels;

    static std::set<Moveset> current_moves;
    static std::mutex input_mutex;

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