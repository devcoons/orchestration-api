#pragma once

	typedef enum
	{
		Performance,
		Balanced,
		Restricted
	}
	IndividualPolicyType;
	
	
	class IndividualPolicies
	{
		public:
			static void Performance(void *,void *);
			static void Balanced(void *,void *);
			static void Restricted(void *,void *);	
	};