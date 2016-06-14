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
		cpuAllCurrent = 0;
		cpuBigCurrent = 0;
		cpuLITTLECurrent =0;
		cpuBigKalman.initialize(1, 12, 1, 0);
		cpuLittleKalman.initialize(1, 12, 1, 0);
		cpuAllKalman.initialize(1, 12, 1, 0);
		for(int i=0;i<8;i++)		
			sysAllKalman[i].initialize(1, 12, 1, 0);
		
		
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
		
		cpuAllCurrent = (cpuBigCurrent + cpuLITTLECurrent ) / 2;
		
		cpuBigKalman.update(cpuBigCurrent);
		cpuLittleKalman.update(cpuLITTLECurrent);
		cpuAllKalman.update(cpuAllCurrent);
		
		for(int i=1;i<=4;i++)
		{
			sstream1.str(std::string());
			sstream2.str(std::string());	
			sstream1 << "/sys/class/hwmon/hwmon0/curr" << i << "_label";
			sstream2 << "/sys/class/hwmon/hwmon0/curr" << i << "_input";
			fileStream1.open (sstream1.str().c_str());
			fileStream2.open (sstream2.str().c_str());
			getline (fileStream1,sysData[i-1][0]) ;
			getline (fileStream2,sysData[i-1][1]) ;
			sysAllKalman[i-1].update(stol(sysData[i-1][1]));			
			fileStream1.close();
			fileStream2.close();
		}
		for(int i=1;i<=4;i++)
		{
			sstream1.str(std::string());
			sstream2.str(std::string());	
			sstream1 << "/sys/class/hwmon/hwmon0/power" << i << "_label";
			sstream2 << "/sys/class/hwmon/hwmon0/power" << i << "_input";
			fileStream1.open (sstream1.str().c_str());
			fileStream2.open (sstream2.str().c_str());
			getline (fileStream1,sysData[i-1+4][0]) ;
			getline (fileStream2,sysData[i-1+4][1]) ;	
			sysAllKalman[i-1+4].update(stol(sysData[i-1+4][1]));			
			fileStream1.close();
			fileStream2.close();
		}

		
	}
	
	void StatsJuno::updateSensors()
	{
		
	}
	
	string StatsJuno::retrieveUsage()
	{			
		return "[{\"name\":\"cpuAll\",\"rtvalue\":\""+std::to_string(cpuAllCurrent)+"\",\"avvalue\":\""+std::to_string((long)cpuAllKalman.getValue())+"\"},{\"name\":\"cpu0\",\"rtvalue\":\""+std::to_string(cpuLITTLECurrent)+"\",\"avvalue\":\""+std::to_string((long)cpuLittleKalman.getValue())+"\"},{\"name\":\"cpu1\",\"rtvalue\":\""+std::to_string(cpuBigCurrent)+"\",\"avvalue\":\""+std::to_string((long)cpuBigKalman.getValue())+"\"}]";
	}
	
	string StatsJuno::retrieveSensors()
	{
		return "[{\"name\":\""+sysData[0][0]+"\",\"data\":\""+sysData[0][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[0].getValue())+
		"\"},{\"name\":\""+sysData[1][0]+"\",\"data\":\""+sysData[1][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[1].getValue())+
		"\"},{\"name\":\""+sysData[2][0]+"\",\"data\":\""+sysData[2][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[2].getValue())+
		"\"},{\"name\":\""+sysData[3][0]+"\",\"data\":\""+sysData[3][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[3].getValue())+
		"\"},{\"name\":\""+sysData[4][0]+"\",\"data\":\""+sysData[4][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[4].getValue())+
		"\"},{\"name\":\""+sysData[5][0]+"\",\"data\":\""+sysData[5][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[5].getValue())+
		"\"},{\"name\":\""+sysData[6][0]+"\",\"data\":\""+sysData[6][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[6].getValue())+
		"\"},{\"name\":\""+sysData[7][0]+"\",\"data\":\""+sysData[7][1]+"\",\"kdata\":\""+std::to_string((long)sysAllKalman[7].getValue())+
		"\"}]";
	}