#pragma once

	#include <vector>
	#include "Server.h"
	
	using namespace std;
	
	class StdService
	{
		private:
			int port;
			::Server server;
			
		public:
			
			StdService();
			~StdService();
			void Start(int, void *);
			static vector<string> split(string , char );
			static void callback(Parameters *, void *);		
	};