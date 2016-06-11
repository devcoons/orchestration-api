#include "AppTracker.h"

AppTracker::AppTracker()
{}

AppTracker::~AppTracker()
{}

void AppTracker::setInitialGoalMs(double _value)
{
	initialGoalMs = _value;	
}

void AppTracker::setOffsetGoalMs(double _value)
{
	offsetGoalMs = _value;	
}

void AppTracker::setCurrentMs(double _value)
{
	currentMs = _value;
}

void AppTracker::setAverageMs(double _value)
{
	if(passesCounter <= 1)
		averageMs = _value;	
	else
		averageMs = (averageMs / passesCounter) * (passesCounter - 1) + _value / passesCounter;	
}

void AppTracker::setMinimumGoalMs(double _value)
{
	minGoalMs = _value;	
}

void AppTracker::setMaximumGoalMs(double _value)
{
	maxGoalMs = _value;	
}

void AppTracker::updateCurrentError(void)
{
	currentError = std::fabs((initialGoalMs + offsetGoalMs) - currentMs);	
}

void AppTracker::updateAverageError(void)
{
	averageError =  std::fabs((initialGoalMs + offsetGoalMs) - averageMs);
}

void AppTracker::setPassesCounter(double _value)
{
	passesCounter = _value;	
}

void AppTracker::increasePassesCounter()
{
	passesCounter++;
}

double AppTracker::getInitialGoalMs(void)
{
	return initialGoalMs;
}

double AppTracker::getOffsetGoalMs(void)
{
	return offsetGoalMs;
}

double AppTracker::getCurrentMs(void)
{
	return currentMs;
}

double AppTracker::getAverageMs(void)
{
	return averageMs;
}

double AppTracker::getMinimumGoalMs(void)
{
	return minGoalMs;
}

double AppTracker::getMaximumGoalMs(void)
{
	return maxGoalMs;
}

double AppTracker::getCurrentError(void)
{
	return currentError;
}

double AppTracker::getAverageError(void)
{
	return averageError;
}

double AppTracker::getPassesCounter(void)
{
	return passesCounter;
}

double AppTracker::getNextGoalDeviation(double _value)
{
	return std::fabs((((initialGoalMs + offsetGoalMs -averageMs) / (passesCounter+1)) * (passesCounter)) +  (initialGoalMs + offsetGoalMs - _value) / (passesCounter+1));	
}