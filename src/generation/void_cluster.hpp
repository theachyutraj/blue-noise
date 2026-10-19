#pragma once

#include "analysis/density.hpp"
#include "analysis/void_cluster.hpp"
#include "core/image.hpp"
#include "core/random.hpp"

#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace generation
{

    inline std::vector<float> make_gaussian_kernel(
        float sigma,
        int radius)
    {
        if (!std::isfinite(sigma) || sigma <= 0.0f)
        {
            throw std::invalid_argument(
                "Sigma must be positive and finite");
        }

        if (radius < 0 || radius > (INT_MAX - 1) / 2)
        {
            throw std::invalid_argument("Invalid kernel radius");
        }

        const int side = 2 * radius + 1;
        std::vector<float> kernel(
            static_cast<std::size_t>(side) * side);

        double sum = 0.0;

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
                sum += weight;
            }
        }

        for (float &weight : kernel)
        {
            weight = static_cast<float>(weight / sum);
        }

        return kernel;
    }

    // Update the density field after adding or removing one point.
    // delta = +1 when adding, -1 when removing.
    inline void update_density(
        Image2D<float> &density,
        const std::vector<float> &kernel,
        std::size_t px,
        std::size_t py,
        int radius,
        float delta)
    {
        const std::size_t width = density.width();
        const std::size_t height = density.height();
        const int side = 2 * radius + 1;

        for (int dy = -radius; dy <= radius; ++dy)
        {
            for (int dx = -radius; dx <= radius; ++dx)
            {
                // The point at (px, py) affects the density
                // at neighboring locations, with periodic wrapping.
                const auto qx = analysis::wrap_coordinate(
                    px, -static_cast<std::ptrdiff_t>(dx), width);
                const auto qy = analysis::wrap_coordinate(
                    py, -static_cast<std::ptrdiff_t>(dy), height);

                const std::size_t index =
                    static_cast<std::size_t>(dy + radius) * side + static_cast<std::size_t>(dx + radius);

                density.at(qx, qy) += delta * kernel[index];
            }
        }
    }

    // Returns a rank image containing each integer from
    // 0 through width*height - 1 exactly once.
    inline Image2D<int> make_void_cluster_rank_mask(
        std::size_t width,
        std::size_t height,
        std::uint32_t seed,
        float sigma = 2.0f,
        int radius = 6)
    {
        // Image2D validates that the dimensions and area are valid.
        Image2D<float> mask(width, height, 0.0f);

        const std::size_t count = width * height;

        if (count > static_cast<std::size_t>(INT_MAX))
        {
            throw std::invalid_argument(
                "Rank mask is too large for integer ranks");
        }

        if (count < 2 || count % 2 != 0)
        {
            throw std::invalid_argument(
                "Rank mask requires an even pixel count of at least 2");
        }

        const auto kernel = make_gaussian_kernel(sigma, radius);

        // Create exactly half occupancy, in randomized positions.
        std::vector<std::size_t> positions(count);
        for (std::size_t i = 0; i < count; ++i)
        {
            positions[i] = i;
        }

        Random rng(seed);

        // Fisher-Yates shuffle.
        for (std::size_t i = count - 1; i > 0; --i)
        {
            const auto j = static_cast<std::size_t>(
                rng.uniform_int(0, static_cast<int>(i)));

            const auto temp = positions[i];
            positions[i] = positions[j];
            positions[j] = temp;
        }

        const std::size_t occupied_target = count / 2;

        for (std::size_t i = 0; i < occupied_target; ++i)
        {
            const std::size_t p = positions[i];
            mask.at(p % width, p / width) = 1.0f;
        }

        auto density = analysis::gaussian_density(
            mask, sigma, radius);

        Image2D<int> ranks(width, height, -1);

        // Phase 1: remove clusters and assign high ranks.
        // Keep one occupied pixel as the seed for void filling.
        std::size_t occupied = occupied_target;
        int next_cluster_rank = static_cast<int>(count) - 1;

        while (occupied > 1)
        {
            const auto cluster =
                analysis::find_cluster(mask, density);

            ranks.at(cluster.x, cluster.y) = next_cluster_rank--;
            mask.at(cluster.x, cluster.y) = 0.0f;

            update_density(
                density, kernel, cluster.x, cluster.y,
                radius, -1.0f);

            --occupied;
        }

        // The remaining seed receives the middle rank.
        bool seed_found = false;

        for (std::size_t y = 0; y < height && !seed_found; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                if (mask.at(x, y) > 0.5f)
                {
                    ranks.at(x, y) = static_cast<int>(occupied_target);
                    seed_found = true;
                    break;
                }
            }
        }

        // Reset the mask and density to original state for void filling.
        for (std::size_t i = 0; i < occupied_target; ++i)
        {
            const std::size_t p = positions[i];
            mask.at(p % width, p / width) = 1.0f;
        }

        density = analysis::gaussian_density(mask, sigma, radius);

        // Phase 2: fill voids and assign low ranks.
        int next_void_rank = 0;

        while (next_void_rank < occupied_target)
        {
            const auto void_pixel =
                analysis::find_void(mask, density);

            ranks.at(void_pixel.x, void_pixel.y) = next_void_rank++;

            mask.at(void_pixel.x, void_pixel.y) = 1.0f;

            update_density(
                density, kernel, void_pixel.x, void_pixel.y,
                radius, +1.0f);
        }

        return ranks;
    }

} // namespace generation