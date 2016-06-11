#include "StatsJuno.h"
		
	StatsJuno::StatsJuno()
	{
		cpuBig[0]=0;
		cpuBig[2]=0;
		cpuBig[1]=0;
		cpuBig[3]=0;
		cpuLITTLE[0]=0;
		cpuLITTLE[2]=0;
		cpuLITTLE[1]=0;
		cpuLITTLE[3]=0;
		passesCounter = 0;
		cpuBigAverage = 0;
		cpuLITTLEAverage = 0;
		cpuBigCurrent = 0;
		cpuLITTLECurrent =0;
	}
	
	StatsJuno::~StatsJuno()
	{
		
	}
	
	vector<string> StatsJuno::split(string str, char delimiter) 
	{
		vector<string> internal;
		stringstream ss(str);
		string tok;
  
		while(getline(ss, tok, delimiter))
			internal.push_back(tok);
		
		return internal;
	}
	
	void StatsJuno::update()
	{
		FILE *fp;
		temporary="";
		fp = (FILE*)popen("head -n7 /proc/stat | tail -n6","r");
		while (fgets(buffer, sizeof(buffer)-1, fp) != NULL)
			temporary+=buffer;
		pclose(fp);
		vector<std::string> sep,cpu0,cpu1,cpu2,cpu3,cpu4,cpu5;
		sep = split(temporary, '\n');
		cpu0 = split(sep[0], ' ');
		cpu1 = split(sep[1], ' ');
		cpu2 = split(sep[2], ' ');
		cpu3 = split(sep[3], ' ');
		cpu4 = split(sep[4], ' ');
		cpu5 = split(sep[5], ' ');
		cpuBig[0] = cpuBig[1];
		cpuBig[2] = cpuBig[3];
		cpuLITTLE[0] = cpuLITTLE[1];
		cpuLITTLE[2] = cpuLITTLE[3];
		cpuBig[1] = stol(cpu0[4].c_str())+stol(cpu1[4].c_str());
		cpuLITTLE[1] = stol(cpu2[4].c_str())+stol(cpu3[4].c_str())+stol(cpu4[4].c_str())+stol(cpu5[4].c_str());
		cpuBig[3] = stol(cpu0[1].c_str()) +stol(cpu0[2].c_str()) +stol(cpu0[3].c_str()) +stol(cpu0[4].c_str())+stol(cpu0[5].c_str())+stol(cpu0[6].c_str())+stol(cpu0[7].c_str())
			+stol(cpu1[1].c_str()) +stol(cpu1[2].c_str()) +stol(cpu1[3].c_str()) +stol(cpu1[4].c_str())+stol(cpu1[5].c_str())+stol(cpu1[6].c_str())+stol(cpu1[7].c_str());
		cpuLITTLE[3] = stol(cpu2[1].c_str()) +stol(cpu2[2].c_str()) +stol(cpu2[3].c_str()) +stol(cpu2[4].c_str())+stol(cpu2[5].c_str())+stol(cpu2[6].c_str())+stol(cpu2[7].c_str())
			+stol(cpu3[1].c_str()) +stol(cpu3[2].c_str()) +stol(cpu3[3].c_str()) +stol(cpu3[4].c_str())+stol(cpu3[5].c_str())+stol(cpu3[6].c_str())+stol(cpu3[7].c_str())
			+stol(cpu4[1].c_str()) +stol(cpu4[2].c_str()) +stol(cpu4[3].c_str()) +stol(cpu4[4].c_str())+stol(cpu4[5].c_str())+stol(cpu4[6].c_str())+stol(cpu4[7].c_str())
			+stol(cpu5[1].c_str()) +stol(cpu5[2].c_str()) +stol(cpu5[3].c_str()) +stol(cpu5[4].c_str())+stol(cpu5[5].c_str())+stol(cpu5[6].c_str())+stol(cpu5[7].c_str());
		
		cpuLITTLECurrent = 100*(1- ( cpuLITTLE[1]-cpuLITTLE[0] )/(cpuLITTLE[3]-cpuLITTLE[2]));
		cpuBigCurrent = 100*(1- ( cpuBig[1]-cpuBig[0] )/(cpuBig[3]-cpuBig[2]));	
		
		passesCounter++;
		
		if(passesCounter == 1)
		{
			cpuLITTLEAverage = cpuLITTLE[4];
			cpuBigAverage = cpuBig[4];
		}
		else
		{
			cpuLITTLEAverage = cpuLITTLEAverage / passesCounter * (passesCounter-1) + cpuLITTLE[4] / passesCounter;
			cpuBigAverage = cpuBigAverage / passesCounter * (passesCounter-1) + cpuBig[4] / passesCounter; 
		}
	}
	
	void StatsJuno::updateSensors()
	{
		
	}
	
	
	string StatsJuno::retrieveUsage()
	{			
		return "[{\"name\":\"cpu0\",\"rtvalue\":\""+std::to_string(cpuLITTLECurrent)+"\",\"avvalue\":\""+std::to_string(cpuLITTLEAverage)+"\"},{\"name\":\"cpu1\",\"rtvalue\":\""+std::to_string(cpuBigCurrent)+"\",\"avvalue\":\""+std::to_string(cpuBigAverage)+"\"}]";
	}
	
	string StatsJuno::retrieveSensors()
	{
		temporary="[";
		FILE *fpipe;
		if ( (fpipe = (FILE*)popen("sensors |  sed -e 's/\\s//g' | sed -e 's/[:]/\",\"data\":\"/g'| tail -n +3|sed '/^\\s*$/d' | sed 's/$/\"},/' |sed 's/^/{\"name\":\"/'", "r")) )
		while (fgets(buffer, sizeof(buffer)-1, fpipe) != NULL)
			temporary+=buffer;
		temporary.erase(std::remove(temporary.begin(), temporary.end(), '\n'), temporary.end());
		temporary = temporary.substr(0, temporary.length()-1);
		temporary+="]";
		return temporary;
	}