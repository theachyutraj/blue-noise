#pragma once

#include "analysis/statistics.hpp"
#include "core/image.hpp"

#include <cstddef>
#include <stdexcept>

namespace analysis
{

    // Wrap a coordinate onto a periodic axis.
    inline std::size_t wrap_coordinate(
        std::size_t coordinate,
        std::ptrdiff_t offset,
        std::size_t extent)
    {
        const auto signed_extent =
            static_cast<std::ptrdiff_t>(extent);

        const auto shifted =
            (static_cast<std::ptrdiff_t>(coordinate) + offset) % signed_extent;

        if (shifted < 0)
        {
            return static_cast<std::size_t>(shifted + signed_extent);
        }

        return static_cast<std::size_t>(shifted);
    }

    // Covariance between pixels separated by (dx, dy),
    // using toroidal (periodic) boundary conditions.
    inline double covariance(
        const Image2D<float> &image,
        std::ptrdiff_t dx,
        std::ptrdiff_t dy)
    {
        const double average = mean(image);
        double sum = 0.0;

        for (std::size_t y = 0; y < image.height(); ++y)
        {
            for (std::size_t x = 0; x < image.width(); ++x)
            {
                const std::size_t nx =
                    wrap_coordinate(x, dx, image.width());

                const std::size_t ny =
                    wrap_coordinate(y, dy, image.height());

                const double a = image.at(x, y) - average;
                const double b = image.at(nx, ny) - average;

                sum += a * b;
            }
        }

        const double count =
            static_cast<double>(image.width()) *
            static_cast<double>(image.height());

        return sum / count;
    }

} // namespace analysis