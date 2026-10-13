#include "core/image.hpp"
#include "core/random.hpp"
#include "visualization/pgm.hpp"
#include "analysis/statistics.hpp"
#include "analysis/spatial.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

int main()
{
    const std::size_t width = 256;
    const std::size_t height = 256;

    Image2D<float> image(width, height, 0.0f);
    Random rng(42);

    // Fill the image with random values in the range [0, 1].
    for (std::size_t y = 0; y < height; y++)
        for (std::size_t x = 0; x < width; x++)
            image.at(x, y) = rng.uniform();

    // Compute and print the mean and variance of the image.
    std::cout << "Mean: " << analysis::mean(image) << '\n';
    std::cout << "Variance: " << analysis::variance(image) << '\n';

    // Compute and print the covariance at various offsets.
    std::cout << "Covariance at (0, 0): "
              << analysis::covariance(image, 0, 0) << '\n';

    std::cout << "Covariance at (1, 0): "
              << analysis::covariance(image, 1, 0) << '\n';

    std::cout << "Covariance at (0, 1): "
              << analysis::covariance(image, 0, 1) << '\n';

    std::cout << "Covariance at (1, 1): "
              << analysis::covariance(image, 1, 1) << '\n';

    std::cout << "Covariance at (1, -1): "
              << analysis::covariance(image, 1, -1) << '\n';

    // Compute and print the histogram with 10 bins.
    const auto bins = analysis::histogram(image, 10);

    for (std::size_t i = 0; i < bins.size(); ++i)
    {
        std::cout << "Bin " << i << ": " << bins[i] << '\n';
    }

    // Save the image as a PGM file.
    visualization::save_pgm(image, "white_noise.pgm");

    std::cout << "Saved white_noise.pgm\n";
    return 0;
}
