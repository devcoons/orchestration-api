#include "Statistics.h"
using namespace std;

namespace Statistics
{
	
	vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}
	
	void Callback(Parameters * parameters, void * object)
	{
		std::cout<<"[ Watcher ] : Connection Opened"<<std::endl;
		size_t bytecount;
		char buffer[2048];
		int buffer_len = 2048;
		std::string temporary;
						vector<std::string> sep ;
			vector<std::string> cpu0 ;
			vector<std::string> cpu1;
			vector<std::string> cpu2 ;
			vector<std::string> cpu3 ;
			vector<std::string> cpu4;
			vector<std::string> cpu5 ;
		Orchestration::Server * self = static_cast<Orchestration::Server*>(object);	
		int n;
		while(1)
		{
			temporary.clear();
			memset(buffer, 0, buffer_len);
			bytecount = recv(parameters->clientSocket, buffer, buffer_len, 0);
			std::string temp = buffer;	
			std::cout<<temp<<std::endl;
			if(temp=="terminate" || bytecount<=0 )
			{
				self->active = false;
				break;
			}
			else if(temp=="appstats")
			{
				temporary="[";
				for (auto app1 = self->applications.begin(); app1 != self->applications.end();)
				{
					temporary += "{\"pid\":\"" + std::to_string((*app1)->sharedMemoryPtr->processID) +"\",\"name\":\"" + ((*app1)->sharedMemoryPtr->processName) +"\",\"goal\":\""+std::to_string((*app1)->sharedMemoryPtr->statistics.getInitialGoalMs())+"\",\"current\":\""+std::to_string((*app1)->sharedMemoryPtr->statistics.getCurrentMs())+"\",\"realcurrent\":\""+std::to_string((*app1)->sharedMemoryPtr->statistics.getOffsetGoalMs())+"\",\"error\":\""+std::to_string((*app1)->sharedMemoryPtr->statistics.getAverageError())+"\",\"realerror\":\""+std::to_string((*app1)->sharedMemoryPtr->statistics.getCurrentError())+"\",\"pgoal\":\""+std::to_string((*app1)->sharedMemoryPtr->statistics.getPassesCounter())+"\"},";	
					++app1;
				}
				if(temporary.substr( temporary.length()-1,1)==",")
				temporary = temporary.substr(0, temporary.length()-1);
				temporary+="]";
				n = write(parameters->clientSocket,temporary.c_str(),temporary.length());			
			}
			else if(temp=="sysstats")
			{
				temporary="[";
				FILE *fpipe;
				if ( (fpipe = (FILE*)popen("sensors |  sed -e 's/\\s//g' | sed -e 's/[:]/\",\"data\":\"/g'| tail -n +3|sed '/^\\s*$/d' | sed 's/$/\"},/' |sed 's/^/{\"name\":\"/'", "r")) )
				while (fgets(buffer, sizeof(buffer)-1, fpipe) != NULL)
					temporary+=buffer;
				temporary.erase(std::remove(temporary.begin(), temporary.end(), '\n'), temporary.end());
				temporary = temporary.substr(0, temporary.length()-1);
				temporary+="]";
				n = write(parameters->clientSocket,temporary.c_str(),temporary.length());
			}
			else if(temp=="cpuusage")
		{
			temporary="";
			FILE *fp;
			fp = (FILE*)popen("head -n7 /proc/stat | tail -n6","r");
			while (fgets(buffer, sizeof(buffer)-1, fp) != NULL)
				temporary+=buffer;
			pclose(fp);
			 sep = split(temporary, '\n');
			 cpu0 = split(sep[0], ' ');
			 cpu1 = split(sep[1], ' ');
			 cpu2 = split(sep[2], ' ');
			 cpu3 = split(sep[3], ' ');
			cpu4 = split(sep[4], ' ');
			 cpu5 = split(sep[5], ' ');
			self->cpu_idle_0[0]=self->cpu_idle_0[1];
			self->cpu_all_0[0]=self->cpu_all_0[1];
			self->cpu_idle_1[0]=self->cpu_idle_1[1];
			self->cpu_all_1[0]=self->cpu_all_1[1];
			self->cpu_idle_0[1] = stol(cpu0[4].c_str())+stol(cpu1[4].c_str());
			
			self->cpu_idle_1[1] = stol(cpu2[4].c_str())+stol(cpu3[4].c_str())+stol(cpu4[4].c_str())+stol(cpu5[4].c_str());
			
			
			self->cpu_all_0[1] = stol(cpu0[1].c_str()) +stol(cpu0[2].c_str()) +stol(cpu0[3].c_str()) +stol(cpu0[4].c_str())+stol(cpu0[5].c_str())+stol(cpu0[6].c_str())+stol(cpu0[7].c_str())
			+stol(cpu1[1].c_str()) +stol(cpu1[2].c_str()) +stol(cpu1[3].c_str()) +stol(cpu1[4].c_str())+stol(cpu1[5].c_str())+stol(cpu1[6].c_str())+stol(cpu1[7].c_str());
			
			self->cpu_all_1[1] = stol(cpu2[1].c_str()) +stol(cpu2[2].c_str()) +stol(cpu2[3].c_str()) +stol(cpu2[4].c_str())+stol(cpu2[5].c_str())+stol(cpu2[6].c_str())+stol(cpu2[7].c_str())
			+stol(cpu3[1].c_str()) +stol(cpu3[2].c_str()) +stol(cpu3[3].c_str()) +stol(cpu3[4].c_str())+stol(cpu3[5].c_str())+stol(cpu3[6].c_str())+stol(cpu3[7].c_str())
			+stol(cpu4[1].c_str()) +stol(cpu4[2].c_str()) +stol(cpu4[3].c_str()) +stol(cpu4[4].c_str())+stol(cpu4[5].c_str())+stol(cpu4[6].c_str())+stol(cpu4[7].c_str())
			+stol(cpu5[1].c_str()) +stol(cpu5[2].c_str()) +stol(cpu5[3].c_str()) +stol(cpu5[4].c_str())+stol(cpu5[5].c_str())+stol(cpu5[6].c_str())+stol(cpu5[7].c_str());
			temporary="[{\"name\":\"cpu0\",\"value\":\""+std::to_string(100*(1- ( self->cpu_idle_1[1]-self->cpu_idle_1[0] )/(self->cpu_all_1[1]-self->cpu_all_1[0])))+"\"},{\"name\":\"cpu1\",\"value\":\""+std::to_string(100*(1- ( self->cpu_idle_0[1]-self->cpu_idle_0[0] )/(self->cpu_all_0[1]-self->cpu_all_0[0])))+"\"}]";
			n = write(parameters->clientSocket,temporary.c_str(),temporary.length());
			
		}
			else if(temp.find("throughtput")!= std::string::npos)
			{
				std::vector<std::string> vect;
				std::stringstream ss(temp);
				std::string ls;
				std::string word;
	 while( std::getline(ss, word, ':') )
			vect.push_back(word);
			
				for (auto app1 = self->applications.begin(); app1 != self->applications.end();)
				{
					for(int ll=0;ll<vect.size();ll++)
					if(((*app1)->sharedMemoryPtr->processName) == vect.at(ll))
					{
						(*app1)->sharedMemoryPtr->statistics.setInitialGoalMs(std::stod(vect.at(ll+1)));
					}
					++app1;
				}
			}
			else
			{
				n =write(parameters->clientSocket,"Unknown Command",15);
			}	
			usleep(100);
		}

		fflush(stdout);
	}
	
}