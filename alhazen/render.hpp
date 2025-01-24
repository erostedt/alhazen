#pragma once

#include "camera.hpp"
#include "image.hpp"
#include "scene.hpp"

FloatImage RenderImage(const Camera &camera, const Scene &scene);
