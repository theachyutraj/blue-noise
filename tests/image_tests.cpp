#include "core/image.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main()
{
    // Basic construction and access.
    Image2D<int> image(4, 3, 0);

    assert(image.width() == 4);
    assert(image.height() == 3);

    image.at(2, 1) = 42;
    assert(image.at(2, 1) == 42);

    // Zero dimensions must be rejected.
    bool zero_rejected = false;

    try
    {
        Image2D<int> invalid(0, 3);
    }
    catch (const std::invalid_argument &)
    {
        zero_rejected = true;
    }

    assert(zero_rejected);

    // Out-of-bounds coordinates must be rejected.
    bool bounds_rejected = false;

    try
    {
        image.at(4, 1);
    }
    catch (const std::out_of_range &)
    {
        bounds_rejected = true;
    }

    assert(bounds_rejected);

    std::cout << "All Image2D tests passed.\n";
}