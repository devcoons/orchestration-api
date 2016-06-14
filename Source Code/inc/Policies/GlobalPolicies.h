#pragma once

	typedef enum
	{
		Free,
		PowerBalancing,
		CurrentBalancing,
		UtilizationBalancing
	}
	GlobalPolicyType;

	class GlobalPolicies
	{
	public:			
		static void Free(void *, void *, void *, double);
		static void PowerBalancing(void *,void *, void *, double);
		static void CurrentBalancing(void *,void *, void *, double);
		static void UtilizationBalancing(void *,void *, void *, double);		
	};