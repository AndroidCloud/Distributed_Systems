
#include "jobtracker.h"


void
jobtracker_1(char *host)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	char *result_1;
	char * jobsubmit_1_arg;
	enum clnt_stat retval_2;
	char *result_2;
	char * getjobstatus_1_arg;
	enum clnt_stat retval_3;
	char *result_3;
	char * heartbeat_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, JOBTRACKER, JT, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	retval_1 = jobsubmit_1(&jobsubmit_1_arg, &result_1, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	retval_2 = getjobstatus_1(&getjobstatus_1_arg, &result_2, clnt);
	if (retval_2 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	retval_3 = heartbeat_1(&heartbeat_1_arg, &result_3, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	jobtracker_1 (host);
exit (0);
}
