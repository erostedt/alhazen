#include "camera.hpp"

#include <cmath>
#include <numbers>

#include "vec3.hpp"

Ray Camera::GenerateRay(f32 x, f32 y) const
{
    const Point3 pixel_center = PixelTopLeft + (x * PixelDeltaU) + (y * PixelDeltaV);
    const Point3 ray_origin = (DefocusDiskAngle <= 0) ? Position : SampleDefocusDisk();
    const Vec3 ray_direction = pixel_center - ray_origin;
    return {ray_origin, Normalized(ray_direction)};
}

Point3 Camera::SampleDefocusDisk() const
{
    Vec3 v = RandomVectorOnUnitDisk();
    return Position + (v.X * DefocusDiskU) + (v.Y * DefocusDiskV);
}

struct Viewport
{
    f32 Width;
    f32 Height;
};

f32 Radians(f32 degrees)
{
    return degrees * std::numbers::pi_v<f32> / 180.0f;
}

static Viewport CalculateViewport(f32 vfov_degrees, f32 focus_distance, f32 aspect_ratio)
{
    f32 angle = Radians(vfov_degrees);
    f32 height = std::tan(angle / 2.0f);
    f32 viewport_height = 2.0f * height * focus_distance;
    f32 viewport_width = viewport_height * aspect_ratio;

    return {viewport_width, viewport_height};
}

Camera CreateCamera(Point3 position, Point3 target, Vec3 up, CameraProperties properties)
{
    u32 image_width = properties.ImageWidth;
    f32 ideal_aspect_ratio = properties.IdealAspectRatio;
    f32 defocus_angle = Radians(properties.DefocusAngleDegrees);
    f32 focus_distance = properties.FocusDistance;

    u32 image_height = (u32)((f32)image_width / ideal_aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    f32 actual_aspect_ratio = (f32)image_width / (f32)image_height;

    Viewport viewport = CalculateViewport(properties.VFOVDegrees, focus_distance, actual_aspect_ratio);

    Vec3 forward = -Normalized(target - position); // Minus is forward
    Vec3 rightward = Cross(up, forward);
    Vec3 upward = Cross(forward, rightward);

    Vec3 viewport_u = viewport.Width * rightward;
    Vec3 viewport_v = -viewport.Height * upward;

    Point3 viewport_center = position - focus_distance * forward;
    Point3 viewport_upper_left = viewport_center - viewport_u / 2 - viewport_v / 2;

    f32 defocus_radius = focus_distance * std::tan(defocus_angle / 2.0f);
    Vec3 defocus_disk_u = defocus_radius * rightward;
    Vec3 defocus_disk_v = defocus_radius * upward;

    Camera camera;
    camera.Position = position;

    camera.PixelDeltaU = viewport_u / (f32)image_width;
    camera.PixelDeltaV = viewport_v / (f32)image_height;
    camera.PixelTopLeft = viewport_upper_left + 0.5 * (camera.PixelDeltaU + camera.PixelDeltaV);

    camera.DefocusDiskAngle = defocus_angle;
    camera.DefocusDiskU = defocus_disk_u;
    camera.DefocusDiskV = defocus_disk_v;

    camera.Right = rightward;
    camera.Up = upward;
    camera.Forward = forward;

    camera.ImageResolution = {image_width, image_height};
    return camera;
}
