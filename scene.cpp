#include "scene.h"
#include <cassert>
#include "simd.h"
#include "ray.h"
#include "intersect.h"

void render(scene const& scene, void* __restrict out, size_t target_width, size_t target_height, size_t bytes_per_line)
{
    assert((target_width % 4) == 0);
    unsigned char* target_bits = static_cast<unsigned char*>(out);

    float step_x = (scene.cam.screen_width) * 2.f / (target_width - 1);
    float step_y = -(scene.cam.screen_height) * 2.f / (target_height - 1);

    point_3<float> right = cross(scene.cam.direction, scene.cam.up);

    point_3<float> v01 = 1 * step_x * right;
    point_3<float> v10 = 2 * step_x * right;
    point_3<float> v11 = 3 * step_x * right;

    point_3<pfloat> ray_offsets(pfloat(v11.x, v10.x, v01.x, 0.f),
                                pfloat(v11.y, v10.y, v01.y, 0.f),
                                pfloat(v11.z, v10.z, v01.z, 0.f));
    //point_3<pfloat> ray_offsets = point_3<pfloat>();

    for (size_t y = 0; y != target_height; y += 1)
    {
        unsigned char* dst = target_bits;
        point_3<float> ray_dir_y = scene.cam.direction + ((float)(std::ptrdiff_t)y - (std::ptrdiff_t)(target_height / 2)) * step_y * scene.cam.up;
        for (size_t x = 0; x != target_width; x += 4)
        {
            point_3<float> ray_dir = ray_dir_y + ((float)(std::ptrdiff_t)x - (std::ptrdiff_t)(target_width / 2)) * step_x * right;

            ray<pfloat> r(scene.cam.origin, normalized(ray_dir + ray_offsets));

            best_intersection<pfloat> best;
            best.z = _mm_set1_ps(std::numeric_limits<float>::infinity());
            best.point = {};
            best.normal = {};
            best.color = {};

            for (auto const& obj : scene.objects)
                intersect_ray_sphere(r, obj, best);

            pfloat intensivity = -dot(best.normal, scene.light_dir);
            intensivity = max(intensivity, 0.f);

            intensivity *= 255.f;
            __m128i result = (to_pint(intensivity * best.color.x) << 16
                            | to_pint(intensivity * best.color.y) << 8
                            | to_pint(intensivity * best.color.z)).values;

            _mm_storeu_si128((__m128i*)dst, result);
            dst += sizeof(__m128i);
        }
        target_bits += bytes_per_line;
    }
}

renderer::renderer()
{
    size_t n = std::thread::hardware_concurrency();

    for (size_t i = 0; i != n; ++i)
    std::thread([this, i]
    {
        for (;;)
        {
            {
                std::unique_lock lg(m);
                has_work_cv.wait(lg, [this]{ return has_work; });
            }

            // ...

            {
                std::unique_lock lg(m);
                --pending;
                if (pending == 0)
                    pending_cv.notify_one();
            }
        }
    });
}

void renderer::render()
{
    std::unique_lock lg(m);
    pending = threads.size();
    has_work = true;
    has_work_cv.notify_all();

    pending_cv.wait(lg, [this] { return pending == 0; });
}


#include <optional>

using value_type = std::optional<std::pair<int const, int>>;

value_type g()
{
    return {};
}

void f()
{
    auto node = std::make_optional(g());

    node.emplace(g());
}
