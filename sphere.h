#pragma once

#include "point.h"

template <typename T>
struct sphere
{
    sphere(point_3<T> center, T radius)
        : center(center)
        , radius(radius)
    {}

    point_3<T> center;
    T radius;
};
