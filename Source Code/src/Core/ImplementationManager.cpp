#include "ImplementationManager.h"

ImplementationManager::ImplementationManager()
{
	this->passesCounter = 0;
	this->profiling = 0;
	this->currentMs = 0;
	this->averageMs = 0;
}

ImplementationManager::~ImplementationManager()
{
	
}

void ImplementationManager::setCurrentMs(double _value)
{
	currentMs = _value;		
}

void ImplementationManager::setAverageMs(double _value)
{
	if(passesCounter <= 1)
		averageMs = _value;
	else
		averageMs = averageMs / passesCounter * (passesCounter -1) + _value / passesCounter;
}

void ImplementationManager::setProfiling(double _value)
{
	profiling = _value;
}

void ImplementationManager::setPassesCounter(double _value)
{
	passesCounter = _value;
}

void ImplementationManager::increasePassesCounter()
{
	passesCounter++;
}

void ImplementationManager::decreaseProfiling()
{
	profiling--;
}

double ImplementationManager::getCurrentMs()
{
	return currentMs;
}

double ImplementationManager::getAverageMs()
{
	return averageMs;
}

double ImplementationManager::getProfiling()
{
	return profiling;
}

double ImplementationManager::getPassesCounter()
{
	return passesCounter;
}