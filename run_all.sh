#!/bin/bash

mkdir -p outputs

time ./build/bouncing_spheres >outputs/bouncing_spheres.ppm
time ./build/checkered_spheres >outputs/checkered_spheres.ppm
time ./build/earth >outputs/earth.ppm
time ./build/perlin_spheres >outputs/perlin_spheres.ppm
time ./build/quads >outputs/quads.ppm
time ./build/raytracing_in_a_weekend >outputs/raytracing_in_a_weekend.ppm
