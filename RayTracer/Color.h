#pragma once
#include "Vec3.h"
#include "Pixel.h"

//utility function to write a single pixel's color out to the standard output stream. 
void writeColor(color3 pixelColor, int samplesPerPixel)
{
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Divide the colour by the number of samples. Gamma correct for gamme = 2.0.
    auto scale = 1.0 / samplesPerPixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each colour component.
    pixelArray.insert(static_cast<sf::Uint8>(256 * clamp(r, 0.0, 0.999)),
                      static_cast<sf::Uint8>(256 * clamp(g, 0.0, 0.999)), 
                      static_cast<sf::Uint8>(256 * clamp(b, 0.0, 0.999)));


}