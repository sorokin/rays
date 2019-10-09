#include "drawing_area.h"
#include "camera.h"
#include "intersect.h"
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <chrono>
#include <iostream>

#include "simd.h"

namespace
{
    void render(QImage& target)
    {
        unsigned char* target_bits = target.bits();
        size_t bytes_per_line = target.bytesPerLine();

        camera<float> cam({0, 0, 0}, {0, 0, 1}, {0, 1, 0}, 0.4, 0.3);
        sphere<float> s1({0, 0, 6}, 0.8);
        point_3<float> light_dir = normalized(point_3<float>(0.4, 0.1, 1));

        size_t target_width = target.width() / 4 * 4;
        size_t target_height = target.height() / 1 * 1;

        float step_x = (cam.screen_width - 1) * 2.f / target_width;
        float step_y = (cam.screen_height - 1) * 2.f / target_height;

        point_3<float> right = cross(cam.direction, cam.up);

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
            for (size_t x = 0; x != target_width; x += 4)
            {
                point_3<float> ray_dir = cam.direction + ((float)x - target_width / 2) * step_x * right + ((float)y - target_height / 2) * step_y * cam.up;

                ray<pfloat> r(cam.origin, normalized(ray_dir + ray_offsets));

                ray_sphere_intersection<pfloat, float> intersection(r, s1);
                if (all_are_false(intersection.present))
                {
                    _mm_storeu_si128((__m128i*)dst, __m128i());
                    //_mm_storeu_si128((__m128i*)dst, _mm_set1_epi16(0x80));
                }
                else
                {
                    pfloat intensivity = -dot(intersection.normal, light_dir);
                    intensivity = max(intensivity, 0.f) & intersection.present;

                    _mm_storeu_si128((__m128i*)dst, (to_pint(intensivity * 255.f) << 8).values);
                }
                dst += sizeof(__m128i);
            }
            target_bits += bytes_per_line;
        }
    }
}

drawing_area::drawing_area(QWidget* parent)
    : QLabel(parent)
{
}

void drawing_area::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QImage img(QSize(width(), height()), QImage::Format_RGB32);
    auto start = std::chrono::high_resolution_clock::now();
    ::render(img);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    painter.drawImage(0, 0, img);
}
