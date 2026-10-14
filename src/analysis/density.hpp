#pragma once

#include "analysis/spatial.hpp"
#include "core/image.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace analysis
{

    inline Image2D<float> gaussian_density(
        const Image2D<float> &mask,
        float sigma,
        int radius)
    {
        if (!std::isfinite(sigma) || sigma <= 0.0f)
        {
            throw std::invalid_argument("Sigma must be positive and finite");
        }

        if (radius < 0)
        {
            throw std::invalid_argument("Radius must be non-negative");
        }

        const std::size_t width = mask.width();
        const std::size_t height = mask.height();

        // Build a truncated, normalized 2D Gaussian kernel.
        const int side = 2 * radius + 1;
        std::vector<float> kernel(
            static_cast<std::size_t>(side) * side);

        double weight_sum = 0.0;

        for (int dy = -radius; dy <= radius; ++dy)
        {
            for (int dx = -radius; dx <= radius; ++dx)
            {
                const double distance_squared =
                    static_cast<double>(dx) * dx +
                    static_cast<double>(dy) * dy;

                const double weight = std::exp(
                    -distance_squared /
                    (2.0 * sigma * sigma));

                const std::size_t index =
                    static_cast<std::size_t>(dy + radius) * side + static_cast<std::size_t>(dx + radius);

                kernel[index] = static_cast<float>(weight);
                weight_sum += weight;
            }
        }

        for (float &weight : kernel)
        {
            weight = static_cast<float>(weight / weight_sum);
        }

        // Convolve the mask with the kernel using periodic boundaries.
        Image2D<float> density(width, height, 0.0f);

        for (std::size_t y = 0; y < height; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                double value = 0.0;

                for (int dy = -radius; dy <= radius; ++dy)
                {
                    for (int dx = -radius; dx <= radius; ++dx)
                    {
                        const std::size_t sx =
                            wrap_coordinate(x, dx, width);
                        const std::size_t sy =
                            wrap_coordinate(y, dy, height);

                        const std::size_t index =
                            static_cast<std::size_t>(dy + radius) * side + static_cast<std::size_t>(dx + radius);

                        value += mask.at(sx, sy) * kernel[index];
                    }
                }

                density.at(x, y) = static_cast<float>(value);
            }
        }

        return density;
    }

} // namespace analysis