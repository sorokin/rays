#pragma once
#include "ray.h"
#include "sphere.h"
#include <cmath>

template <typename T>
struct ray_sphere_intersection
{
    ray_sphere_intersection(ray<T> r, sphere<T> s)
    {
        // norm(r.origin + t * r.direction - s.center) == s.radius
        // norm_sqr(r.origin + t * r.direction - s.center) == sqr(s.radius)
        // norm_sqr(r.origin - s.center + t * r.direction) == sqr(s.radius)
        // dot(r.origin - s.center + t * r.direction,
        //     r.origin - s.center + t * r.direction) == sqr(s.radius)
        // dot(r.origin - s.center, r.origin - s.center) +
        //     2 * t * dot(r.origin - s.center, r.direction) +
        //     t^2 * dot(r.direction, r.direction) == sqr(s.radius)
        // norm_sqr(r.origin - s.center) +
        //     2 * t * dot(r.origin - s.center, r.direction) +
        //     t^2 * norm_sqr(r.direction) == sqr(s.radius)

        T a = norm_sqr(r.direction);
        T half_b = dot(r.origin - s.center, r.direction);
        T c = norm_sqr(r.origin - s.center) - s.radius * s.radius;

        T d = half_b * half_b - a * c;
        if (d < 0)
            return; // no line-sphere intersection

        T sqrt_d = std::sqrt(d);
        T t_1 = (-half_b - sqrt_d) / a;
        T t_2 = (-half_b + sqrt_d) / a;

        T t; // t of first intersection
        if (t_1 < 0)
        {
            if (t_2 < 0)
                return; // line-sphere has collision, but ray-sphere doesn't
            else
                t = t_2; // ray origin is inside the sphere
        }
        else
            t = t_1;

        present = true;
        point = r.origin + t * r.direction;
        normal = normalized(point - s.center);
    }

    bool present = false;
    point_3<T> point;
    point_3<T> normal;
};


