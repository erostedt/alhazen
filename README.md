# About

Single threaded cpu raytracer with no dependencies but standard library.
This is an alternative implementation to the first two books in the series:
[Raytracing in one weekend](https://raytracing.github.io/)

The implementation does not use any interfaces/virtual functions and instead
uses tagged unions, free functions and indices to utilize a more "procedural" approach

## Install

You need [cmake](https://cmake.org/)
Only tested for `cmake` version >= 3.25, but likely works for earlier versions.

- run `./install.sh` for building a debug build
- run `./install.sh Release` for building a release build

You can run the examples one by one or run all using the `run_all.sh` script

## How to use

The project aims to be self documented and looking the `run_all.sh` script and the
examples for how to use.

But generally something like this:

```cpp
    # Create storage for materials and objects
    std::vector<Material> materials;
    materials.reserve(4);

    std::vector<Object> objects;
    objects.reserve(4);

    # Add materials and objects
    objects.push_back(CreateStationarySpere({0.0f, -1000.0f, 0.0f}, 1000.0f, (u32)materials.size()));
    materials.push_back(CreateLambertian(Color{0.5f, 0.5f, 0.5f}));

    objects.push_back(CreateStationarySpere({0.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateDielectric(1.5f));

    objects.push_back(CreateStationarySpere({-4.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateLambertian(Color{0.4f, 0.2f, 0.1f}));

    objects.push_back(CreateStationarySpere({4.0f, 1.0f, 0.0f}, 1.0f, (u32)materials.size()));
    materials.push_back(CreateMetal({0.7f, 0.6f, 0.5f}, 0.0f));

    # Move objects into bounding volume hierarchy
    const auto bvh = CreateBVH(std::move(objects));

    # Create scene
    const Scene scene = {std::move(materials), std::move(bvh)};

    # Create camera
    CameraProperties props;
    props.ImageWidth = 1200;
    props.VFOVDegrees = 20.0f;
    props.DefocusAngleDegrees = 0.6f;
    props.FocusDistance = 10.0f;

    const Camera camera = CreateCamera({13.0f, 2.0f, 3.0f}, ORIGIN, UP, props);

    # Render scene
    const u32 rays_per_pixel = 10;
    const u32 max_bounces = 20;
    auto image = RenderImage(camera, scene, rays_per_pixel, max_bounces);

    # Dump pixels
    WriteFloatImageToPPM(std::cout, image);

```
