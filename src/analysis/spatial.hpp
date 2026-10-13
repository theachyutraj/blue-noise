#pragma once

#include "analysis/statistics.hpp"
#include "core/image.hpp"

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <vector>

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

    inline std::vector<double> radial_covariance(
        const Image2D<float> &image,
        std::ptrdiff_t max_radius)
    {
        if (max_radius < 0)
        {
            throw std::invalid_argument(
                "Maximum radius must not be negative");
        }

        const auto safe_radius = static_cast<std::size_t>(
            std::min(image.width(), image.height()) / 2);

        if (static_cast<std::size_t>(max_radius) > safe_radius)
        {
            throw std::invalid_argument(
                "Maximum radius is too large for this image");
        }

        const double average = mean(image);
        const auto radius_count =
            static_cast<std::size_t>(max_radius) + 1;

        std::vector<double> sums(radius_count, 0.0);
        std::vector<std::size_t> counts(radius_count, 0);

        for (std::ptrdiff_t dy = -max_radius;
             dy <= max_radius; ++dy)
        {
            for (std::ptrdiff_t dx = -max_radius;
                 dx <= max_radius; ++dx)
            {

                const double distance = std::sqrt(
                    static_cast<double>(dx * dx + dy * dy));

                const auto radius = static_cast<std::size_t>(
                    std::round(distance));

                if (radius > static_cast<std::size_t>(max_radius))
                {
                    continue;
                }

                                double sum = 0.0;

                for (std::size_t y = 0; y < image.height(); ++y)
                {
                    for (std::size_t x = 0; x < image.width(); ++x)
                    {
                        const auto nx =
                            wrap_coordinate(x, dx, image.width());
                        const auto ny =
                            wrap_coordinate(y, dy, image.height());

                        const double a = image.at(x, y) - average;
                        const double b = image.at(nx, ny) - average;

                        sum += a * b;
                    }
                }

                const double pixel_count =
                    static_cast<double>(image.width()) *
                    static_cast<double>(image.height());

                sums[radius] += sum / pixel_count;
                ++counts[radius];
            }
        }

        std::vector<double> result(radius_count);

        for (std::size_t r = 0; r < radius_count; ++r)
        {
            result[r] = sums[r] / static_cast<double>(counts[r]);
        }

        return result;
    }
} // namespace analysis