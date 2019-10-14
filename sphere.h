#pragma once

#include "point.h"

template <typename T>
struct sphere
{
    sphere(point_3<T> center, T radius, point_3<T> color)
        : center(center)
        , radius(radius)
        , color(color)
    {}

    point_3<T> center;
    T radius;
    point_3<T> color;
};
