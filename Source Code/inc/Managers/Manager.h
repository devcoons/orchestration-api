#pragma once
#include <functional>
#include <chrono>
#include "CPUbIG.h"
#include "CPULittle.h"
#include "FPGA.h"
#include "PCI.h"

typedef enum
{
	CPUbIG,
	CPULittle,
	FPGA,
	PCI
}
ImplementationType;

typedef std::function<void(void*)> ImplementationKernel;

class Manager
{
private:
protected:
public:
	Manager();
	~Manager();
	double	counter;
	double	profiling;
	double	currentThroughput;
	double	averageThroughput;
	ImplementationKernel function;
	ImplementationType type;
};

