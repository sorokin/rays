#pragma once
#include "ray.h"
#include "sphere.h"
#include <cmath>

#include "simd.h"

template <typename T>
struct best_intersection
{
    T z;
    point_3<T> point;
    point_3<T> normal;
    point_3<T> color;
};

template <typename Tr, typename Ts>
inline void intersect_ray_sphere(ray<Tr> r, sphere<Ts> s, best_intersection<Tr>& best)
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

    //T a = norm_sqr(r.direction);
    Tr half_b = dot(r.origin - s.center, r.direction);
    Tr c = norm_sqr(r.origin - s.center) - s.radius * s.radius;

    Tr d = half_b * half_b - /*a * */c;
    auto d_is_non_negative = d >= 0;
    if (all_are_false(d_is_non_negative))
        return; // no line-sphere intersection
    d = d & d_is_non_negative;

    using std::sqrt;
    Tr sqrt_d = sqrt(d);
    Tr t_1 = (-half_b - sqrt_d) /*/ a*/;

    Tr t; // t of first intersection
    auto t_1_is_non_negative = t_1 >= 0;
    if (all_are_false(t_1_is_non_negative))
    {
        /*T t_2 = (-half_b + sqrt_d) / a;
        if (t_2 < 0)
            return; // line-sphere has collision, but ray-sphere doesn't
        else
            t = t_2; // ray origin is inside the sphere            
            */
        return;
    }
    else
        t = t_1;

    pmask present = d_is_non_negative & t_1_is_non_negative & (best.z > t);
    conditional_assign(best.z, t, present);
    point_3<Tr> point = r.origin + t * r.direction;
    conditional_assign(best.point, point, present);
    conditional_assign(best.normal, normalized(point - s.center), present);
    conditional_assign(best.color, s.color, present);
}


