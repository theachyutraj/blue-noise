#include "core/image.hpp"
#include "core/random.hpp"
#include "visualization/pgm.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

int main()
{
    const std::size_t width = 256;
    const std::size_t height = 256;

    Image2D<float> image(width, height, 0.0f);
    Random rng(41);

    for (std::size_t y = 0; y < height; y++)
        for (std::size_t x = 0; x < width; x++)
            image.at(x, y) = rng.uniform();

    visualization::save_pgm(image, "white_noise.pgm");

    std::cout << "Saved white_noise.pgm\n";
    return 0;
}
