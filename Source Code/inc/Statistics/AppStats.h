#pragma once

	#include <cmath>
	
	using namespace std;
		
	class AppStats
	{
		private:
			volatile double offsetGoalMs;
			volatile double initialGoalMs;
			volatile double currentMs;
			volatile double averageMs;
			volatile double minGoalMs;
			volatile double maxGoalMs;
			volatile double currentError;
			volatile double averageError;
			volatile double passesCounter;
			
		protected:
		public:
			AppStats();
			~AppStats();	
			void setInitialGoalMs(double);
			void setOffsetGoalMs(double);
			void setCurrentMs(double);
			void setAverageMs(double);
			void setMinimumGoalMs(double);
			void setMaximumGoalMs(double);
			void setCurrentError(double);
			void setAverageError(double);
			void setPassesCounter(double);	
			void increasePassesCounter(void);
			double getInitialGoalMs(void);
			double getOffsetGoalMs(void);
			double getCurrentMs(void);
			double getAverageMs(void);
			double getMinimumGoalMs(void);
			double getMaximumGoalMs(void);
			double getCurrentError(void);
			double getAverageError(void);
			double getPassesCounter(void);
			
			double getNextGoalDeviation(double);
			
	};