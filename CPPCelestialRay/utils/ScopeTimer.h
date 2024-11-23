#pragma once
#include <iostream>
#include <chrono>
#include <string>

class ScopeTimer
{
public:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	std::string message = nullptr;

	ScopeTimer() : message("Time took ") {
		start = std::chrono::high_resolution_clock::now();
	}

	ScopeTimer(std::string message) : message(message) {
		start = std::chrono::high_resolution_clock::now();
	}

	~ScopeTimer() {
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		float ms = duration.count() * 1000.0f;
		std::cout << message << ms << "ms" << std::endl;
	}
};

