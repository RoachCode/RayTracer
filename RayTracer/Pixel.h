#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PixelArray
{
public:
    sf::Clock clock;
    std::vector<sf::Uint8> pixelVec;
    void insert(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b)
    {
        pixelVec.push_back(r);
        pixelVec.push_back(g);
        pixelVec.push_back(b);
        pixelVec.push_back(static_cast<sf::Uint8>(255));
    }
    sf::Texture texture;
    sf::Sprite sprite;
    const unsigned int width = sf::VideoMode::getDesktopMode().width;
    const unsigned int height = sf::VideoMode::getDesktopMode().height;
    void createImage()
    {
        uint8_t* pixels = new uint8_t[width * height * 4];
        for (int i = 0; i < static_cast<int>(pixelVec.size()); i++)
        {
            pixels[i] = pixelVec[i];
        }
        const int missingPixelCount = (width * height * 4) - pixelVec.size();
        for (int i = 0; i < missingPixelCount; ++i)
        {
            pixels[pixelVec.size() + i] = sf::Uint8(155);
        }
        if (texture.getSize().x == 0) texture.create(width, height);
        texture.update(pixels);
        sprite.setTexture(texture);
        delete[] pixels;
    }

};
PixelArray pixelArray;