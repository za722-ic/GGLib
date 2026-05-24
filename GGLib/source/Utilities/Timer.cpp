#pragma once // TODO why did I put this here? remove?

#include "GG/Utilities/Timer.h"

std::vector<Timer*> Timer::timers;

Timer::Timer(double duration, bool countDown) : timerDuration(duration), isCountingDown(countDown)
{
	resetTimer();

	timers.push_back(this);

	onTimerTick = NULL;
	onTimerEnd = NULL;
}

Timer::~Timer()
{
	auto it = std::find(timers.begin(), timers.end(), this);
	timers.erase(it);
}

void Timer::startTimer()
{
	t = isCountingDown ? timerDuration : 0.0f;
	isRunning = true;
	isStarted = true;
}

void Timer::stopTimer()
{
	t = isCountingDown ? 0.0f : timerDuration; // in case t < 0 or > timerDuration after the most recent tick
	isRunning = false;
	isStarted = false;
}

void Timer::resetTimer()
{
	t = isCountingDown ? timerDuration : 0.0f;
	isRunning = false;
	isStarted = false;
}

void Timer::pauseTimer()
{
	isRunning = false;
	if (onTimerPause) onTimerPause();
}

void Timer::unpauseTimer()
{
	if (isStarted)
	{
		isRunning = true;
		if (onTimerUnpause) onTimerUnpause();
	}	
}

void Timer::tickTimer(double deltaTime)
{
	if (isRunning) t += isCountingDown ? -deltaTime : deltaTime;
	else return;

	if (t <= 0.0f && isCountingDown || t >= timerDuration && !isCountingDown)
	{
		stopTimer();
		if (onTimerEnd) onTimerEnd();
	}
	else
	{
		if (onTimerTick) onTimerTick(); // https://stackoverflow.com/questions/21806632/how-to-properly-check-if-stdfunction-is-empty-in-c11
	}
}

void Timer::setOnTimerTick(std::function<void()> func)
{
	onTimerTick = func;
}

void Timer::setOnTimerEnd(std::function<void()> func)
{
	onTimerEnd = func;
}

void Timer::setOnTimerPause(std::function<void()> func)
{
	onTimerPause = func;
}

void Timer::setOnTimerUnpause(std::function<void()> func)
{
	onTimerUnpause = func;
}

double Timer::getT() const
{
	return t;
}

double Timer::getTms() const
{
	return t * 1000.0f;
}

std::string  Timer::getTAsString() const
{
	return tToString(t);
}

double Timer::getPercentage() const
{
	if (isCountingDown)
	{
		return 1 - t / timerDuration;
	}
	else
	{
		return t / timerDuration;
	}
}

double Timer::getInversePercentage() const
{
	return 1 - getPercentage();
}

bool Timer::getIsRunning() const
{
	return isRunning;
}

bool Timer::getIsPaused() const
{
	return isStarted && !isRunning;
}

bool Timer::getIsStarted() const
{
	return isStarted;
}

double Timer::getDuration() const
{
	return timerDuration;
}

void Timer::setDuration(double newDuration)
{
	timerDuration = newDuration;
	t = isCountingDown ? timerDuration : 0.0f;
}

void Timer::setCountDown(bool countDown)
{
	isCountingDown = countDown;
}


std::string Timer::tToString(double t)
{
	// e.g. if t = 123.1222131 then return "2:03.122"
	// probably should generalise this at some point for different precisions, and hours & days

	int ms = (int)(1000.0f * (t - (int)t));   // e.g. 122
	int min = (int)t / 60;                    // e.g. 2
	int s = (int)t - min * 60;                // e.g. 3

	std::ostringstream outStr;
	outStr << min << ":" << std::setfill('0') << std::setw(2) << s << "." << std::setw(3) << ms;

	return outStr.str();
}

