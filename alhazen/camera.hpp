#pragma once

#include "point3.hpp"
#include "ray.hpp"
#include "types.hpp"
#include "vec3.hpp"

struct CameraProperties
{
    u32 ImageWidth = 400;
    f32 IdealAspectRatio = 16.0f / 9.0f;
    f32 VFOVDegrees = 90.0f;
    f32 DefocusAngleDegrees = 0.0f;
    f32 FocusDistance = 10.0f;
};

struct Resolution
{
    u32 Width;
    u32 Height;
};

struct Camera
{
    Point3 Position;

    Vec3 PixelDeltaU;
    Vec3 PixelDeltaV;
    Point3 PixelTopLeft;

    f32 DefocusDiskAngle;
    Vec3 DefocusDiskU;
    Vec3 DefocusDiskV;

    Vec3 Right;
    Vec3 Up;
    Vec3 Forward;

    Resolution ImageResolution;

    Ray GenerateRay(f32 x, f32 y) const noexcept;
    inline Point3 SampleDefocusDisk() const noexcept
    {
        Vec3 v = RandomVectorOnUnitDisk();
        return Position + (v.X * DefocusDiskU) + (v.Y * DefocusDiskV);
    }
};

Camera CreateCamera(Point3 position, Point3 target, Vec3 up, const CameraProperties &properties);
