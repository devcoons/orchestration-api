#pragma once

	#include <chrono>
	#include <functional>

	typedef std::function<void(void*)> ImplementationKernel;
	
	typedef enum
	{
		XeonCPU,
		ARMA57,
		ARMA53,		
		VMCPU1,
		VMCPU2,
		FPGA,
		PCI
	}
	ImplementationType;
	
	class ImplementationManager
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
			
			ImplementationManager();
			~ImplementationManager();
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