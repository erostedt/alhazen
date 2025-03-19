#include <iostream>
#include <vector>

#include "bvh.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "perlin.hpp"
#include "point3.hpp"
#include "quaternion.hpp"
#include "random.hpp"
#include "render.hpp"
#include "texture.hpp"
#include "vec3.hpp"

using namespace ObjectTypes;

int main()
{
    std::vector<Material> materials;
    std::vector<Object> objects;
    i32 boxes_per_side = 20;
    for (i32 i = 0; i < boxes_per_side; ++i)
    {
        for (i32 j = 0; j < boxes_per_side; ++j)
        {
            f32 w = 100.0f;
            f32 x0 = -1000.0f + (f32)i * w;
            f32 z0 = -1000.0f + (f32)j * w;
            f32 y0 = 0.0f;
            f32 x1 = x0 + w;
            f32 y1 = UniformF32(1.0f, 101.0f);
            f32 z1 = z0 + w;

            auto box = CreateBox({x0, y0, z0}, {x1, y1, z1}, IDENTITY, (u32)materials.size());
            for (const auto &quad : box)
            {
                objects.push_back(quad);
            }
        }
    }

    materials.push_back(CreateLambertian(Color{0.48f, 0.83f, 0.53f}));

    objects.push_back(
        CreateQuad({273.0f, 554.0f, 279.5f}, {300.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 265.0f}, (u32)materials.size()));
    materials.push_back(CreateDiffuseLighting({7.0f, 7.0f, 7.0f}));

    Point3 center1 = {400.0f, 400.0f, 200.0f};
    Point3 center2 = center1 + Vec3{30.0f, 0.0f, 0.0f};

    objects.push_back(CreateMovingSphere(center1, center2, 50.0f, (u32)materials.size()));
    Material sphere_material = CreateLambertian(Color{0.7f, 0.3f, 0.1f});
    materials.push_back(sphere_material);

    objects.push_back(CreateStationarySpere({260.0f, 150.0f, 45.0f}, 50.0f, (u32)materials.size()));
    materials.push_back(CreateDielectric(1.5f));

    objects.push_back(CreateStationarySpere({0.0f, 150.0f, 145.0f}, 50.0f, (u32)materials.size()));
    materials.push_back(CreateMetal({0.8f, 0.8f, 0.9f}, 1.0f));

    Object boundary1 = CreateStationarySpere({360, 150, 145}, 70, (u32)materials.size());
    objects.push_back(boundary1);
    materials.push_back(CreateDielectric(1.5));

    objects.push_back(CreateConstantMedium(&boundary1, 0.2f, (u32)materials.size()));
    materials.push_back(CreateIsotropic({0.2f, 0.4f, 0.9f}));

    Object boundary2 = CreateStationarySpere(ORIGIN, 5000.0f, (u32)materials.size());
    objects.push_back(CreateConstantMedium(&boundary2, 0.0001f, (u32)materials.size()));
    materials.push_back(CreateIsotropic(WHITE));

    objects.push_back(CreateStationarySpere({400.0f, 200.0f, 400.0f}, 100.0f, (u32)materials.size()));
    Texture texture = CreateImageTexture(LoadFloatImage("./images/earthmap.ppm"));
    materials.push_back(CreateLambertian(texture));

    objects.push_back(CreateStationarySpere({220.0f, 280.0f, 300.0f}, 80.0f, (u32)materials.size()));
    Texture noise_texture = CreateNoiseTexture(CreatePerlinNoise(), 0.2f);
    materials.push_back(CreateLambertian(noise_texture));

    Quaternion q = CreateFromAxisAngle(UP, 15);
    Point3 center_position = Point3{-100.0f, 270.0f, 395.0f} + 82.5f * ONE;
    i32 ns = 1000;
    for (i32 j = 0; j < ns; ++j)
    {
        f32 x = UniformF32(-82.5f, 82.5f);
        f32 y = UniformF32(-82.5f, 82.5f);
        f32 z = UniformF32(-82.5f, 82.5f);
        Vec3 v = {x, y, z};
        Point3 p = center_position + Rotate(q, v);
        objects.push_back(CreateStationarySpere(p, 10.0f, (u32)materials.size()));
    }

    materials.push_back(CreateLambertian(Color{0.73f, 0.73f, 0.73f}));

    const auto bvh = CreateBVH(std::move(objects));
    const Scene scene = {std::move(materials), std::move(bvh), BLACK};

    CameraProperties props;
    props.IdealAspectRatio = 1.0f;
    props.ImageWidth = 400;
    props.VFOVDegrees = 40.0f;

    Point3 from = {478.0f, 278.0f, -600.0f};
    Point3 target = {278.0f, 278.0f, 0.0f};
    const Camera camera = CreateCamera(from, target, UP, props);

    const u32 rays_per_pixel = 250;
    const u32 max_bounces = 4;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
