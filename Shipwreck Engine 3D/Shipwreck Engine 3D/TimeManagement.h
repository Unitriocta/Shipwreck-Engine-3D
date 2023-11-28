#pragma once
#include <ctime>

class Time {

public:
	clock_t _time;
public:

	void StartTime() {
		_time = clock();
	}


	float GetTime() {
		return (clock() - _time);
	}
};