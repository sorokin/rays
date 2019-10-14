#include "drawing_area.h"
#include "camera.h"
#include "intersect.h"
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <chrono>
#include <iostream>

#include "scene.h"

drawing_area::drawing_area(QWidget* parent)
    : QLabel(parent)
    , start_time(std::chrono::high_resolution_clock::now())
{
    timer.setInterval(1);
    timer.start();
    connect(&timer, &QTimer::timeout, this, [this]
    {
        repaint();
    });
}

void drawing_area::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    int w = width();
    int h = height();
    QImage img(QSize(w, h), QImage::Format_RGB32);
    float current_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1000000.f;

    scene s;
    s.cam = camera<float>({0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 0.4, 0.4 * ((float)h / w));
    s.light_dir = normalized(point_3<float>(3 * sin(current_time * 3.f), 3 * cos(current_time * 5.f), -1));
    //s.light_dir = point_3(0, 0, -1);
    s.objects.push_back(sphere<float>({-1, -1, -6}, 0.75, {1, 1, 0}));
    s.objects.push_back(sphere<float>({-1, 1, -6}, 0.75, {1, 0, 0}));
    s.objects.push_back(sphere<float>({1, 1, -6}, 0.75, {0, 0, 1}));
    s.objects.push_back(sphere<float>({1, -1, -6}, 0.75, {0, 1, 0}));

    auto start = std::chrono::high_resolution_clock::now();
    ::render(s, img.bits(), w / 4 * 4, h, img.bytesPerLine());
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << w << 'x' << h << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    painter.drawImage(0, 0, img);
}
