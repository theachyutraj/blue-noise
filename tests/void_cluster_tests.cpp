#include "analysis/density.hpp"
#include "analysis/void_cluster.hpp"

#include <cassert>
#include <iostream>

int main()
{
    Image2D<float> mask(16, 16, 0.0f);

    // Two nearby points and one isolated point.
    mask.at(5, 5) = 1.0f;
    mask.at(6, 5) = 1.0f;
    mask.at(13, 13) = 1.0f;

    const auto density =
        analysis::gaussian_density(mask, 2.0f, 6);

    const auto cluster =
        analysis::find_cluster(mask, density);

    const auto void_pixel =
        analysis::find_void(mask, density);

    // The cluster must be an occupied pixel.
    assert(mask.at(cluster.x, cluster.y) == 1.0f);

    // The void must be an empty pixel.
    assert(mask.at(void_pixel.x, void_pixel.y) == 0.0f);

    std::cout << "Void-cluster tests passed.\n";
    return 0;
}