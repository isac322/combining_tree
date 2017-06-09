#pragma once

#include <chrono>

class Timer {
private:
	typedef std::chrono::high_resolution_clock clock;
	typedef std::chrono::duration<double, std::ratio<1>> seconds;
	std::chrono::time_point<clock> begin;

public:
	Timer() : begin(clock::now()) {}
	
	void reset() { begin = clock::now(); }
	
	double elapsed() const {
		return std::chrono::duration_cast<seconds>(clock::now() - begin).count();
	}
};