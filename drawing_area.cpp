#include "drawing_area.h"
#include "camera.h"
#include "intersect.h"
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <chrono>
#include <iostream>

namespace
{
    void render(QImage& target)
    {
        unsigned char* target_bits = target.bits();
        size_t bytes_per_line = target.bytesPerLine();

        camera<float> cam({0, 0, 0}, {0, 0, 1}, {0, 1, 0}, 0.4, 0.3);
        sphere<float> s1({0, 0, 6}, 0.8);
        point_3<float> light_dir = normalized(point_3<float>(0.4, 0.1, 1));

        size_t target_width = target.width();
        size_t target_height = target.height();

        float step_x = (cam.screen_width - 1) * 2.f / target_width;
        float step_y = (cam.screen_height - 1) * 2.f / target_height;

        point_3<float> right = cross(cam.direction, cam.up);

        for (size_t y = 0; y != target_height; ++y)
        {
            unsigned char* dst = target_bits;
            for (size_t x = 0; x != target_width; ++x)
            {
                point_3<float> ray_dir = normalized(cam.direction + ((float)x - target_width / 2) * step_x * right + ((float)y - target_height / 2) * step_y * cam.up);

                ray<float> r(cam.origin, ray_dir);

                ray_sphere_intersection<float> intersection(r, s1);
                if (intersection.present)
                {
                    float intensivity = -dot(intersection.normal, light_dir);
                    if (intensivity < 0)
                        intensivity = 0;
                    
                    *dst++ = 0;
                    *dst++ = intensivity * 255.;
                    *dst++ = 0;
                    *dst++ = 0;
                }
                else
                {
                    *dst++ = 0;
                    *dst++ = 0;
                    *dst++ = 0;
                    *dst++ = 0;
                }
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
