#include <iostream>
#include <vector>

#include "bvh.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "point3.hpp"
#include "random.hpp"
#include "render.hpp"
#include "texture.hpp"
#include "vec3.hpp"

int main()
{
    std::vector<Material> materials;
    materials.reserve(500);

    std::vector<Object> objects;
    objects.reserve(500);

    objects.push_back(CreateStationarySpere({0.0f, -1000.0f, 0.0f}, 1000.0f, 0));
    materials.push_back(CreateLambertian(CreateCheckerTexture(0.32f, {0.2f, 0.3f, 0.1f}, {0.9f, 0.9f, 0.9f})));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            auto choose_mat = UniformF32();
            Point3 center((f32)a + 0.9f * UniformF32(), 0.2f, (f32)b + 0.9f * UniformF32());

            if (SquaredLength(center - Point3(4.0f, 0.2f, 0.0f)) <= 0.9f * 0.9f)
            {
                continue;
            }

            if (choose_mat < 0.8f)
            {
                // diffuse
                Point3 end_center = center + Vec3(0, UniformF32(0.0f, 0.5f), 0);
                objects.push_back(CreateMovingSphere(center, end_center, 0.2f, (u32)materials.size()));
                Color albedo = RandomColor() * RandomColor();
                materials.push_back(CreateLambertian(albedo));
            }
            else if (choose_mat < 0.95f)
            {
                // metal
                objects.push_back(CreateStationarySpere(center, 0.2f, (u32)materials.size()));
                Color albedo = RandomColor(0.5f, 1.0f);
                f32 fuzz = UniformF32(0.0f, 0.5f);
                materials.push_back(CreateMetal(albedo, fuzz));
            }
            else
            {
                // glass
                objects.push_back(CreateStationarySpere(center, 0.2f, (u32)materials.size()));
                materials.push_back(CreateDielectric(1.5f));
            }
        }
    }

    objects.push_back(CreateStationarySpere({0.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateDielectric(1.5f));

    objects.push_back(CreateStationarySpere({-4.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateLambertian(Color{0.4f, 0.2f, 0.1f}));

    objects.push_back(CreateStationarySpere({4.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateMetal({0.7f, 0.6f, 0.5f}, 0.0f));

    const auto bvh = CreateBVH(std::move(objects));
    const Scene scene = {std::move(materials), std::move(bvh)};

    CameraProperties props;
    props.ImageWidth = 800;
    props.VFOVDegrees = 20.0f;
    props.DefocusAngleDegrees = 0.6f;
    props.FocusDistance = 10.0f;

    const Camera camera = CreateCamera({13.0f, 2.0f, 3.0f}, ORIGIN, UP, props);
    const u32 rays_per_pixel = 20;
    const u32 max_bounces = 10;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
