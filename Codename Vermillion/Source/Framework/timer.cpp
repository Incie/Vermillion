#include"pch.h"
#include "timer.h"
#include<chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::time_point_cast;
using std::chrono::microseconds;


Timer::Timer()
	: delta(0.0),
	timelast(NowAsMicroseconds()),
	timestep(16000),
	one_second(1000000.0),
	one_second_long(1000000)
{
}

Timer::~Timer()
{
}

bool Timer::Tick()
{
	long long now = NowAsMicroseconds();

	long long timedelta = now - timelast;
	if ( timedelta >= timestep) {
		delta = (double)timedelta / one_second;
		timelast = now;
		return true;
	}
	return false;
}

double Timer::GetDelta()
{
	return delta;
}

void Timer::LimitByFPS(int fps)
{
	if(fps == 0)
		fps = 1;

	timestep = one_second_long / fps;
}

void Timer::LimitByMicroseconds(int us)
{
	timestep = us;
}

void Timer::LimitByMilliseconds(int ms)
{
	timestep = ms * 1000;
}

long long Timer::NowAsMicroseconds()
{
	auto t0 = high_resolution_clock::now();

	auto time_us = time_point_cast<microseconds>(t0).time_since_epoch();
	auto value = duration_cast<microseconds>(time_us);

	long long longvalue = value.count();
	return longvalue;
}
