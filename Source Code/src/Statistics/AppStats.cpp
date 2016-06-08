#include "AppStats.h"

AppStats::AppStats()
{}

AppStats::~AppStats()
{}

void AppStats::setInitialGoalMs(double _value)
{
	initialGoalMs = _value;	
}

void AppStats::setOffsetGoalMs(double _value)
{
	offsetGoalMs = _value;	
}

void AppStats::setCurrentMs(double _value)
{
	currentMs = _value;
}

void AppStats::setAverageMs(double _value)
{
	if(passesCounter == 1)
		averageMs = _value;	
	else
		averageMs = averageMs / passesCounter * (passesCounter - 1) + _value / passesCounter;	
}

void AppStats::setMinimumGoalMs(double _value)
{
	minGoalMs = _value;	
}

void AppStats::setMaximumGoalMs(double _value)
{
	maxGoalMs = _value;	
}

void AppStats::setCurrentError(double _value)
{
	currentError = std::abs(initialGoalMs + offsetGoalMs - _value);	
}

void AppStats::setAverageError(double _value)
{
	if(passesCounter == 1)
		averageError = std::abs(initialGoalMs + offsetGoalMs - _value);	
	else
		averageError = averageError / passesCounter * (passesCounter - 1) + std::abs(initialGoalMs + offsetGoalMs - _value) / passesCounter;
}

void AppStats::setPassesCounter(double _value)
{
	passesCounter = _value;	
}

void AppStats::increasePassesCounter()
{
	passesCounter++;
}

double AppStats::getInitialGoalMs(void)
{
	return initialGoalMs;
}

double AppStats::getOffsetGoalMs(void)
{
	return offsetGoalMs;
}

double AppStats::getCurrentMs(void)
{
	return currentMs;
}

double AppStats::getAverageMs(void)
{
	return averageMs;
}

double AppStats::getMinimumGoalMs(void)
{
	return minGoalMs;
}

double AppStats::getMaximumGoalMs(void)
{
	return maxGoalMs;
}

double AppStats::getCurrentError(void)
{
	return currentError;
}

double AppStats::getAverageError(void)
{
	return averageError;
}

double AppStats::getPassesCounter(void)
{
	return passesCounter;
}

double AppStats::getNextGoalDeviation(double _value)
{
	return averageError / passesCounter * (passesCounter - 1) + std::abs(initialGoalMs + offsetGoalMs - _value) / passesCounter;
	
	
}