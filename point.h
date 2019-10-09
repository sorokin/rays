#pragma once

#include <cmath>
#include <type_traits>

template <typename T>
struct point_3
{
    point_3() = default;

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    point_3(point_3<U> other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
    {}

    point_3(T x, T y, T z)
        : x(x)
        , y(y)
        , z(z)
    {}

    T x;
    T y;
    T z;
};

template <typename U, typename V>
auto operator+(point_3<U> a, point_3<V> b)
{
    return point_3(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename U, typename V>
auto operator-(point_3<U> a, point_3<V> b)
{
    return point_3(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename U, typename V>
auto operator*(U a, point_3<V> b)
{
    return point_3(a * b.x, a * b.y, a * b.z);
}

template <typename U, typename V>
auto operator*(point_3<U> a, V b)
{
    return point_3(a.x * b, a.y * b, a.z * b);
}

template <typename U, typename V>
auto operator/(point_3<U> a, V b)
{
    return point_3(a.x / b, a.y / b, a.z / b);
}

template <typename U, typename V>
auto dot(point_3<U> a, point_3<V> b)
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
    using std::sqrt;
    return sqrt(norm_sqr(a));
}

template <typename T>
inline point_3<T> normalized(point_3<T> a) __attribute__((always_inline));

template <typename T>
point_3<T> normalized(point_3<T> a)
{
    T scale_factor = T(1.) / norm(a);
    return a * scale_factor;
}

template <typename U, typename V>
auto cross(point_3<U> a, point_3<V> b)
{
    return point_3(a.y * b.z - b.y * a.z,
                   a.z * b.x - b.z * a.x,
                   a.x * b.y - b.x * a.y);
}
