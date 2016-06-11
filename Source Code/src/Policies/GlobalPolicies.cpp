#include "AppContainer.h"
#include "GlobalPolicies.h"

void GlobalPolicies::Free(void * container, void * managers)
{
	
	
}

void GlobalPolicies::Powersaving(void * container,void * managers)
{
	if(overallAmpereGoal <= 0)
		return;
	
	AppContainer * application = static_cast<AppContainer*>(container);
	ImplementationManager *ptr = static_cast<ImplementationManager*>(managers);
	
	
}
