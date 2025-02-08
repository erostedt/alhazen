
#include <iostream>
#include <vector>

#include "bvh.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "perlin.hpp"
#include "point3.hpp"
#include "render.hpp"
#include "texture.hpp"
#include "vec3.hpp"

int main()
{
    Texture noise_texture = CreateNoiseTexture(CreatePerlinNoise(), 4.0f);
    Material material = CreateLambertian(noise_texture);

    std::vector<Material> materials = {material};

    std::vector<Object> objects;
    objects.reserve(2);

    objects.push_back(CreateStationarySpere({0.0f, -1000.0f, 0.0f}, 1000.0f, 0));
    objects.push_back(CreateStationarySpere({0.0f, 2.0f, 0.0f}, 2.0f, 0));

    const auto bvh = CreateBVH(std::move(objects));
    const Scene scene = {std::move(materials), std::move(bvh)};

    CameraProperties props;
    props.ImageWidth = 400;
    props.VFOVDegrees = 20.0f;

    const Camera camera = CreateCamera({13.0f, 2.0f, 3.0f}, ORIGIN, UP, props);

    const u32 rays_per_pixel = 100;
    const u32 max_bounces = 50;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
