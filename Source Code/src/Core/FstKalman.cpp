#include "FstKalman.h"

	FstKalman::FstKalman()
	{
		
	}

	FstKalman::~FstKalman()
	{
		
	}
	
	void FstKalman::initialize(double _procNoiseCov, double _measNCov, double _estErrCov, double _initVal)
	{
		procNoiseCov = _procNoiseCov;
		measNCov = _measNCov;
		estErrCov = _estErrCov;
		value = _initVal;
	}
	
	void FstKalman::update(double measurement)
	{
		estErrCov = estErrCov + procNoiseCov;
		kalmanGain = estErrCov / (estErrCov + measNCov);
		value = value + kalmanGain * (measurement - value);
		estErrCov = (1 - kalmanGain) * estErrCov;
	}
	
	double FstKalman::getValue()
	{
		return value;
	}