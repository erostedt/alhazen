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

static std::vector<Object> CreateBox(const Point3 &p, const Point3 &q, const Quaternion &rotation, u32 material_index)
{
    std::vector<Object> sides;
    sides.reserve(6);

    Point3 min = {std::min(p.X, q.X), std::min(p.Y, q.Y), std::min(p.Z, q.Z)};
    Point3 max = {std::max(p.X, q.X), std::max(p.Y, q.Y), std::max(p.Z, q.Z)};

    Vec3 dx = {max.X - min.X, 0, 0};
    Vec3 dy = {0, max.Y - min.Y, 0};
    Vec3 dz = {0, 0, max.Z - min.Z};

    Vec3 rdx = Rotate(rotation, dx);
    Vec3 rdy = Rotate(rotation, dy);
    Vec3 rdz = Rotate(rotation, dz);

    Vec3 hdx = 0.5f * dx;
    Vec3 hdy = 0.5f * dy;
    Vec3 hdz = 0.5f * dz;

    Point3 center = min + 0.5f * (max - min);

    Point3 front = center + Rotate(rotation, hdz);
    Point3 back = center + Rotate(rotation, -hdz);
    Point3 left = center + Rotate(rotation, -hdx);
    Point3 right = center + Rotate(rotation, hdx);
    Point3 top = center + Rotate(rotation, hdy);
    Point3 bottom = center + Rotate(rotation, -hdy);

    Object qr = CreateQuad(front, rdx, rdy, material_index);
    Object qg = CreateQuad(back, -rdx, rdy, material_index + 1);
    Object qb = CreateQuad(left, -rdz, rdy, material_index + 2);
    Object qy = CreateQuad(right, rdz, rdy, material_index + 3);
    Object qm = CreateQuad(top, rdx, -rdz, material_index + 4);
    Object qc = CreateQuad(bottom, rdx, rdz, material_index + 5);

    sides.push_back(qr);
    sides.push_back(qg);
    sides.push_back(qb);
    sides.push_back(qy);
    sides.push_back(qm);
    sides.push_back(qc);
    return sides;
}

int main()
{
    Color r = {1.0f, 0.0f, 0.0f};
    Color g = {0.0f, 1.0f, 0.0f};
    Color b = {0.0f, 0.0f, 1.0f};

    Material red = CreateLambertian(r);
    Material green = CreateLambertian(g);
    Material blue = CreateLambertian(b);
    Material yellow = CreateLambertian(r + g);
    Material magenta = CreateLambertian(r + b);
    Material cyan = CreateLambertian(g + b);

    std::vector<Material> materials = {red, green, blue, yellow, magenta, cyan};

    f32 angle = 66;
    std::vector<Object> box =
        CreateBox(Point3{-2.0f, -2.0f, -2.0f}, Point3{2.0f, 2.0f, 2.0f}, CreateFromAxisAngle(UP, angle), 0);

    const auto bvh = CreateBVH(std::move(box));
    const Scene scene = {std::move(materials), std::move(bvh)};

    CameraProperties props;
    props.IdealAspectRatio = 1.0f;
    props.ImageWidth = 400;
    props.VFOVDegrees = 80.0f;

    const Camera camera = CreateCamera({0, 0, 9.0f}, ORIGIN, UP, props);

    const u32 rays_per_pixel = 10;
    const u32 max_bounces = 20;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
