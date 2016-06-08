#pragma once

typedef enum
{
	Performance,
	Balanced,
	Powersaving
}
PolicyType;

class Policy
{
public:
	Policy();
	~Policy();
};

