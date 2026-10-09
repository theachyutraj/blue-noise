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
          pixels_(width * height, initial_value)
    {
        if (width == 0 || height == 0)
        {
            throw std::invalid_argument(
                "Image dimensions must be positive");
        }
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
        return pixels_.at(y * width_ + x);
    }

    const T &at(std::size_t x, std::size_t y) const
    {
        return pixels_.at(y * width_ + x);
    }

private:
    std::size_t width_;
    std::size_t height_;
    std::vector<T> pixels_;
};
