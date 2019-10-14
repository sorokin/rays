#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "camera.h"
#include "point.h"
#include "sphere.h"

struct scene
{
    scene() = default;

    camera<float> cam;
    point_3<float> light_dir;
    std::vector<sphere<float>> objects;
};

struct renderer
{
    renderer();
    
    void render();
    
    std::mutex m;
    bool has_work;
    std::condition_variable has_work_cv;
    size_t pending;
    std::condition_variable pending_cv;
    std::vector<std::thread> threads;
};

void render(scene const&, void* __restrict out, size_t width, size_t height, size_t bytes_per_line);
