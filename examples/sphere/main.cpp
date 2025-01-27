#include <iostream>
#include <vector>

#include "camera.hpp"
#include "image.hpp"
#include "material.hpp"
#include "render.hpp"

int main()
{
    const std::vector<Material> materials = {
        {MaterialType::LAMBERTIAN, {.L = {0.8f, 0.8f, 0.0f}}},
        {MaterialType::LAMBERTIAN, {.L = {0.1f, 0.2f, 0.5f}}},
        {MaterialType::METAL, {.M = {0.8f, 0.8f, 0.8f}}},
        {MaterialType::METAL, {.M = {0.8f, 0.6f, 0.2f}}},
    };

    const std::vector<Sphere> objects = {
        Sphere{{0.0f, -100.5f, -1.0f}, 100.0f, 0},
        Sphere{{0.0, 0.0, -1.2f}, 0.5f, 1},
        Sphere{{-1.0f, 0.0f, -1.0f}, 0.5f, 2},
        Sphere{{1.0f, 0.0f, -1.0f}, 0.5f, 3},
    };

    const Scene scene = {materials, objects};

    CameraProperties props;
    props.ImageWidth = 400;

    const Camera camera = CreateCamera({0, 0, 0}, {0, 0, -1}, props);
    const u32 rays_per_pixel = 10;
    const u32 max_bounces = 50;
    FloatImage image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
