#include "camera.hpp"

Ray Camera::GenerateRay(u32 x, u32 y) const
{
    const Point3 pixel_center = PixelTopLeft + ((f32)x * PixelDeltaU) + ((f32)y * PixelDeltaV);
    const Vec3 ray_direction = pixel_center - Position;
    return {Position, ray_direction};
}

Camera CreateCamera(Point3 position, Vec3 forward, CameraProperties properties)
{
    u32 image_width = properties.ImageWidth;
    f32 ideal_aspect_ratio = properties.IdealAspectRatio;
    f32 focal_length = properties.FocalLength;
    f32 viewport_height = properties.ViewportHeight;

    u32 image_height = (u32)((f32)image_width / ideal_aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    f32 actual_aspect_ratio = (f32)image_width / (f32)image_height;
    f32 viewport_width = viewport_height * actual_aspect_ratio;

    Vec3 viewport_u = {viewport_width, 0.0f, 0.0f};
    Vec3 viewport_v = {0.0f, -viewport_height, 0.0f};

    Point3 viewport_center = position + focal_length * forward;
    Point3 viewport_upper_left = viewport_center - viewport_u / 2 - viewport_v / 2;

    Camera camera;
    camera.Position = position;
    camera.PixelDeltaU = viewport_u / (f32)image_width;
    camera.PixelDeltaV = viewport_v / (f32)image_height;
    camera.PixelTopLeft = viewport_upper_left + 0.5 * (camera.PixelDeltaU + camera.PixelDeltaV);
    camera.ImageResolution = {image_width, image_height};
    return camera;
}
