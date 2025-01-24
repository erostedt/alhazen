#include "color.hpp"

Color LinearBlend(Color c1, Color c2, f32 a)
{
    Color blended;
    blended.Red = a * c1.Red + (1.0f - a) * c2.Red;
    blended.Green = a * c1.Green + (1.0f - a) * c2.Green;
    blended.Blue = a * c1.Blue + (1.0f - a) * c2.Blue;
    return blended;
}
