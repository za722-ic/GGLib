#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>

class GameTimer
{
public:
	static std::vector<GameTimer*> gameTimers;

private:
	double timerDuration;
	double t;

	bool isRunning;
	bool isStarted;

	bool isCountingDown;

	std::function<void()> onTimerTick;
	std::function<void()> onTimerEnd;
	std::function<void()> onTimerPause;
	std::function<void()> onTimerUnpause;

public:
	/*
	Use this if you want the timer to count indefinetly
	*/
	GameTimer(bool countDown = false) : GameTimer(countDown ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity(), countDown)
	{}


	/*
	Duration (double) - How long the timer should run.
	countDown (bool - Set true if t should decrease over time. Otherwise t will increase over time.
	*/
	GameTimer(double duration, bool countDown = true);

	~GameTimer();

	void startTimer();
	void stopTimer();
	void resetTimer();
	void pauseTimer();
	void unpauseTimer();


	void tickTimer(double deltaTime); // called every frame

	void setOnTimerTick(std::function<void()> func);
	void setOnTimerEnd(std::function<void()> func);
	void setOnTimerPause(std::function<void()> func);
	void setOnTimerUnpause(std::function<void()> func);

	double getT() const;
	double getTms() const; // t in milliseconds
	std::string getTAsString() const;
	double getPercentage() const;
	double getInversePercentage() const;
	bool getIsRunning() const;
	bool getIsStarted() const;
	bool getIsPaused() const;
	double getDuration() const;



	void setDuration(double newDuration);
	void setCountDown(bool countDown);


	static std::string tToString(double t);

private:
	// This is here cus Rule Of 3: https://stackoverflow.com/questions/9139103/am-i-violating-rule-of-three
	// Copying timers would be pretty cool though, so if you ever need that then change these functions
	GameTimer(GameTimer const&) = delete;
	GameTimer& operator=(GameTimer const&) = delete;
};