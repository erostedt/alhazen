#include <iostream>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "image.hpp"
#include "material.hpp"
#include "object.hpp"
#include "random.hpp"
#include "render.hpp"
#include "vec3.hpp"

int main()
{
    std::vector<Material> materials;
    materials.reserve(500);

    std::vector<Object> objects;
    objects.reserve(500);

    materials.push_back(CreateLambertian({0.5f, 0.5f, 0.5f}));
    objects.push_back(CreateSphere({0.0f, -1000.0f, 0.0f}, 1000.0f, 0));

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

            objects.push_back(CreateSphere(center, 0.2f, (u32)materials.size()));
            if (choose_mat < 0.8f)
            {
                // diffuse
                Color albedo = RandomColor() * RandomColor();
                materials.push_back(CreateLambertian(albedo));
            }
            else if (choose_mat < 0.95f)
            {
                // metal
                Color albedo = RandomColor(0.5f, 1.0f);
                f32 fuzz = UniformF32(0.0f, 0.5f);
                materials.push_back(CreateMetal(albedo, fuzz));
            }
            else
            {
                // glass
                materials.push_back(CreateDielectric(1.5f));
            }
        }
    }

    objects.push_back(CreateSphere({0.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateDielectric(1.5f));

    objects.push_back(CreateSphere({-4.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateLambertian({0.4f, 0.2f, 0.1f}));

    objects.push_back(CreateSphere({4.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateMetal({0.7f, 0.6f, 0.5f}, 0.0f));

    const Scene scene = {materials, objects};

    CameraProperties props;
    props.ImageWidth = 1200;
    props.VFOVDegrees = 20.0f;
    props.DefocusAngleDegrees = 0.6f;
    props.FocusDistance = 10.0f;

    const Camera camera = CreateCamera({13, 2, 3}, {0, 0, 0}, UP, props);
    const u32 rays_per_pixel = 10;
    const u32 max_bounces = 20;
    FloatImage image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
