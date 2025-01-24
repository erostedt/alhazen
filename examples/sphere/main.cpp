#include <cmath>
#include <iostream>

#include "camera.hpp"
#include "image.hpp"
#include "render.hpp"

int main()
{
    Scene scene = {{Sphere{{0, 0, -1}, 0.5f}, Sphere{{0, -100.5, -1}, 100.0f}}};
    Camera camera = CreateCamera({0, 0, 0}, {0, 0, -1}, CameraProperties{});
    FloatImage image = RenderImage(camera, scene);
    WriteFloatImageToPPM(std::cout, image);
}
