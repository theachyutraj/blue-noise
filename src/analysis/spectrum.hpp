#pragma once

#include "core/image.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <vector>

namespace analysis
{

    inline std::vector<double> radial_power_spectrum(
        const Image2D<double> &power)
    {
        const std::size_t width = power.width();
        const std::size_t height = power.height();

        // Maximum radius that fits within the centered spectrum.
        const std::size_t max_radius =
            std::min(width, height) / 2;

        std::vector<double> sums(max_radius + 1, 0.0);
        std::vector<std::size_t> counts(max_radius + 1, 0);

        // FFT frequencies are assumed to have been shifted to the center.
        const auto center_x = width / 2;
        const auto center_y = height / 2;

        for (std::size_t y = 0; y < height; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                const auto dx = static_cast<std::ptrdiff_t>(x) -
                                static_cast<std::ptrdiff_t>(center_x);
                const auto dy = static_cast<std::ptrdiff_t>(y) -
                                static_cast<std::ptrdiff_t>(center_y);

                const auto radius = static_cast<std::size_t>(
                    std::round(std::hypot(
                        static_cast<double>(dx),
                        static_cast<double>(dy))));

                if (radius > max_radius)
                    continue;

                sums[radius] += power.at(x, y);
                ++counts[radius];
            }
        }

        for (std::size_t r = 0; r <= max_radius; ++r)
        {
            if (counts[r] > 0)
            {
                sums[r] /= static_cast<double>(counts[r]);
            }
        }

        return sums;
    }

    inline Image2D<double> center_spectrum(
        const Image2D<double> &power)
    {
        const std::size_t width = power.width();
        const std::size_t height = power.height();

        Image2D<double> centered(width, height);

        for (std::size_t y = 0; y < height; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                const std::size_t source_x =
                    (x + (width + 1) / 2) % width;

                const std::size_t source_y =
                    (y + (height + 1) / 2) % height;

                centered.at(x, y) = power.at(source_x, source_y);
            }
        }

        return centered;
    }
} // namespace analysis