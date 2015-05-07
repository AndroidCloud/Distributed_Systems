
#include <stdio.h>
//#include <sys/types.h>
//#include <rpc/auth.h>
#include <rpc/rpc.h>
#include <rpc/clnt.h>

#include <bits/stdc++.h>
#include <sys/stat.h>
#include <chrono>
#include <thread>
#include <mutex>
#include "../Proto/MapReduce.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

using namespace std;
using namespace std::chrono;



#include "../header/jobtracker.h"
#include "../header/tasktracker.h"


static std::mutex barrier1;
static std::mutex barrier2;
static std::mutex barrier3;
map <string, vector<int> > blockReport;
map <int,int> block_map;

int num_threads=2,num_taskTracker=2;

vector <string> init()
{
	vector <string> ip;
	//ip.push_back("10.0.3.163");
	//ip.push_back("10.0.3.160");
	ip.push_back("10.0.3.163");
	ip.push_back("127.0.0.1");
	return ip;
}
vector <string> tasktracker_ip = init();
vector <int> countThread;
int presentTaskTracker = 0;

vector < vector<int> > ar(num_taskTracker,vector <int> (num_threads,0));
char* convertString(string s)
{
	char *ret;
	ret = new char[s.length()+1];
	strcpy(ret,s.c_str());
	return ret;
}
void allocateReducer(int reduceWrk,int jobId,string reducerName)
{
	int TTid,threadId;
	//char *host;
	barrier1.lock();
//	vector <int> countThread(num_taskTracker,0);
	while(1)
	{
		int flag=0;
		for(int i=0;i<num_taskTracker;i++)
		{
			for(int j=0;j<num_threads;j++)
			{
				if(ar[i][j]==0)
				{
					TTid=i;
					threadId=j+1;
					ar[i][j]=reduceWrk;
					flag=1;
					break;
				}
			}
			if(flag==1)break;
		}
		if(flag==1)break;
	}
	
	barrier1.unlock();
	
	/*TTid = presentTaskTracker;
	presentTaskTracker = (presentTaskTracker+1)%num_taskTracker;
	threadId = countThread[TTid]+1;
	countThread[TTid] = (countThread[TTid]+1)%num_threads;
	*/
	/*bool_t bres;
	int mode = RPC_CL_NONBLOCKING;
    int flushMode = RPC_CL_BLOCKING_FLUSH;
	*/
	ReducerTaskInfo info;
	info.set_jobid(jobId);
	info.set_taskid(threadId);
	info.set_reducername(reducerName);
	cout << reducerName << " " << reduceWrk << " " << info.reducername() << endl;
	//info.set_outputfile(output);
	info.set_ownip(tasktracker_ip[TTid]);
	info.set_reducernumber(reduceWrk);
	for(int i=0;i<num_taskTracker;i++)
	{
		info.add_mapoutputlocation(tasktracker_ip[i]);
	}
	string op;
	if(!info.SerializeToString(&op))
	{
		cerr << "Failed to write" <<endl;
		exit(0);
	}
	cout << op << endl;
	//barrier2.lock();
	CLIENT *clnt;
	char *host = convertString(tasktracker_ip[TTid]);
	
	#ifndef	DEBUG
	clnt = clnt_create (host, TASKTRACKER, TT, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	/*bres = clnt_control(clnt, CLSET_IO_MODE, (char*)&mode);
    if (bres)
	    bres = clnt_control(clnt, CLSET_FLUSH_MODE, (char*)&flushMode);

    if (!bres) {
        clnt_perror(clnt, "clnt_control");
        exit(1);
    }
*/
	char* *result;

	char *arg = convertString(op);
	cout << " Going in " << reduceWrk << " " << tasktracker_ip[TTid] <<  endl;
	result = reducer_1(&arg,clnt);
/*	bres = clnt_control(clnt, CLFLUSH, NULL);
    if (!bres) {
			  clnt_perror(clnt, "clnt_control");
    }
    */
	cout << " Coming Out " << reduceWrk << " " << tasktracker_ip[TTid] << endl;
	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */
	//barrier2.unlock();
	
//	barrier1.unlock();
	
	barrier3.lock();
	ar[TTid][threadId]=0;
	barrier3.unlock();
	
	//send Info to TT regarding reduce job

}

int getJobId()
{
	ifstream infile;
	infile.open("JobId");
	int job=0,max_id=0;
	while(infile >> job)
	{
		if(job > max_id)max_id=job;
	}
	infile.close();
	max_id++;
	ofstream outfile;
	outfile.open("JobId", std::ios_base::app);
	string line = to_string(max_id) + "\n";
	outfile << line;
	outfile.close();
	return max_id;
}

int getFileHandle(string given_filename)
{
	ifstream infile;
	infile.open("file_list");
	int file_handle;
	string filename;
	while(infile >> filename >> file_handle)
	{
		if(filename == given_filename)break;
	}
	return file_handle;
}


void createBlockReport(int given_file_handle)
{
	int file_handle,blocknum;
	string ip;
	ifstream infile;
	infile.open("file_info");
	while(infile >> file_handle >> blocknum >> ip)
	{
		if(file_handle == given_file_handle && block_map.find(blocknum) == block_map.end())
		{
			cout << "handle " << file_handle << " " << blocknum << endl;
			blockReport[ip].push_back(blocknum);
			block_map[blocknum]=1;
		}
	}
	infile.close();


}
void assign_Task(string mapper_name,int jobId,int numReduceTasks)
{
	for (std::map< string,vector<int> >::iterator it=blockReport.begin(); it!=blockReport.end(); ++it)
	{
		MapTaskInfo info;
		info.set_jobid(jobId);
		info.set_mapname(mapper_name);
		info.set_numreducetasks(numReduceTasks);
		CLIENT *clnt;
		char * *result_1;
		char * map_1_arg;
			
		string ip = it->first;
		char *tasktracker_ip;
		tasktracker_ip = new char[ip.length() + 1];
		strcpy(tasktracker_ip, ip.c_str());
		vector <int> blocks = it->second;
		int len = blocks.size();
		for(int i=0;i<len;i++)
		{
			info.add_inputblocks(blocks[i]);
		}
		string op;
		if(!info.SerializeToString(&op))
		{
			cerr << "Failed to serialize MapTaskInfo" <<endl;
			exit(0);
		}
	

		map_1_arg = new char[op.length() + 1];
		strcpy(map_1_arg, op.c_str());
		

		#ifndef	DEBUG

		clnt = clnt_create (tasktracker_ip, TASKTRACKER, TT, "tcp");
		if (clnt == NULL) {
			clnt_pcreateerror (tasktracker_ip);
			exit (1);
		}
		#endif
		result_1 = mapper_1(&map_1_arg,clnt);
		#ifndef	DEBUG
			clnt_destroy (clnt);
		#endif	 /* DEBUG */

		info.clear_inputblocks();	

	}
	
}

char **
jobsubmit_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result="hi";
	if(*argp!=NULL)
	{
		//deserialize
		string data(*argp);
		JobSubmitRequest Request;
		Request.ParseFromString(data);
		string mapper_name = Request.mapname();
		string input_file = Request.inputfile();
		int numReduceTasks = Request.numreducetasks();
		string reducerName = Request.reducername();
		int id = getJobId();

		int file_handle = getFileHandle(input_file);
		createBlockReport(file_handle);
		assign_Task(mapper_name,id,numReduceTasks);
	
	//Reduce Work Starts here
		thread t[numReduceTasks];
		for(int i=0;i<numReduceTasks;i++)
		{
				t[i] = thread(allocateReducer,i+1,id,reducerName);
		}
		for(int i=0;i<numReduceTasks;i++)
		{
				t[i].join();
		}
		
		countThread.resize(num_taskTracker,0);
		//for(int i=0;i<numReduceTasks;i++)allocateReducer(i+1,id,reducerName);

	}
	/*
	 * insert server code here
	 */
	// cout << " Cgeck " << endl;
	return &result;
}

char **
getjobstatus_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */

	return &result;
}

char **
heartbeat_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */

	return &result;
}
