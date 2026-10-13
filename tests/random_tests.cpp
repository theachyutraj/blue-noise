#include "core/random.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main()
{
    Random first(42);
    Random second(42);
    Random different(123);

    // Identical seeds produce identical sequences.
    for (int i = 0; i < 100; ++i)
    {
        assert(first.uniform() == second.uniform());
    }

    // Integer generation is reproducible too.
    Random int_first(7);
    Random int_second(7);

    for (int i = 0; i < 100; ++i)
    {
        assert(int_first.uniform_int(0, 10) ==
               int_second.uniform_int(0, 10));
    }

    // Invalid integer ranges must be rejected.
    bool rejected = false;

    try
    {
        int_first.uniform_int(10, 0);
    }
    catch (const std::invalid_argument &)
    {
        rejected = true;
    }

    assert(rejected);

    // Different seeds should generally produce different sequences.
    bool sequences_differ = false;

    for (int i = 0; i < 100; ++i)
    {
        if (different.uniform() != second.uniform())
        {
            sequences_differ = true;
            break;
        }
    }

    assert(sequences_differ);

    std::cout << "All Random tests passed.\n";
}