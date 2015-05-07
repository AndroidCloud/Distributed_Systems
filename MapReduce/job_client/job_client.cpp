
#include <stdio.h>
#include <rpc/rpc.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <chrono>
#include "../Proto/MapReduce.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

using namespace std;
using namespace std::chrono;


#include "../header/jobtracker.h"

void
jobtracker_1(char *host,string op)
{
	CLIENT *clnt;
	char * *result_1;
	char * jobsubmit_1_arg;
	char * *result_2;
	char * getjobstatus_1_arg;
	char * *result_3;
	char * heartbeat_1_arg;
	printf("host %s\n",host);
#ifndef	DEBUG
	clnt = clnt_create (host, JOBTRACKER, JT, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	jobsubmit_1_arg = new char[op.length() + 1];
	strcpy(jobsubmit_1_arg, op.c_str());
	

	result_1 = jobsubmit_1(&jobsubmit_1_arg, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf("%s\n",result_1 );
	/*result_2 = getjobstatus_1(&getjobstatus_1_arg, clnt);
	if (result_2 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = heartbeat_1(&heartbeat_1_arg, clnt);
	if (result_3 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	*/
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 7) {
		printf ("usage: ./job_client <mapper> <reducer> <input_file> <op_file> <num of reduces> <JT ip>\n", argv[0]);
		exit (1);
	}
	host = argv[6];
	string mapper(argv[1]);
	string reducer(argv[2]);
	string input_file(argv[3]);
	string op_file(argv[4]);
	int num_reducers = atoi(argv[5]);
	//serializeJobSubmit
	JobSubmitRequest Request;
	Request.set_mapname(mapper);
	Request.set_reducername(reducer);
	Request.set_inputfile(input_file);
	Request.set_outputfile(op_file);
	Request.set_numreducetasks(num_reducers);
	string op;
	if(!Request.SerializeToString(&op))
	{
		cerr << "Failed to write" <<endl;
		exit(0);
	}
	jobtracker_1 (host,op);
exit (0);
}
