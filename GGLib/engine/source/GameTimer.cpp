#pragma once

#include "GameTimer.h"

std::vector<GameTimer*> GameTimer::gameTimers;

GameTimer::GameTimer(double duration, bool countDown) : timerDuration(duration), isCountingDown(countDown)
{
	resetTimer();

	gameTimers.push_back(this);

	onTimerTick = NULL;
	onTimerEnd = NULL;
}

GameTimer::~GameTimer()
{
	auto it = std::find(gameTimers.begin(), gameTimers.end(), this);
	gameTimers.erase(it);
}

void GameTimer::startTimer()
{
	t = isCountingDown ? timerDuration : 0.0f;
	isRunning = true;
	isStarted = true;
}

void GameTimer::stopTimer()
{
	t = isCountingDown ? 0.0f : timerDuration; // in case t < 0 or > timerDuration after the most recent tick
	isRunning = false;
	isStarted = false;
}

void GameTimer::resetTimer()
{
	t = isCountingDown ? timerDuration : 0.0f;
	isRunning = false;
	isStarted = false;
}

void GameTimer::pauseTimer()
{
	isRunning = false;
	if (onTimerPause) onTimerPause();
}

void GameTimer::unpauseTimer()
{
	if (isStarted)
	{
		isRunning = true;
		if (onTimerUnpause) onTimerUnpause();
	}	
}

void GameTimer::tickTimer(double deltaTime)
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

void GameTimer::setOnTimerTick(std::function<void()> func)
{
	onTimerTick = func;
}

void GameTimer::setOnTimerEnd(std::function<void()> func)
{
	onTimerEnd = func;
}

void GameTimer::setOnTimerPause(std::function<void()> func)
{
	onTimerPause = func;
}

void GameTimer::setOnTimerUnpause(std::function<void()> func)
{
	onTimerUnpause = func;
}

double GameTimer::getT() const
{
	return t;
}

double GameTimer::getTms() const
{
	return t * 1000.0f;
}

std::string  GameTimer::getTAsString() const
{
	return tToString(t);
}

double GameTimer::getPercentage() const
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

double GameTimer::getInversePercentage() const
{
	return 1 - getPercentage();
}

bool GameTimer::getIsRunning() const
{
	return isRunning;
}

bool GameTimer::getIsPaused() const
{
	return isStarted && !isRunning;
}

bool GameTimer::getIsStarted() const
{
	return isStarted;
}

double GameTimer::getDuration() const
{
	return timerDuration;
}

void GameTimer::setDuration(double newDuration)
{
	timerDuration = newDuration;
	t = isCountingDown ? timerDuration : 0.0f;
}

void GameTimer::setCountDown(bool countDown)
{
	isCountingDown = countDown;
}


std::string GameTimer::tToString(double t)
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

