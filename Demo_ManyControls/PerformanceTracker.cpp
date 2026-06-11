#include "PerformanceTracker.h"

// WINDOWS ONLY - USED FOR GATHERING MEMORY METRICS
#include <windows.h>
#include <psapi.h>

void PerformanceTracker::recordFrame(double dtSeconds)
{
    frameTimesMs.push_back(dtSeconds * 1000.0);
}

void PerformanceTracker::sampleMemory()
{
    size_t bytes = getCurrentMemoryUsageBytes();
    double megabytes = static_cast<double>(bytes) / (1024.0 * 1024.0);
    memorySamplesMB.push_back(megabytes);
}

size_t PerformanceTracker::getCurrentMemoryUsageBytes()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        return pmc.WorkingSetSize;
    }
    return 0;
}

PerformanceTracker::BenchmarkResults PerformanceTracker::calculateMetrics()
{
    double totalTimeMs = std::accumulate(frameTimesMs.begin(), frameTimesMs.end(), 0.0);
    double avgFrameTimeMs = totalTimeMs / frameTimesMs.size();
    double avgFps = 1000.0 / avgFrameTimeMs;

    // sort frame times so longest/worst frames first
    std::vector<double> sortedTimes = frameTimesMs;
    std::sort(sortedTimes.begin(), sortedTimes.end(), std::greater<double>());

    // determine how many frames constitute 1% of total sample size
    size_t onePercentCount = static_cast<size_t>(sortedTimes.size() * 0.01);

    // sum up worst 1% frame times
    double worstOnePercentSumMs = 0.0;
    for (size_t i = 0; i < onePercentCount; i++) worstOnePercentSumMs += sortedTimes[i];

    double avgOnePercentLowTimeMs = worstOnePercentSumMs / onePercentCount;
    double onePercentLowFps = 1000.0 / avgOnePercentLowTimeMs;

    // calculate average and peak memory consumption
    double maxMem = 0;
    if (!memorySamplesMB.empty())
    {
        maxMem = *std::max_element(memorySamplesMB.begin(), memorySamplesMB.end());
    }

    // return data
    return { avgFps, onePercentLowFps,maxMem };
}
