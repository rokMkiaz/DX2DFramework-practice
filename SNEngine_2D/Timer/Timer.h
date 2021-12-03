#pragma once

class Timer final
{
public:


private:
};

class Stopwatch final
{
public:
	Stopwatch();
	~Stopwatch() = default;

	void Start();

	auto GetElapsedTimeSec() -> const float;
	auto GetElapsedTimeMs() -> const float;
private:
	std::chrono::time_point<std::chrono::high_resolution_clock>start_time;

};