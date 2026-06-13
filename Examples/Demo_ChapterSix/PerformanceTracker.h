#pragma once

#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>

class PerformanceTracker
{
private:
	std::vector<double> frameTimesMs;
	std::vector<double> memorySamplesMB;

public:
	struct BenchmarkResults
	{
		double averageFps;
		double onePercentLowFps;
		double maxMem;
	};

public:
	void recordFrame(double dt_seconds);
	void sampleMemory();

	BenchmarkResults calculateMetrics();

private:
	size_t getCurrentMemoryUsageBytes();
};
