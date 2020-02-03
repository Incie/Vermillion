#pragma once


class Timer
{
public:
	Timer();
	~Timer();

	bool Tick();
	double GetDelta();

	double TimeSinceTickAsMilliseconds();

	void LimitByFPS(int fps);
	void LimitByMicroseconds(int us);
	void LimitByMilliseconds(int ms);
private:
	long long NowAsMicroseconds();

	double delta;

	long long timestepMicroSeconds;
	long long timelast;

	const double one_second;
	const long long one_second_long;
};