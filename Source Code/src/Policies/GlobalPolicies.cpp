#include "StsService.h"
#include "AppContainer.h"
#include "GlobalPolicies.h"

	void GlobalPolicies::Free(void * container, void * managers, void * stsService, double goal)
	{
		
		
	}

	void GlobalPolicies::PowerBalancing(void * container,void * managers, void * stsService, double goal)
	{
		if(goal <= 0)
			return;
		
		StsService * statsService = static_cast<StsService*>(stsService);
		AppContainer * application = static_cast<AppContainer*>(container);
		ImplementationManager *ptr = static_cast<ImplementationManager*>(managers);
		
		if(goal >= statsService->getFilteredPowerAvg())
			return;		
				double newOffset = application->tracker.getInitialGoalMs()+application->tracker.getOffsetGoalMs()+(statsService->getFilteredPowerAvg() - goal) * 10 > application->tracker.getMaximumGoalMs() ? application->tracker.getMaximumGoalMs()-application->tracker.getInitialGoalMs() :  application->tracker.getOffsetGoalMs()+(statsService->getFilteredPowerAvg() - goal) * 10;
		application->tracker.setOffsetGoalMs(newOffset);
		
	}

	void GlobalPolicies::CurrentBalancing(void * container,void * managers, void * stsService, double goal)
	{
		if(goal <= 0)
			return;
		
		StsService * statsService = static_cast<StsService*>(stsService);
		AppContainer * application = static_cast<AppContainer*>(container);
		ImplementationManager *ptr = static_cast<ImplementationManager*>(managers);
		
		if(goal >= statsService->getFilteredCurrentAvg())
			return;
				double newOffset = application->tracker.getInitialGoalMs()+application->tracker.getOffsetGoalMs()+(statsService->getFilteredCurrentAvg() - goal) * 10 > application->tracker.getMaximumGoalMs() ? application->tracker.getMaximumGoalMs()-application->tracker.getInitialGoalMs() :  application->tracker.getOffsetGoalMs()+(statsService->getFilteredCurrentAvg() - goal) * 10;
		application->tracker.setOffsetGoalMs(newOffset);
			
	}

	void GlobalPolicies::UtilizationBalancing(void * container,void * managers, void * stsService, double goal)
	{
		if(goal <= 0)
			return;
		
		StsService * statsService = static_cast<StsService*>(stsService);
		AppContainer * application = static_cast<AppContainer*>(container);
		ImplementationManager *ptr = static_cast<ImplementationManager*>(managers);
		
		if(goal >= statsService->getFilterUtilizationAvg())
			return;
		double newOffset = application->tracker.getInitialGoalMs()+application->tracker.getOffsetGoalMs()+(statsService->getFilterUtilizationAvg() - goal) * 10 > application->tracker.getMaximumGoalMs() ? application->tracker.getMaximumGoalMs()-application->tracker.getInitialGoalMs() :  application->tracker.getOffsetGoalMs()+(statsService->getFilterUtilizationAvg() - goal) * 10;
		application->tracker.setOffsetGoalMs(newOffset);
		
	}