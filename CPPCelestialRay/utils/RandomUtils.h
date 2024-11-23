#pragma once
#include <random>

// Define a static local random device, generator, and distribution
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> _rand01(0.0, 1.0);

// Inline function to generate a random double between 0.0 and 1.0 (exclusive)
inline float rand01() {
    return _rand01(gen);
}

// Template function to generate a random number between min and max of any type
template<typename T>
T rand(T min, T max) {
    // Ensure that the range is valid
    if (min >= max) {
        throw std::invalid_argument("Invalid range: min should be less than max");
    }

    //static std::mt19937 gen(rand01() * 4294967295u); // Seed with a random number between 0 and UINT_MAX
    std::uniform_real_distribution<T> _rand(min, max);

    return _rand(gen);
}

template<> int rand<int>(int min, int max);



// Inline function to generate a random double between 0.0 and 1.0 (exclusive)
//inline glm::vec3 vec3rand01() {
//    return glm::vec3(_rand01(gen), _rand01(gen), _rand01(gen));
//}