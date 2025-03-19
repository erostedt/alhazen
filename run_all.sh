#!/bin/bash

mkdir -p outputs

time ./build/bouncing_spheres >outputs/bouncing_spheres.ppm
time ./build/checkered_spheres >outputs/checkered_spheres.ppm
time ./build/cornell_box >outputs/cornell_box.ppm
time ./build/earth >outputs/earth.ppm
time ./build/perlin_spheres >outputs/perlin_spheres.ppm
time ./build/quads >outputs/quads.ppm
time ./build/raytracing_in_a_weekend >outputs/raytracing_in_a_weekend.ppm
time ./build/raytracing_the_next_week >outputs/raytracing_the_next_week.ppm
time ./build/simple_light >outputs/simple_light.ppm
