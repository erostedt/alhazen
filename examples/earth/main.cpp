#include <iostream>
#include <vector>

#include "bvh.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "point3.hpp"
#include "render.hpp"
#include "texture.hpp"
#include "vec3.hpp"

int main()
{

    Texture texture = CreateImageTexture(LoadFloatImage("./images/earthmap.jpg"));
    Material material = CreateLambertian(texture);

    std::vector<Material> materials = {material};

    Object globe = CreateStationarySpere(ORIGIN, 2.0f, 0);
    std::vector<Object> objects = {globe};

    const auto bvh = CreateBVH(std::move(objects));
    const Scene scene = {std::move(materials), std::move(bvh)};

    CameraProperties props;
    props.ImageWidth = 400;
    props.VFOVDegrees = 20.0f;

    const Camera camera = CreateCamera({0.0f, 0.0f, 12.0f}, ORIGIN, UP, props);

    const u32 rays_per_pixel = 100;
    const u32 max_bounces = 50;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
