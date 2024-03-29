#pragma once

#include <emmintrin.h>
#include <smmintrin.h>
#include "point.h"

struct pfloat
{
    pfloat() = default;

    pfloat(float value)
        : values(_mm_set1_ps(value))
    {}

    pfloat(float v0, float v1, float v2, float v3)
        : values(_mm_set_ps(v0, v1, v2, v3))
    {}

    pfloat(__m128 values)
        : values(values)
    {}

    __m128 values;
};

struct pmask
{
    pmask() = default;

    pmask(__m128 values)
        : values(values)
    {}

    __m128 values;
};

inline pfloat operator+(pfloat a, pfloat b)
{
    return _mm_add_ps(a.values, b.values);
}

inline pfloat operator-(pfloat a, pfloat b)
{
    return _mm_sub_ps(a.values, b.values);
}

inline pfloat operator*(pfloat a, pfloat b)
{
    return _mm_mul_ps(a.values, b.values);
}

inline pfloat& operator*=(pfloat& a, pfloat b)
{
    a.values = _mm_mul_ps(a.values, b.values);
    return a;
}

inline pfloat operator/(pfloat a, pfloat b)
{
    return _mm_div_ps(a.values, b.values);
}

inline pfloat operator-(pfloat a)
{
    return _mm_sub_ps(_mm_set1_ps(0.f), a.values);
}

inline pfloat sqrt(pfloat a)
{
    return _mm_sqrt_ps(a.values);
}

inline pfloat min(pfloat a, pfloat b)
{
    return _mm_min_ps(a.values, b.values);
}

inline pfloat max(pfloat a, pfloat b)
{
    return _mm_max_ps(a.values, b.values);
}

inline pmask operator<(pfloat a, pfloat b)
{
    return _mm_cmplt_ps(a.values, b.values);
}

inline pmask operator<=(pfloat a, pfloat b)
{
    return _mm_cmple_ps(a.values, b.values);
}

inline pmask operator==(pfloat a, pfloat b)
{
    return _mm_cmpeq_ps(a.values, b.values);
}

inline pmask operator>(pfloat a, pfloat b)
{
    return _mm_cmpgt_ps(a.values, b.values);
}

inline pmask operator>=(pfloat a, pfloat b)
{
    return _mm_cmpge_ps(a.values, b.values);
}

inline bool all_are_false(pmask a)
{
    return _mm_movemask_ps(a.values) == 0;
}

inline bool all_are_true(pmask a)
{
    return _mm_movemask_ps(a.values) == 0x0f;
}

inline bool some_are_false(pmask a)
{
    return _mm_movemask_ps(a.values) != 0x0f;
}

inline bool some_are_true(pmask a)
{
    return _mm_movemask_ps(a.values) != 0;
}

inline pfloat andnot(pfloat a, pmask b)
{
    return _mm_andnot_ps(a.values, b.values);
}

inline pmask operator&(pmask a, pmask b)
{
    return _mm_and_ps(a.values, b.values);
}

inline pfloat operator&(pfloat a, pmask b)
{
    return _mm_and_ps(a.values, b.values);
}

inline pfloat blend(pfloat iffalse, pfloat iftrue, pmask m)
{
    return _mm_blendv_ps(iffalse.values, iftrue.values, m.values);
}

inline void conditional_assign(pfloat& lhs, pfloat rhs, pmask m)
{
    lhs = blend(lhs, rhs, m);
}

inline void conditional_assign(point_3<pfloat>& lhs, point_3<pfloat> rhs, pmask m)
{
    conditional_assign(lhs.x, rhs.x, m);
    conditional_assign(lhs.y, rhs.y, m);
    conditional_assign(lhs.z, rhs.z, m);
}

struct pint
{
    pint() = default;

    pint(__m128i values)
        : values(values)
    {}

    __m128i values;
};

inline pint operator|(pint a, pint b)
{
    return _mm_or_si128(a.values, b.values);
}

inline pint to_pint(pfloat a)
{
    return _mm_cvtps_epi32(a.values);
}

inline pint operator<<(pint a, int b)
{
    return _mm_slli_epi32(a.values, b);
}
