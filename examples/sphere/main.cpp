#include <iostream>
#include <vector>

#include "camera.hpp"
#include "image.hpp"
#include "material.hpp"
#include "render.hpp"
#include "vec3.hpp"

int main()
{
    const std::vector<Material> materials = {
        CreateLambertian({0.8f, 0.8f, 0.0f}), CreateLambertian({0.1f, 0.2f, 0.5f}),  CreateDielectric(1.5f),
        CreateDielectric(1.0f / 1.5f),        CreateMetal({0.8f, 0.6f, 0.2f}, 1.0f),
    };

    const std::vector<Sphere> objects = {
        Sphere{{0.0f, -100.5f, -1.0f}, 100.0f, 0}, Sphere{{0.0, 0.0, -1.2f}, 0.5f, 1},
        Sphere{{-1.0f, 0.0f, -1.0f}, 0.5f, 2},     Sphere{{-1.0f, 0.0f, -1.0f}, 0.4f, 3},
        Sphere{{1.0f, 0.0f, -1.0f}, 0.5f, 4},
    };

    const Scene scene = {materials, objects};

    CameraProperties props;
    props.ImageWidth = 400;
    props.VFOVDegrees = 90.0f;

    const Camera camera = CreateCamera({-2, 2, 1}, {0, 0, -1}, UP, props);
    const u32 rays_per_pixel = 10;
    const u32 max_bounces = 50;
    FloatImage image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
