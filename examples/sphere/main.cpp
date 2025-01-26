#include <cmath>
#include <iostream>

#include "camera.hpp"
#include "image.hpp"
#include "render.hpp"

int main()
{
    const Scene scene = {{Sphere{{0, 0, -1}, 0.5f}, Sphere{{0, -100.5, -1}, 100.0f}}};

    CameraProperties props;
    props.ImageWidth = 400;

    const Camera camera = CreateCamera({0, 0, 0}, {0, 0, -1}, props);
    const u32 rays_per_pixel = 10;
    const u32 max_bounces = 50;
    FloatImage image = RenderImage(camera, scene, rays_per_pixel, max_bounces);
    WriteFloatImageToPPM(std::cout, image);
}
