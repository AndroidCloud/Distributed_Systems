#include <stdio.h>
#include <rpc/rpc.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <chrono>
#include "project.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

using namespace std;
using namespace std::chrono;

#include "datanode.h"
#include "namenode.h"

int readFlag=0,writeFlag=0;
int block_size = 10000000;

string serializeOpenFileRequest(char *filename)
{
	OpenFileRequest Request;
	string ss(filename);
	Request.set_filename(ss);
	if(readFlag==1)Request.set_forread(1);
	else Request.set_forread(0);
	string op;
	if(!Request.SerializeToString(&op))
	{
		cerr << "Failed to Serialise OpenFileRequest" <<endl;
		exit(0);
	}
	return op;
	

}

string serializeBlockNumber(int blockNum)
{
	int i;
	ReadBlockRequest Request;
	Request.set_blocknumber(blockNum);
	string op;
	if(!Request.SerializeToString(&op))
	{
		cerr << "Failed to write" <<endl;
		exit(0);
	}
	//cout << op << endl;
	return op;

}
void deserializeReadBlockResponse(string data)
{
	ReadBlockResponse Response;
	Response.ParseFromString(data);
	//cout << "len" << Response.data_size();
	int len = Response.data_size();

	for(int j=0;j<len;j++)
	{
		cout << Response.data(j);
	}
	
}

void deserializeOpenFileResponse(string data)
{
	OpenFileResponse Response;

	Response.ParseFromString(data);
	if(!Response.has_status())
	{
		cout << " error in OpenFileResponse " << endl;
		exit(0);
	}
	int len = Response.blockinfo_size();
	//cout << len << " len " << endl;
	for(int i=0;i<len;i++)
	{
		BlockLocations bl = Response.blockinfo(i);
		//cout << "BlockNumber " << bl.blocknumber() << endl;
		int blockNum = bl.blocknumber();
		string op = serializeBlockNumber(blockNum);
		int len2 = bl.locations_size();
		for(int j=0;j<1;j++)
		{
			DataNodeLocation dl = bl.locations(j);
			//cout << " ip: " << dl.ip() << endl;
			string ip = dl.ip();
			CLIENT *clnt;
			char * *result_1;
			char * readblock_1_arg;
			char * *result_2;
			//char * writeblock_1_arg;
			char *dip;

			dip = new char[ip.length() + 1];
			strcpy(dip, ip.c_str());
			readblock_1_arg = new char[op.length() + 1];
			strcpy(readblock_1_arg, op.c_str());
	
			#ifndef	DEBUG
			clnt = clnt_create (dip, DATANODE, DN, "tcp");
			if (clnt == NULL) {
				clnt_pcreateerror (dip);
				exit (1);
			}
			#endif

			result_1 = readblock_1(&readblock_1_arg,clnt);
			string str(*result_1);
			deserializeReadBlockResponse(str);
			
			#ifndef	DEBUG
			clnt_destroy (clnt);
			#endif	 /* DEBUG */
				
	
		}
	}
}
string serializeAssignBlockRequest(int handle)
{
	AssignBlockRequest Request;
	Request.set_handle(handle);
	string op;
	if(!Request.SerializeToString(&op))
	{
		cerr << "Failed to serializeAssignBlockRequest" <<endl;
		exit(0);
	}
	return op;


}

void serializeWriteBlockRequest(char *filename,char *namenode_ip,int file_handle)
{
	WriteBlockRequest Request;
	FILE *fp;
    char c;
    fp = fopen(filename, "r"); // error check this!
    int count=0;
 	string s1 = serializeAssignBlockRequest(file_handle);
    CLIENT *clnt;
	char * *result_1;
	char * readblock_1_arg;
	char * *result_2;
	//char * writeblock_1_arg;

	readblock_1_arg = new char[s1.length() + 1];
	strcpy(readblock_1_arg, s1.c_str());
	//cout << " Point 3" << endl;
	#ifndef	DEBUG
	clnt = clnt_create (namenode_ip, NAMENODE, NN, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (namenode_ip);
		exit (1);
	}
	#endif
	//cout << " Point 4 " << endl;
    while((c = fgetc(fp)) != EOF) 
    {
    	string data="";   
    	data = data+c;
    	Request.add_data(data);
    //	cout << count << endl;
    	count++;
    	if(count==block_size)
    	{
	cout << " Point 5 " << endl;
    		result_1 = assignblock_1(&readblock_1_arg,clnt);
			if (result_1 == (char **) NULL) {
			clnt_perror (clnt, "call failed");
		}
		string s(*result_1);
		AssignBlockResponse Response;
		Response.ParseFromString(s);

	cout << " Point 6 " << endl;
		if(Response.has_status())
		{
			BlockLocations bl = Response.newblock();
			int blockNum = bl.blocknumber();
			cout << " block num " << blockNum << endl;
			cout << " block " << blockNum << " written at: ";
			BlockLocations* bl1 = Request.mutable_blockinfo();
   			bl1->set_blocknumber(blockNum);;
			for(int j=0;j<bl.locations_size();j++)
			{
				CLIENT *cln;
				cout << bl.locations(j).ip() << " ";
				//cout << " size " << bl.locations(j).ip().length() << endl;
				char *dlp;
				dlp = new char[bl.locations(j).ip().length() + 1];
				strcpy(dlp, bl.locations(j).ip().c_str());
	
				#ifndef	DEBUG
				cln = clnt_create (dlp, DATANODE, DN, "tcp");
				if (cln == NULL) {
					clnt_pcreateerror (dlp);
					exit (1);
				}
				#endif
				string op;
				if(!Request.SerializeToString(&op))
				{
					cerr << "Failed to write" <<endl;
					exit(0);
				}
				
				char *writeblock_1_arg;
				writeblock_1_arg = new char[op.length() + 1];
				strcpy(writeblock_1_arg, op.c_str());
				result_2 = writeblock_1(&writeblock_1_arg,cln);

				#ifndef	DEBUG
				clnt_destroy (cln);
				#endif	 /* DEBUG */
			//	cout << " Point 2" << endl;
	
	
			}
			cout << endl;
		}
	

    		count=0;
    		Request.clear_data();
    	}
    }
    if(count!=0)
    {
    	result_1 = assignblock_1(&readblock_1_arg,clnt);
			if (result_1 == (char **) NULL) {
			clnt_perror (clnt, "call failed");
		}
		string s(*result_1);
		AssignBlockResponse Response;
		Response.ParseFromString(s);

		if(Response.has_status())
		{
			BlockLocations bl = Response.newblock();
			int blockNum = bl.blocknumber();
			BlockLocations* bl1 = Request.mutable_blockinfo();
   			bl1->set_blocknumber(blockNum);;
			
			cout << " block " << blockNum << " written at: " << endl;
			for(int j=0;j<bl.locations_size();j++)
			{
				CLIENT *cln;
				cout << bl.locations(j).ip() << " ";
				char *dlp;
				dlp = new char[bl.locations(j).ip().length() + 1];
				strcpy(dlp, bl.locations(j).ip().c_str());
	
				#ifndef	DEBUG
				cln = clnt_create (dlp, DATANODE, DN, "tcp");
				if (cln == NULL) {
					clnt_pcreateerror (dlp);
					exit (1);
				}
				#endif
				
				string op;
				if(!Request.SerializeToString(&op))
				{
					cerr << "Failed to write" <<endl;
					exit(0);
				}
				char *writeblock_1_arg;
				writeblock_1_arg = new char[op.length() + 1];
				strcpy(writeblock_1_arg, op.c_str());
				result_2 = writeblock_1(&writeblock_1_arg,cln);

				#ifndef	DEBUG
				clnt_destroy (cln);
				#endif	 /* DEBUG */

			}
			cout << endl;
		}

    }

    fclose(fp);
    
	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */

}


void datanode_1(char *filename,char *namenode_ip)
{
	CLIENT *clnt;
	char * *result_1;
	char * readblock_1_arg;
	char * *result_2;
	char * writeblock_1_arg;

	string s1 = serializeOpenFileRequest(filename);
	//send dis request to namenode

//	cout << " Point 1" << endl;
	#ifndef	DEBUG
	clnt = clnt_create (namenode_ip, NAMENODE, NN, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (namenode_ip);
		exit (1);
	}
	#endif	/* DEBUG */
	readblock_1_arg = new char[s1.length() + 1];
	strcpy(readblock_1_arg, s1.c_str());
	result_1 = openfile_1(&readblock_1_arg,clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */
//	cout << " Point 2" << endl;
	string s(*result_1);

	// read portion
	if(readFlag == 1)
	{
	deserializeOpenFileResponse(s);
	
	}
	else if(writeFlag==1)
	{
		OpenFileResponse Response;
		Response.ParseFromString(s);
		if(Response.status()==0)cout << " File Already Exists" << endl;
		else
		{
			int file_handle = Response.handle();
			serializeWriteBlockRequest(filename,namenode_ip,file_handle);
		}

	}
	
}


int
main (int argc, char *argv[])
{
	char *filename;
	char *namenode;

	if (argc < 4) {
		printf ("usage: read/write <file_name> <IP Address of Namenode>\n");
		exit (1);
	}
	if(strcmp(argv[1],"read")==0)readFlag=1;
	else writeFlag=1;
	filename = argv[2];
	namenode = argv[3];
	datanode_1 (filename,namenode);
	exit (0);
}
