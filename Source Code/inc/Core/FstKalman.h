#pragma once
	
	class FstKalman
	{
		private:
			double procNoiseCov; //process noise covariance
			double measNCov; //measurement noise covariance
			double value; //value
			double estErrCov; //estimation error covariance
			double kalmanGain; //kalman gain
		public:
			FstKalman();			
			~FstKalman();
			double getValue();
			void update(double);
			void initialize(double, double, double, double);		
	};
	
	
	