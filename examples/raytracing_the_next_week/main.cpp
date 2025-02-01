#include <iostream>

#include "camera.hpp"
#include "image.hpp"
#include "point3.hpp"
#include "render.hpp"
#include "vec3.hpp"

int main()
{
    const Scene scene;

    CameraProperties props;
    props.ImageWidth = 400;
    props.VFOVDegrees = 20.0f;
    props.DefocusAngleDegrees = 0.6f;
    props.FocusDistance = 10.0f;

    const Camera camera = CreateCamera({13.0f, 2.0f, 3.0f}, ORIGIN, UP, props);
    const u32 rays_per_pixel = 10;
    const u32 max_bounces = 20;
    FloatImage image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
