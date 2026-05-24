#pragma once // TODO why did I put this here? remove?

#include "GG/Utilities/Timer.h"

std::vector<GG::Timer*> GG::Timer::timers;

GG::Timer::Timer(double duration, bool countDown) : timerDuration(duration), isCountingDown(countDown)
{
	resetTimer();

	timers.push_back(this);

	onTimerTick = NULL;
	onTimerEnd = NULL;
}

GG::Timer::~Timer()
{
	auto it = std::find(timers.begin(), timers.end(), this);
	timers.erase(it);
}

void GG::Timer::startTimer()
{
	t = isCountingDown ? timerDuration : 0.0f;
	isRunning = true;
	isStarted = true;
}

void GG::Timer::stopTimer()
{
	t = isCountingDown ? 0.0f : timerDuration; // in case t < 0 or > timerDuration after the most recent tick
	isRunning = false;
	isStarted = false;
}

void GG::Timer::resetTimer()
{
	t = isCountingDown ? timerDuration : 0.0f;
	isRunning = false;
	isStarted = false;
}

void GG::Timer::pauseTimer()
{
	isRunning = false;
	if (onTimerPause) onTimerPause();
}

void GG::Timer::unpauseTimer()
{
	if (isStarted)
	{
		isRunning = true;
		if (onTimerUnpause) onTimerUnpause();
	}
}

void GG::Timer::tickTimer(double deltaTime)
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

void GG::Timer::setOnTimerTick(std::function<void()> func)
{
	onTimerTick = func;
}

void GG::Timer::setOnTimerEnd(std::function<void()> func)
{
	onTimerEnd = func;
}

void GG::Timer::setOnTimerPause(std::function<void()> func)
{
	onTimerPause = func;
}

void GG::Timer::setOnTimerUnpause(std::function<void()> func)
{
	onTimerUnpause = func;
}

double GG::Timer::getT() const
{
	return t;
}

double GG::Timer::getTms() const
{
	return t * 1000.0f;
}

std::string GG::Timer::getTAsString() const
{
	return tToString(t);
}

double GG::Timer::getPercentage() const
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

double GG::Timer::getInversePercentage() const
{
	return 1 - getPercentage();
}

bool GG::Timer::getIsRunning() const
{
	return isRunning;
}

bool GG::Timer::getIsPaused() const
{
	return isStarted && !isRunning;
}

bool GG::Timer::getIsStarted() const
{
	return isStarted;
}

double GG::Timer::getDuration() const
{
	return timerDuration;
}

void GG::Timer::setDuration(double newDuration)
{
	timerDuration = newDuration;
	t = isCountingDown ? timerDuration : 0.0f;
}

void GG::Timer::setCountDown(bool countDown)
{
	isCountingDown = countDown;
}


std::string GG::Timer::tToString(double t)
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
