#include "core/image.hpp"
#include "core/random.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

int main()
{
    const std::size_t width = 256;
    const std::size_t height = 256;

    Image2D<float> image(width, height, 0.0f);
    Random rng(42);

    // Fill the image with independent uniform random values.
    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            image.at(x, y) = rng.uniform();
        }
    }

    // Save as an ASCII grayscale PGM image.
    std::ofstream file("white_noise.pgm");

    if (!file)
    {
        throw std::runtime_error("Could not open output file");
    }

    file << "P2\n";
    file << image.width() << " " << image.height() << "\n";
    file << "255\n";

    for (std::size_t y = 0; y < image.height(); ++y)
    {
        for (std::size_t x = 0; x < image.width(); ++x)
        {
            const int value =
                static_cast<int>(image.at(x, y) * 255.0f);

            file << value << " ";
        }

        file << "\n";
    }

    std::cout << "Saved white_noise.pgm\n";
    return 0;
}
