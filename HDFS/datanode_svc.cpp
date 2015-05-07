//#include "namenode.h"

#include "datanode.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <functional>
#include <bits/stdc++.h>
#include "namenode.h"
 #include <sys/wait.h>
 #include <time.h>
 #include "project.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>



#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif
char *namenode_ip="10.1.33.58";

int dn_id=1;
using namespace std;
clock_t initial = clock();

string serializeHeartBeatRequest()
{
	HeartBeatRequest Request;
	Request.set_id(dn_id);
	string op;
	if(!Request.SerializeToString(&op))
	{
		cerr << "Failed to write" <<endl;
		exit(0);
	}

	return op;
	
}

string serializeBlockReportRequest()
{
	BlockReportRequest Request;
	ifstream infile;
	infile.open("blocks_info");
	int num;
	while(infile >> num)
	{
	//	cout << "num " << num << endl;
		Request.add_blocknumbers(num);
	}
	infile.close();
	Request.set_id(dn_id);
	//cout << " size " << Request.blocknumbers_size() << endl;
	string op;
	if(!Request.SerializeToString(&op))
	{
		cerr << "Failed to write" <<endl;
		exit(0);
	}
	//for(int i=0;i<op.length();i++)cout << int(op[i])<<"-";cout << endl;
	return op;
	
}
static void
datanode_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		char *readblock_1_arg;
		char *writeblock_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case readBlock:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) readblock_1_svc;
		break;

	case writeBlock:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) writeblock_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}
pid_t pid;
void cleanup(int signal) {
  int status;
  waitpid(pid, &status, 0);
  write(1,"cleanup!\n",9);
}

int
main (int argc, char **argv)
{
//	start();
	signal(SIGCHLD, cleanup);
	pid = fork();
	if(pid > 0)
	{
	register SVCXPRT *transp;

	pmap_unset (DATANODE, DN);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, DATANODE, DN, datanode_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (DATANODE, DN, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, DATANODE, DN, datanode_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (DATANODE, DN, tcp).");
		exit(1);
	}
	
	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
	}
	else if(pid ==0)
	{
		string op = serializeHeartBeatRequest();
		CLIENT *clnt;
		//char *namenode_ip = "127.0.0.1";
		char * *result_1;
		char * readblock_1_arg;
		char * *result_2;
				
		readblock_1_arg = new char[op.length() + 1];
		strcpy(readblock_1_arg, op.c_str());
	
		#ifndef	DEBUG
		clnt = clnt_create (namenode_ip, NAMENODE, NN, "tcp");
		if (clnt == NULL) {
			clnt_pcreateerror (namenode_ip);
			exit (1);
		}
		#endif
		
		while(true)
			{
				clock_t final=clock()-initial;
				double elapsed = (double)final / ((double)CLOCKS_PER_SEC);
				unsigned long int a = (unsigned long int)elapsed;
				double b = elapsed - (unsigned long int)elapsed;
				int num=0;
				if(b==0.0 && a%120==0){
					result_1 = sendheartbeat_1(&readblock_1_arg,clnt);
					string op2 = serializeBlockReportRequest();
					int length = op2.length();
					char * sendblockreport_1_arg;// = convertStringToChar2(op2);
					sendblockreport_1_arg = new char[length + 1];
					strcpy(sendblockreport_1_arg, op2.c_str());
				//	cout << " length4 " << strlen(sendblockreport_1_arg) << endl;; 
					result_2 = sendblockreport_1(&sendblockreport_1_arg,clnt);

				}
			}
			#ifndef	DEBUG
			clnt_destroy (clnt);
			#endif	 /* DEBUG */
			
    kill( pid, SIGINT );
	kill( pid, SIGKILL );
	

	}
}
