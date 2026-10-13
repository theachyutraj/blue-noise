#pragma once

#include "core/image.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

namespace visualization
{

    inline void save_pgm(
        const Image2D<float> &image,
        const std::string &filename)
    {
        std::ofstream file(filename);

        if (!file)
        {
            throw std::runtime_error("Could not open output file: " + filename);
        }

        file << "P2\n";
        file << image.width() << " " << image.height() << "\n";
        file << "255\n";

        for (std::size_t y = 0; y < image.height(); ++y)
        {
            for (std::size_t x = 0; x < image.width(); ++x)
            {
                const float pixel = image.at(x, y);

                if (!std::isfinite(pixel))
                {
                    throw std::invalid_argument(
                        "Cannot save non-finite pixel values");
                }

                const float clamped = std::clamp(pixel, 0.0f, 1.0f);
                const int value = static_cast<int>(clamped * 255.0f);

                file << value << ' ';
            }

            file << '\n';
        }

        if (!file)
        {
            throw std::runtime_error("Failed while writing output file: " + filename);
        }
    }

} // namespace visualization
