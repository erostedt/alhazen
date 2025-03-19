#include <iostream>
#include <vector>

#include "bvh.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "point3.hpp"
#include "quaternion.hpp"
#include "render.hpp"
#include "vec3.hpp"

using namespace ObjectTypes;

int main()
{
    Material red = CreateLambertian(Color{0.65f, 0.05f, 0.05f});
    Material white = CreateLambertian(Color{0.73f, 0.73f, 0.73f});
    Material green = CreateLambertian(Color{0.12f, 0.45f, 0.15f});
    Material light = CreateDiffuseLighting(Color{15.0f, 15.0f, 15.0f});

    std::vector<Material> materials = {red, white, green, light};

    std::vector<Object> objects;
    objects.reserve(6);

    objects.push_back(CreateQuad({555.0f, 277.5f, 277.5f}, {0.0f, 555.0f, 0.0f}, {0.0f, 0.0f, 555.0f}, 2));
    objects.push_back(CreateQuad({0.0f, 277.5f, 277.5f}, {0.0f, 555.0f, 0.0f}, {0.0f, 0.0f, 555.0f}, 0));
    objects.push_back(CreateQuad({278.0f, 554.0f, 279.5f}, {-130.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -105.0f}, 3));
    objects.push_back(CreateQuad({277.5f, 0.0f, 277.5f}, {555.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 555.0f}, 1));
    objects.push_back(CreateQuad({277.5f, 555.0f, 277.5f}, {-555.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -555.0f}, 1));

    objects.push_back(CreateQuad({277.5f, 277.5f, 555.0f}, {555.0f, 0.0f, 0.0f}, {0.0f, 555.0f, 0.0f}, 1));

    Point3 p = {285.0f, 0.0f, 315.0f};
    Point3 q = {450.0f, 330.0f, 480.0f};
    std::vector<Object> box1 = CreateBox(p, q, CreateFromAxisAngle(UP, 15), 1);

    std::vector<Object> box2 =
        CreateBox({110.0f, 0.0f, 55.0f}, {285.0f, 165.0f, 220.0f}, CreateFromAxisAngle(UP, -18), 1);

    for (const auto &qu : box1)
    {
        objects.push_back(qu);
    }

    for (const auto &qu : box2)
    {
        objects.push_back(qu);
    }

    const auto bvh = CreateBVH(std::move(objects));
    const Scene scene = {std::move(materials), std::move(bvh), BLACK};

    CameraProperties props;
    props.IdealAspectRatio = 1.0f;
    props.ImageWidth = 600;
    props.VFOVDegrees = 40.0f;

    Point3 from = {278.0f, 278.0f, -800.0f};
    Point3 target = {278.0f, 278.0f, 0.0f};
    const Camera camera = CreateCamera(from, target, UP, props);

    const u32 rays_per_pixel = 200;
    const u32 max_bounces = 50;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
