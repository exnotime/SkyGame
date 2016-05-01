#pragma once
#include <stdint.h>
#include <time.h>
class Timer {
public:
	Timer() {
		m_StartTime = GetTime();
	}
	~Timer() {

	}
	float Tick() {
		uint64_t now = GetTime();
		m_DeltaTime = (double)(now - m_LastTick) * 0.000000001;
		m_LastTick = now;
		return m_DeltaTime;
	}
	float GetDeltaTime() {
		return m_DeltaTime;
	}

private:
	uint64_t GetTime() {
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		return now.tv_sec * 1000000000ull + now.tv_nsec;
	}

	uint64_t m_StartTime = 0;
	uint64_t m_LastTick = 0;
	float m_DeltaTime = 0.0f;

};