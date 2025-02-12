#include <iostream>
#include <vector>

#include "bvh.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "point3.hpp"
#include "render.hpp"
#include "vec3.hpp"

int main()
{
    Material left_red = CreateLambertian(Color{1.0f, 0.2f, 0.2f});
    Material back_green = CreateLambertian(Color{0.2f, 1.0f, 0.2f});
    Material right_blue = CreateLambertian(Color{0.2f, 0.2f, 1.0f});
    Material upper_orange = CreateLambertian(Color{1.0f, 0.5f, 0.0f});
    Material lower_teal = CreateLambertian(Color{0.2f, 0.8f, 0.8f});

    std::vector<Material> materials = {left_red, back_green, right_blue, upper_orange, lower_teal};

    std::vector<Object> objects = {
        CreateQuad({-3.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -4.0f}, {0.0f, 4.0f, 0.0f}, 0),
        CreateQuad({0.0f, 0.0f, 0.0f}, {4.0f, 0.0f, 0.0f}, {0.0f, 4.0f, 0.0f}, 1),
        CreateQuad({3.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 4.0f}, {0.0f, 4.0f, 0.0f}, 2),
        CreateQuad({0.0f, 3.0f, 3.0f}, {4.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 4.0f}, 3),
        CreateQuad({0.0f, -3.0f, 3.0f}, {4.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -4.0f}, 4),
    };

    const auto bvh = CreateBVH(std::move(objects));
    const Scene scene = {std::move(materials), std::move(bvh)};

    CameraProperties props;
    props.IdealAspectRatio = 1.0f;
    props.ImageWidth = 400;
    props.VFOVDegrees = 80.0f;

    const Camera camera = CreateCamera({0.0f, 0.0f, 9.0f}, ORIGIN, UP, props);

    const u32 rays_per_pixel = 100;
    const u32 max_bounces = 50;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
