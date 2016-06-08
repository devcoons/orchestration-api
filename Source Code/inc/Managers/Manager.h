#pragma once

	#include <chrono>
	#include <functional>
	#include "X64Xeon.h"
	#include "ARMJuno.h"
	#include "X64VMachine.h"

	typedef std::function<void(void*)> ImplementationKernel;

	typedef enum
	{
		JunoCPUA57,
		JunoCPUA53,
		JunoFPGA,
		X64XeonCPU,
		X64XeonPCI,
		X64VMachineCPU1,
		X64VMachineCPU2
	}
	ImplementationType;

	class Manager
	{
		private:
			double currentMs;
			double averageMs;
			double profiling;
			double passesCounter;
			
		protected:
		public:
			ImplementationType type;
			ImplementationKernel function;
			
			Manager();
			~Manager();
			void setCurrentMs(double);
			void setAverageMs(double);
			void setProfiling(double);
			void setPassesCounter(double);
			void increasePassesCounter();
			void decreaseProfiling();
			double getCurrentMs();
			double getAverageMs();
			double getProfiling();
			double getPassesCounter();
	};