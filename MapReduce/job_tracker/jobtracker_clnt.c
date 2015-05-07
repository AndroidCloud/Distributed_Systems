
#include <memory.h> /* for memset */
#include "jobtracker.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

enum clnt_stat 
jobsubmit_1(char **argp, char **clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, jobSubmit,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
getjobstatus_1(char **argp, char **clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, getJobStatus,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
heartbeat_1(char **argp, char **clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, heartBeat,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) clnt_res,
		TIMEOUT));
}
