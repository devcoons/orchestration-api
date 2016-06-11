#include "AppContainer.h"
#include "IndividualPolicies.h"

void IndividualPolicies::Performance(void * container,void * managers)
{
	//??
}

void IndividualPolicies::Balanced(void * container,void * managers)
{
	AppContainer * application = static_cast<AppContainer*>(container);
	ImplementationManager *ptr = static_cast<ImplementationManager*>(managers);
	
	int selectedKernel = 0;
	double currentDeviation = 0;
	double selectedDeviation = application->tracker.getNextGoalDeviation((ptr[0].getCurrentMs()*40/100) + (ptr[0].getAverageMs()*60/100));
	
	for (int j = 0; j < application->getManagers()->getSize(); j++)
	{
		currentDeviation =  application->tracker.getNextGoalDeviation((ptr[j].getCurrentMs()*40/100) + (ptr[j].getAverageMs()*60/100));
		if (ptr[j].getAverageMs() == 0)
		{
			selectedKernel = j;
			break;
		}
		if (currentDeviation < selectedDeviation)
		{
			selectedKernel = j;	
			selectedDeviation = currentDeviation;						
		}
	}
	application->selectedKernelManager = selectedKernel;
}

void IndividualPolicies::Restricted(void * container,void * managers)
{
	AppContainer * application = static_cast<AppContainer*>(container);
	ImplementationManager *ptr = static_cast<ImplementationManager*>(managers);
	
	int selectedKernel = -1;
	double currentDeviation = 0;
	double selectedDeviation = application->tracker.getNextGoalDeviation((ptr[0].getCurrentMs()*40/100) + (ptr[0].getAverageMs()*60/100));
	double currentMs = 0;
	for (int j = 0; j < application->getManagers()->getSize(); j++)
	{
		currentMs = (ptr[j].getCurrentMs()*40/100) + (ptr[j].getAverageMs()*60/100);
		currentDeviation =  application->tracker.getNextGoalDeviation(currentMs);
		if (ptr[j].getAverageMs() == 0)
		{
			selectedKernel = j;
			break;
		}
		if ((currentDeviation < selectedDeviation || selectedKernel == -1)  && currentMs >= application->tracker.getMinimumGoalMs() && currentMs <= application->tracker.getMaximumGoalMs())
		{
			selectedKernel = j;	
			selectedDeviation = currentDeviation;						
		}
	}
	application->selectedKernelManager = selectedKernel;
}

