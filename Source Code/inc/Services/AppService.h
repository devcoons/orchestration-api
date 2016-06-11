#pragma once

	#include "Server.h"

	class AppService
	{
		private:
			int port;
			::Server server;
			
		public:
			AppService();
			~AppService();
			void Start(int, void *);
			static void callback(Parameters *, void *);
	};