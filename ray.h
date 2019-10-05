#pragma once

#include "point.h"

template <typename T>
struct ray
{
    ray(point_3<T> origin, point_3<T> direction)
        : origin(origin)
        , direction(direction)
    {}

    point_3<T> origin;
    point_3<T> direction;
};
