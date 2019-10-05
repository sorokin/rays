#pragma once

#include <cmath>

template <typename T>
struct point_3
{
    point_3() = default;

    point_3(T x, T y, T z)
        : x(x)
        , y(y)
        , z(z)
    {}

    T x;
    T y;
    T z;
};

template <typename T>
point_3<T> operator+(point_3<T> a, point_3<T> b)
{
    return point_3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T>
point_3<T> operator-(point_3<T> a, point_3<T> b)
{
    return point_3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T>
point_3<T> operator*(T a, point_3<T> b)
{
    return point_3<T>(a * b.x, a * b.y, a * b.z);
}

template <typename T>
point_3<T> operator*(point_3<T> a, T b)
{
    return point_3<T>(a.x * b, a.y * b, a.z * b);
}

template <typename T>
point_3<T> operator/(point_3<T> a, T b)
{
    return point_3<T>(a.x / b, a.y / b, a.z / b);
}

template <typename T>
T dot(point_3<T> a, point_3<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
T norm_sqr(point_3<T> a)
{
    return dot(a, a);
}

template <typename T>
T norm(point_3<T> a)
{
    return std::sqrt(norm_sqr(a));
}

template <typename T>
point_3<T> normalized(point_3<T> a)
{
    T scale_factor = T(1.) / norm(a);
    return a * scale_factor;
}

template <typename T>
point_3<T> cross(point_3<T> a, point_3<T> b)
{
    return point_3<T>(a.y * b.z - b.y * a.z,
                      a.z * b.x - b.z * a.x,
                      a.x * b.y - b.x * a.y);
}
