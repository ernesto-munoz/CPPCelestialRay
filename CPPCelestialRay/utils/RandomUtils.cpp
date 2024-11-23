#include "RandomUtils.h"

template<>
int rand<int>(int min, int max) {
    // Ensure that the range is valid
    if (min >= max) {
        throw std::invalid_argument("Invalid range: min should be less than max");
    }

    //static std::mt19937 gen(rand01() * 4294967295u); // Seed with a random number between 0 and UINT_MAX
    std::uniform_int_distribution<> _rand(min, max);

    return _rand(gen);
}