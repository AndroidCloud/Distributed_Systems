
#include <stdio.h>
#include <rpc/rpc.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <chrono>
#include "project.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
 #include <time.h>

using namespace std;
using namespace std::chrono;


#include "namenode.h"
int num_datanodes=3;

vector <string> init()
{
	vector <string> ip;
	ip.push_back("10.0.3.163");
	ip.push_back("10.0.3.160");
	ip.push_back("10.0.3.201");
	//ip.push_back("127.0.0.1");
	return ip;
}
vector <string> datanode_ip = init();



char * convertStringToChar(string str)
{
	int len = str.size();
	//cout << len << endl;
	char *d = new char[len+1];
	for(int j=0;j<len;j++)d[j]=str[j];
	return d;
}

string serializeAssignBlockResponse(int handle)
{
	srand (time(0));
	ifstream infile("file_info");
	string block_ip;
	int block_num,file_handle,flag=0,count=0,given_filehandle;
	AssignBlockResponse Response;
	map <int,vector <string> > mymap;
	int max_blocknum=0,max_file_handle=0;
	while(infile >> file_handle >> block_num >> block_ip)
	{
		if(max_blocknum < block_num)max_blocknum = block_num;
		
	}
	max_blocknum++;
	int num1,num2;
	//srand(time(NULL));
	num1 = rand();
	num2 = rand();
	num1 = num1%num_datanodes;
	num2 = num2%num_datanodes;
	while(num2==num1)
	{
		num1 = rand();
		num2 = rand();
		num1 = num1%num_datanodes;
		num2 = num2%num_datanodes;
	
	}
	infile.close();

	string line1 = to_string(handle) + " " + to_string(max_blocknum) + " " + datanode_ip[num1] + "\n";
	string line2 = to_string(handle) + " " + to_string(max_blocknum) + " " + datanode_ip[num2] + "\n";
	ofstream outfile;
  	outfile.open("file_info", std::ios_base::app);
  	outfile << line1;
  	outfile << line2;
  	Response.set_status(1);
  	BlockLocations* bl = Response.mutable_newblock();
    bl->set_blocknumber(max_blocknum);
    for(int i=0;i<2;i++)
    {
    	DataNodeLocation* dl = bl->add_locations();
    	if(i==0)dl->set_ip(datanode_ip[num1]);
    	else dl->set_ip(datanode_ip[num2]);
    }
    string op;
	if(!Response.SerializeToString(&op))
	{
		cerr << "Failed to write" <<endl;
		exit(0);
	}
	return op;

	

}

string serializeOpenFileResponse(string given_filename,int readFlag)
{
	OpenFileResponse Response;
	ifstream infile1("file_list");
	string file_name;
	int file_handle,max_file_handle=0,flag_exist=0;
	while(infile1 >> file_name >> file_handle)
	{
		if(file_name == given_filename)
		{
			
			flag_exist=1;
			break;
		}
		if(file_handle > max_file_handle)max_file_handle = file_handle;
	}
	if(flag_exist==1)max_file_handle = file_handle;
	else max_file_handle++;
	infile1.close();
	
	//for read
	if(readFlag==1){
	ifstream infile("file_info");
	string block_ip;
	int block_num,file_handle_1,flag=0,count=0,given_filehandle;
	map <int,vector <string> > mymap;
	
	while(infile >> file_handle_1 >> block_num >> block_ip)
	{
		if(file_handle_1 == max_file_handle){
			mymap[block_num].push_back(block_ip);
			flag=1;
			count++;
		}
	}
	infile.close();
	if(flag==1)
	{
		Response.set_status(1);
		Response.set_handle(max_file_handle);
		map <int,vector <string> >::iterator it;
		for(it=mymap.begin();it!=mymap.end();++it)
		{
			//cout << it->first << endl;
			BlockLocations* bl = Response.add_blockinfo();
			bl->set_blocknumber(it->first);
			vector <string> ip = it->second;
			int len = ip.size();
			for (int j=0;j<len;j++)
			{
				DataNodeLocation* dl = bl->add_locations();
				dl->set_ip(ip[j]);
			}
		}


	}
	}//readflag=1;
	else
	{
		if(flag_exist==0)
			{
				Response.set_status(1);
				Response.set_handle(max_file_handle);
				ofstream outfile;
				outfile.open("file_list", std::ios_base::app);
				string line = given_filename + " " + to_string(max_file_handle) +"\n";
				outfile << line;
				outfile.close();
			}
		else Response.set_status(0);
	}
	
	string op;
	if(!Response.SerializeToString(&op))
	{
		cerr << "Failed to write" <<endl;
		exit(0);
	}
	return op;
	

}


char **
openfile_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	if(*argp!=NULL)
	{
		string s1(*argp);
	//	deserializeOpenFileRequest(s1);
		OpenFileRequest Request;
		Request.ParseFromString(s1);
		//cout << Request.filename() << " " << Request.forread() << endl;
		string s2;
		int forread = Request.forread();
		s2 = serializeOpenFileResponse(Request.filename(),forread);
		result = convertStringToChar(s2);

	}

	
	return &result;
}

char **
getblocklocations_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */

	return &result;
}

char **
assignblock_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;
	if(*argp!=NULL)
	{
	string s(*argp);
	AssignBlockRequest Request;
	Request.ParseFromString(s);
	int file_handle = Request.handle();
	string s2 = serializeAssignBlockResponse(file_handle);
	result = convertStringToChar(s2);
	}
	return &result;
}

char **
closefile_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */

	return &result;
}

char **
list_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */

	return &result;
}

char **
sendblockreport_1_svc(char **argp,struct svc_req *rqstp)
{
	static char * result="hi";
	BlockReportRequest Request;
	string data(*argp);
	//cout << " length " << strlen(*argp) << endl;
	Request.ParseFromString(data);
	int len = Request.blocknumbers_size();
	int id = Request.id();
	if(len > 0)
	{
		cout << "BlockReport of DN " << id << " : ";
		for(int i=0;i<len;i++)cout << Request.blocknumbers(i) << " ";
		cout << endl;

	}


	return &result;
}

char**
sendheartbeat_1_svc(char **argp, struct svc_req *rqstp)
{
//	cout << " hi " << endl;
	static char * result="hi";
	HeartBeatRequest Request;
	string data(*argp);
	Request.ParseFromString(data);
	cout << " DN " << Request.id() << " Alive " << endl;
	return &result;
}
