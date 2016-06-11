#pragma once

	typedef enum
	{
		Free,
		Powersaving
	}
	GlobalPolicyType;

	class GlobalPolicies
	{
	public:
		static double overallAmpereGoal;
		static void Free(void *, void *);
		static void Powersaving(void *,void *);
	};