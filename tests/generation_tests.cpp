#include "generation/void_cluster.hpp"

#include <cassert>
#include <iostream>
#include <vector>

int main()
{
    constexpr std::size_t width = 64;
    constexpr std::size_t height = 64;
    constexpr std::size_t count = width * height;

    const auto ranks =
        generation::make_void_cluster_rank_mask(
            width, height, 42, 2.0f, 6);

    // Every rank must be in the expected range and unique.
    std::vector<bool> seen(count, false);

    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            const int rank = ranks.at(x, y);

            assert(rank >= 0);
            assert(static_cast<std::size_t>(rank) < count);
            assert(!seen[static_cast<std::size_t>(rank)]);

            seen[static_cast<std::size_t>(rank)] = true;
        }
    }

    for (bool was_seen : seen)
    {
        assert(was_seen);
    }

    std::cout << "Generation tests passed.\n";
    return 0;
}