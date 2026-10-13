#pragma once

#include <cstdint>
#include <random>

class Random
{
public:
    explicit Random(std::uint32_t seed)
        : engine_(seed)
    {
    }

    float uniform()
    {
        return distribution_(engine_);
    }

    // Returns an integer in the inclusive range [min, max].
    int uniform_int(int min, int max)
    {
        if (min > max)
        {
            throw std::invalid_argument("min must not exceed max");
        }

        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine_);
    }

private:
    std::mt19937 engine_;
    std::uniform_real_distribution<float> distribution_{0.0f, 1.0f};
};
