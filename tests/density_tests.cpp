#include "analysis/density.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

int main()
{
    // A 32x32 mask with exactly one occupied pixel.
    Image2D<float> mask(32, 32, 0.0f);
    mask.at(16, 16) = 1.0f;

    const auto density =
        analysis::gaussian_density(mask, 2.0f, 6);

    // The density should be strongest at the occupied pixel.
    assert(density.at(16, 16) > density.at(17, 16));
    assert(density.at(17, 16) > density.at(19, 16));
    assert(density.at(19, 16) > density.at(22, 16));

    // Symmetry: equally distant pixels should have equal density.
    assert(std::abs(
               density.at(15, 16) - density.at(17, 16)) < 1e-6f);

    // The density should be positive near the point.
    assert(density.at(16, 16) > 0.0f);

    std::cout << "Density tests passed.\n";
    return 0;
}