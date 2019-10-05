#pragma once
#include "point.h"

template <typename T>
struct camera
{
    camera(point_3<T> origin,
           point_3<T> direction,
           point_3<T> up,
           T screen_width,
           T screen_height)
        : origin(origin)
        , direction(direction)
        , up(up)
        , screen_width(screen_width)
        , screen_height(screen_height)
    {}

    point_3<T> origin;
    point_3<T> direction;
    point_3<T> up;
    T screen_width;
    T screen_height;
};
