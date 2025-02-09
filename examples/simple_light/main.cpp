#include <iostream>
#include <vector>

#include "bvh.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "perlin.hpp"
#include "render.hpp"
#include "texture.hpp"
#include "vec3.hpp"

int main()
{
    Texture noise_texture = CreateNoiseTexture(CreatePerlinNoise(), 4.0f);
    Material noise = CreateLambertian(noise_texture);
    Material light = CreateDiffuseLighting({4.0f, 4.0f, 4.0f});

    std::vector<Material> materials = {noise, light};

    std::vector<Object> objects;
    objects.reserve(4);

    objects.push_back(CreateStationarySpere({0.0f, -1000.0f, 0.0f}, 1000.0f, 0));
    objects.push_back(CreateStationarySpere({0.0f, 2.0f, 0.0f}, 2.0f, 0));
    objects.push_back(CreateQuad({3.0f, 1.0f, -2.0f}, {2.0f, 0.0f, 0.0f}, {0.0f, 2.0f, 0.0f}, 1));
    objects.push_back(CreateStationarySpere({0.0f, 7.0f, 0.0f}, 2.0f, 1));

    const auto bvh = CreateBVH(std::move(objects));
    const Scene scene = {std::move(materials), std::move(bvh), BLACK};

    CameraProperties props;
    props.ImageWidth = 400;
    props.VFOVDegrees = 20.0f;

    const Camera camera = CreateCamera({26.0f, 3.0f, 6.0f}, {0.0f, 2.0f, 0.0f}, UP, props);

    const u32 rays_per_pixel = 100;
    const u32 max_bounces = 50;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
