#pragma once
	
	#include <string>
	
	using namespace std;
	
	class StatsVMachine
	{
		private:
		public:
			StatsVMachine();
			~StatsVMachine();
			void update();
			string retrieve();		
	};