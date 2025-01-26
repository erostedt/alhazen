#pragma once

#include "camera.hpp"
#include "image.hpp"
#include "scene.hpp"

FloatImage RenderImage(const Camera &camera, const Scene &scene, u32 rays_per_pixel, u32 max_bounces);
