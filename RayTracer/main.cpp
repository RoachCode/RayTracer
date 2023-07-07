#include <iostream>
#include <string>
#include "Pixel.h"

#include "Weekend.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
int vertFOV;
// rays per pixel
const int samplesPerPixel{ 2 };
// number of bounces
const int maxDepth{ 4 };
// determines whether we are in the render and draw loop or just the draw loop
static bool renderBool{ true };

color3 rayColor(const ray& r, const Hittable& world, int depth)
{
    hitRecord rec;

    // If we've exceeded the ray bounce limit (so we don't overflow the stack)
    // Then no more light is gathered.
    if (depth <= 0) { return color3(0, 0, 0); }

    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color3 attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * rayColor(scattered, world, depth - 1);
        }
        return color3(0, 0, 0);
    }
    Vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color3(1.0, 1.0, 1.0) + t * color3(0.5, 0.7, 1.0);
}

HittableList randomScene() 
{
    // Define the world and it's hittable objects
    HittableList world;

    // The ground is just a large diffuse sphere
    auto groundMaterial = make_shared<Lambertian>(color3(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -10000, 0), 10000, groundMaterial));

    // Many small spheres with random distributiong and composition
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto chooseMat = randomDouble();
            point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) 
            {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) 
                {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95) 
                {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else 
                {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }
    

    // Three large spheres of different material
    // Glass
    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));
    // Diffuse
    auto material2 = make_shared<Lambertian>(color3(0.1, 0.4, 0.2));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));
    // Metal
    auto material3 = make_shared<Metal>(color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

void render(unsigned int imageHeight, unsigned int imageWidth, sf::RenderWindow &window, Camera &cam, HittableList& world)
{
    static bool centered{ false };
    if (renderBool)
    {
        pixelArray.pixelVec.clear();
        window.setVerticalSyncEnabled(false);
        for (int j = imageHeight - 1; j >= 0; --j)
        {
            for (int i = 0; i < static_cast<int>(imageWidth); ++i)
            {
                color3 pixelColor(0, 0, 0);
                for (int s = 0; s < samplesPerPixel; ++s)
                {
                    auto u = (i + randomDouble()) / (imageWidth - 1);
                    auto v = (j + randomDouble()) / (imageHeight - 1);
                    ray r = cam.getRay(u, v);
                    pixelColor += rayColor(r, world, maxDepth);
                }
                writeColor(pixelColor, samplesPerPixel);
            }

            window.clear();
            pixelArray.createImage();
            window.draw(pixelArray.sprite);
            window.display();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                return;
            }
        }

        //std::cerr << "\nDone.\n";
        pixelArray.createImage();
        renderBool = false;
    }
    window.setVerticalSyncEnabled(true);
    window.draw(pixelArray.sprite);
    window.display();
}

int main()
{
    // Image

    const unsigned int imageWidth = sf::VideoMode::getDesktopMode().width;
    //const unsigned int imageHeight = static_cast<unsigned int>(imageWidth / aspectRatio);
    const unsigned int imageHeight = sf::VideoMode::getDesktopMode().height;
    const auto aspectRatio = 16.0 / 9.0;

    // World
    auto R = cos(pi / 4);
    auto world = randomScene();

    //auto material_ground = make_shared<Lambertian>(color3(0.8, 0.8, 0.0));
    //auto material_center = make_shared<Lambertian>(color3(0.99, 0.2, 0.4));
    //auto material_left = make_shared<Dielectric>(1.5);
    //auto material_right = make_shared<Metal>(color3(0.0, 0.4, 0.8), 0.0);

    //world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    //world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    //world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    vertFOV = 30;
    //distance, height, lateral
    point3 lookFrom(6, 2, 8);
    point3 lookAt(0, 0, 0);
    Vec3 vUp(0, 1, 0);
    auto distToFocus = (lookFrom - lookAt).length();
    auto aperture = 0.05;

    Camera cam(lookFrom, lookAt, vUp, vertFOV, aspectRatio, aperture, distToFocus);

    sf::RenderWindow window(sf::VideoMode(imageWidth, imageHeight), "Raytracing With SFML", sf::Style::None);
    sf::Texture screenshot;
    while (window.isOpen())
    {

        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                case sf::Keyboard::Down:
                    renderBool = true;
                    vertFOV + 20 <= 180 ? vertFOV += 20 : vertFOV = 180;
                    cam.redefine(lookFrom, lookAt, vUp, vertFOV, aspectRatio, aperture, distToFocus);
                    break;
                case sf::Keyboard::Up:
                    renderBool = true;
                    vertFOV - 20 >= 0 ? vertFOV -= 20 : vertFOV = 0;
                    cam.redefine(lookFrom, lookAt, vUp, vertFOV, aspectRatio, aperture, distToFocus);
                    break;
                case sf::Keyboard::F12:
                    screenshot.create(window.getSize().x, window.getSize().y);
                    screenshot.update(window);
                    screenshot.copyToImage().saveToFile("C:/Users/Windows/Desktop/screenshot.png");
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        // Render
        window.clear();
        render(imageHeight, imageWidth, window, cam, world);
        //sf::Time loopTime = pixelArray.clock.restart();
        //std::cout << "Loop ms: " << loopTime.asMilliseconds();

        window.display();
    }

    return 0;
}