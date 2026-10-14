#pragma once

#include "core/image.hpp"

#include <cstddef>
#include <stdexcept>

namespace analysis
{

    struct PixelPosition
    {
        std::size_t x;
        std::size_t y;
    };

    inline PixelPosition find_cluster(
        const Image2D<float> &mask,
        const Image2D<float> &density)
    {
        if (mask.width() != density.width() ||
            mask.height() != density.height())
        {
            throw std::invalid_argument(
                "Mask and density dimensions must match");
        }

        bool found = false;
        PixelPosition best{0, 0};
        float highest_density = 0.0f;

        for (std::size_t y = 0; y < mask.height(); ++y)
        {
            for (std::size_t x = 0; x < mask.width(); ++x)
            {
                if (mask.at(x, y) <= 0.5f)
                {
                    continue; // Only occupied pixels are candidates.
                }

                if (!found || density.at(x, y) > highest_density)
                {
                    found = true;
                    highest_density = density.at(x, y);
                    best = {x, y};
                }
            }
        }

        if (!found)
        {
            throw std::invalid_argument(
                "Cannot find a cluster in an empty mask");
        }

        return best;
    }

    inline PixelPosition find_void(
        const Image2D<float> &mask,
        const Image2D<float> &density)
    {
        if (mask.width() != density.width() ||
            mask.height() != density.height())
        {
            throw std::invalid_argument(
                "Mask and density dimensions must match");
        }

        bool found = false;
        PixelPosition best{0, 0};
        float lowest_density = 0.0f;

        for (std::size_t y = 0; y < mask.height(); ++y)
        {
            for (std::size_t x = 0; x < mask.width(); ++x)
            {
                if (mask.at(x, y) > 0.5f)
                {
                    continue; // Only empty pixels are candidates.
                }

                if (!found || density.at(x, y) < lowest_density)
                {
                    found = true;
                    lowest_density = density.at(x, y);
                    best = {x, y};
                }
            }
        }

        if (!found)
        {
            throw std::invalid_argument(
                "Cannot find a void in a full mask");
        }

        return best;
    }

} // namespace analysis