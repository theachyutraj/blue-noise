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

private:
    std::mt19937 engine_;
    std::uniform_real_distribution<float> distribution_{0.0f, 1.0f};
};
