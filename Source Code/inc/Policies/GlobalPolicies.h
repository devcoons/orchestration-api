#pragma once

	typedef enum
	{
		Performance,
		Powersaving
	}
	GlobalPolicyType;

	class GlobalPolicy
	{
	public:
		GlobalPolicy();
		~GlobalPolicy();
	};