#pragma once

#include <chrono>


class Timer {
private:
	// panaudojame using
	using hrClock = std::chrono::high_resolution_clock;
	using durationDouble = std::chrono::duration<double>;
	using tmPt = std::chrono::time_point<hrClock>;

	//variables
	tmPt start_;
	durationDouble duration_{ 0 }, null_duration_{ 0 };

	inline void addDuration_() { duration_ += hrClock::now() - start_; }
public:
	//default constructor
	Timer() : start_{ hrClock::now() } {}

	//functions
	inline void reset() {
		start_ = hrClock::now();
		duration_ = null_duration_;
	}
	inline double elapsed() {
		addDuration_();
		return duration_.count();
	}
	inline void pause() {
		addDuration_();
	}
	inline void resume() {
		start_ = hrClock::now();
	}
};