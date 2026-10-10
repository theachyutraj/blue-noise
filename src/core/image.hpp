#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
class Image2D
{
public:
    Image2D(std::size_t width, std::size_t height, T initial_value = T{})
        : width_(width),
          height_(height),
          pixels_(checked_area(width, height), initial_value)
    {
    }

    std::size_t width() const
    {
        return width_;
    }

    std::size_t height() const
    {
        return height_;
    }

    T &at(std::size_t x, std::size_t y)
    {
        if (x >= width_ || y >= height_)
        {
            throw std::out_of_range("Image coordinates out of bounds");
        }

        return pixels_[y * width_ + x];
    }

    const T &at(std::size_t x, std::size_t y) const
    {
        if (x >= width_ || y >= height_)
        {
            throw std::out_of_range("Image coordinates out of bounds");
        }

        return pixels_[y * width_ + x];
    }

private:
    static std::size_t checked_area(
        std::size_t width,
        std::size_t height)
    {
        if (width == 0 || height == 0)
        {
            throw std::invalid_argument(
                "Image dimensions must be positive");
        }

        if (width > std::numeric_limits<std::size_t>::max() / height)
        {
            throw std::length_error(
                "Image dimensions are too large");
        }

        return width * height;
    }

    std::size_t width_;
    std::size_t height_;
    std::vector<T> pixels_;
};
