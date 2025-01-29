#pragma once

#include "point3.hpp"
#include "ray.hpp"
#include "types.hpp"
#include "vec3.hpp"

struct CameraProperties
{
    u32 ImageWidth = 400;
    f32 IdealAspectRatio = 16.0f / 9.0f;
    f32 FocalLength = 1.0f;
    f32 VFOVDegrees = 90.0f;
};

struct Resolution
{
    u32 Width;
    u32 Height;
};

struct Camera
{
    Point3 Position;
    Point3 PixelTopLeft;
    Vec3 PixelDeltaU;
    Vec3 PixelDeltaV;
    Resolution ImageResolution;

    Ray GenerateRay(f32 x, f32 y) const;
};

Camera CreateCamera(Point3 position, Vec3 forward, CameraProperties properties);
