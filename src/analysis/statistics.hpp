#pragma once

#include "core/image.hpp"

#include <cstddef>
#include <vector>
#include <stdexcept>

namespace analysis
{

    inline double mean(const Image2D<float> &image)
    {
        double sum = 0.0;

        for (std::size_t y = 0; y < image.height(); ++y)
        {
            for (std::size_t x = 0; x < image.width(); ++x)
            {
                sum += image.at(x, y);
            }
        }

        const double count =
            static_cast<double>(image.width()) *
            static_cast<double>(image.height());

        return sum / count;
    }

    inline double variance(const Image2D<float> &image)
    {
        const double average = mean(image);
        double squared_deviations = 0.0;

        for (std::size_t y = 0; y < image.height(); ++y)
        {
            for (std::size_t x = 0; x < image.width(); ++x)
            {
                const double difference = image.at(x, y) - average;
                squared_deviations += difference * difference;
            }
        }

        const double count =
            static_cast<double>(image.width()) *
            static_cast<double>(image.height());

        return squared_deviations / count;
    }

    inline std::vector<std::size_t> histogram(
        const Image2D<float> &image,
        std::size_t bin_count)
    {
        if (bin_count == 0)
        {
            throw std::invalid_argument(
                "Histogram must have at least one bin");
        }

        std::vector<std::size_t> bins(bin_count, 0);

        for (std::size_t y = 0; y < image.height(); ++y)
        {
            for (std::size_t x = 0; x < image.width(); ++x)
            {
                const float pixel = image.at(x, y);

                // Map [0, 1] to a bin index.
                std::size_t index =
                    static_cast<std::size_t>(pixel * bin_count);

                // A pixel exactly equal to 1 belongs in the last bin.
                if (index >= bin_count)
                {
                    index = bin_count - 1;
                }

                ++bins[index];
            }
        }

        return bins;
    }
} // namespace analysis
