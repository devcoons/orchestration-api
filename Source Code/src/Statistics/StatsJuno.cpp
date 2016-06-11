#include "StatsJuno.h"
#include "OrchestrationServer.h"
		
	StatsJuno::StatsJuno()
	{
		cpu_idle_0[0]=0;
		cpu_all_0[0]=0;
		cpu_idle_0[1]=0;
		cpu_all_0[1]=0;
		cpu_idle_1[0]=0;
		cpu_all_1[0]=0;
		cpu_idle_1[1]=0;
		cpu_all_1[1]=0;
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
	

	
	string StatsJuno::retrieveUsage()
	{
						temporary="";
		FILE *fp;
		
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
		cpu_idle_0[0] = cpu_idle_0[1];
		cpu_all_0[0] = cpu_all_0[1];
		cpu_idle_1[0] = cpu_idle_1[1];
		cpu_all_1[0] = cpu_all_1[1];
		cpu_idle_0[1] = stol(cpu0[4].c_str())+stol(cpu1[4].c_str());
		cpu_idle_1[1] = stol(cpu2[4].c_str())+stol(cpu3[4].c_str())+stol(cpu4[4].c_str())+stol(cpu5[4].c_str());
		cpu_all_0[1] = stol(cpu0[1].c_str()) +stol(cpu0[2].c_str()) +stol(cpu0[3].c_str()) +stol(cpu0[4].c_str())+stol(cpu0[5].c_str())+stol(cpu0[6].c_str())+stol(cpu0[7].c_str())
			+stol(cpu1[1].c_str()) +stol(cpu1[2].c_str()) +stol(cpu1[3].c_str()) +stol(cpu1[4].c_str())+stol(cpu1[5].c_str())+stol(cpu1[6].c_str())+stol(cpu1[7].c_str());
		cpu_all_1[1] = stol(cpu2[1].c_str()) +stol(cpu2[2].c_str()) +stol(cpu2[3].c_str()) +stol(cpu2[4].c_str())+stol(cpu2[5].c_str())+stol(cpu2[6].c_str())+stol(cpu2[7].c_str())
			+stol(cpu3[1].c_str()) +stol(cpu3[2].c_str()) +stol(cpu3[3].c_str()) +stol(cpu3[4].c_str())+stol(cpu3[5].c_str())+stol(cpu3[6].c_str())+stol(cpu3[7].c_str())
			+stol(cpu4[1].c_str()) +stol(cpu4[2].c_str()) +stol(cpu4[3].c_str()) +stol(cpu4[4].c_str())+stol(cpu4[5].c_str())+stol(cpu4[6].c_str())+stol(cpu4[7].c_str())
			+stol(cpu5[1].c_str()) +stol(cpu5[2].c_str()) +stol(cpu5[3].c_str()) +stol(cpu5[4].c_str())+stol(cpu5[5].c_str())+stol(cpu5[6].c_str())+stol(cpu5[7].c_str());
			
		return "[{\"name\":\"cpu0\",\"value\":\""+std::to_string(100*(1- ( cpu_idle_1[1]-cpu_idle_1[0] )/(cpu_all_1[1]-cpu_all_1[0])))+"\"},{\"name\":\"cpu1\",\"value\":\""+std::to_string(100*(1- ( cpu_idle_0[1]-cpu_idle_0[0] )/(cpu_all_0[1]-cpu_all_0[0])))+"\"}]";

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