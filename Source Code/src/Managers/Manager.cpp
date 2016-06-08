#include "Manager.h"

Manager::Manager()
{
	this->passesCounter = 0;
	this->profiling = 0;
	this->currentMs = 0;
	this->averageMs = 0;
}

Manager::~Manager()
{
	
}

void Manager::setCurrentMs(double _value)
{
	currentMs = _value;		
}

void Manager::setAverageMs(double _value)
{
	if(passesCounter == 1)
		averageMs = _value;
	else
		averageMs = averageMs / passesCounter * (passesCounter -1) + _value / passesCounter;
}

void Manager::setProfiling(double _value)
{
	profiling = _value;
}

void Manager::setPassesCounter(double _value)
{
	passesCounter = _value;
}

void Manager::increasePassesCounter()
{
	passesCounter++;
}

void Manager::decreaseProfiling()
{
	profiling--;
}

double Manager::getCurrentMs()
{
	return currentMs;
}

double Manager::getAverageMs()
{
	return averageMs;
}

double Manager::getProfiling()
{
	return profiling;
}

double Manager::getPassesCounter()
{
	return passesCounter;
}