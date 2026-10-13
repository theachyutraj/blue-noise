#pragma once

#include "analysis/statistics.hpp"
#include "core/image.hpp"

#include <fftw3.h>

#include <climits>
#include <cstddef>
#include <stdexcept>

namespace analysis
{

    inline Image2D<double> power_spectrum(
        const Image2D<float> &image)
    {
        const std::size_t width = image.width();
        const std::size_t height = image.height();

        if (width > static_cast<std::size_t>(INT_MAX) ||
            height > static_cast<std::size_t>(INT_MAX))
        {
            throw std::invalid_argument(
                "Image dimensions exceed FFTW's supported integer range");
        }

        const std::size_t count = width * height;
        const double average = mean(image);

        fftw_complex *input = fftw_alloc_complex(count);
        fftw_complex *output = fftw_alloc_complex(count);

        if (input == nullptr || output == nullptr)
        {
            fftw_free(input);
            fftw_free(output);
            throw std::runtime_error("Could not allocate FFT buffers");
        }

        for (std::size_t y = 0; y < height; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                const std::size_t i = y * width + x;

                input[i][0] = image.at(x, y) - average;
                input[i][1] = 0.0;
            }
        }

        fftw_plan plan = fftw_plan_dft_2d(
            static_cast<int>(height),
            static_cast<int>(width),
            input,
            output,
            FFTW_FORWARD,
            FFTW_ESTIMATE);

        if (plan == nullptr)
        {
            fftw_free(input);
            fftw_free(output);
            throw std::runtime_error("Could not create FFTW plan");
        }

        fftw_execute(plan);

        Image2D<double> power(width, height);
        const double normalization =
            static_cast<double>(count) * static_cast<double>(count);

        for (std::size_t y = 0; y < height; ++y)
        {
            for (std::size_t x = 0; x < width; ++x)
            {
                const std::size_t i = y * width + x;
                const double real = output[i][0];
                const double imag = output[i][1];

                power.at(x, y) =
                    (real * real + imag * imag) / normalization;
            }
        }

        fftw_destroy_plan(plan);
        fftw_free(input);
        fftw_free(output);

        return power;
    }

} // namespace analysis