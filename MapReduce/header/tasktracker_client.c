
#include "tasktracker.h"


void
tasktracker_1(char *host)
{
	CLIENT *clnt;
	char * *result_1;
	char * mapper_1_arg;
	char * *result_2;
	char * reducer_1_arg;
	char * *result_3;
	char * reducerinput_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, TASKTRACKER, TT, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = mapper_1(&mapper_1_arg, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = reducer_1(&reducer_1_arg, clnt);
	if (result_2 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = reducerinput_1(&reducerinput_1_arg, clnt);
	if (result_3 == (char **) NULL) {
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
	tasktracker_1 (host);
exit (0);
}
