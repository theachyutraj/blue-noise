#pragma once

#include "core/image.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace visualization
{

    inline Image2D<float> spectrum_image(
        const Image2D<double> &power)
    {
        Image2D<float> result(power.width(), power.height());

        double max_log_power = 0.0;

        // Find the maximum log-scaled power.
        for (std::size_t y = 0; y < power.height(); ++y)
        {
            for (std::size_t x = 0; x < power.width(); ++x)
            {
                const double value = power.at(x, y);

                if (!std::isfinite(value) || value < 0.0)
                {
                    throw std::invalid_argument(
                        "Power spectrum must contain finite, nonnegative values");
                }

                max_log_power = std::max(
                    max_log_power,
                    std::log1p(value));
            }
        }

        if (max_log_power == 0.0)
        {
            return result;
        }

        const std::size_t width = power.width();
        const std::size_t height = power.height();

        for (std::size_t y = 0; y < height; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                const double log_power =
                    std::log1p(power.at(x, y));

                result.at(x, y) = static_cast<float>(
                    log_power / max_log_power);
            }
        }

        return result;
    }

} // namespace visualization