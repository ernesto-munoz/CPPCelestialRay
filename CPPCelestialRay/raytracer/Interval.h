#pragma once
#include <iostream>
#include <glm/glm.hpp> 

//const double INFINITY = std::numeric_limits<double>::infinity();

class Interval
{
public:
	double min, max;

	Interval() : min(INFINITY), max(-INFINITY) {}
	Interval(double min, double max) : min(min), max(max) {}
	Interval(const Interval& a, const Interval& b) {
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= b.max ? a.max : b.max;
	}

	double Size() const {
		return max - min;
	}

	bool Contains(double x) const {
		return min <= x && x <= max;
	}

	bool Surrounds(double x) const {
		return min < x && x < max;
	}

	Interval Expand(float delta) const {
		auto padding = delta / 2;
		return Interval(min - padding, max + padding);

	}

	static const Interval empty, universe;
};

Interval operator+(const Interval& ival, float displacement);
Interval operator+(float displacement, const Interval& ival);