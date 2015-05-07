
#ifndef _JOBTRACKER_H_RPCGEN
#define _JOBTRACKER_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


#define JOBTRACKER 0x20110002
#define JT 1

#if defined(__STDC__) || defined(__cplusplus)
#define jobSubmit 1
extern  enum clnt_stat jobsubmit_1(char **, char **, CLIENT *);
extern  bool_t jobsubmit_1_svc(char **, char **, struct svc_req *);
#define getJobStatus 2
extern  enum clnt_stat getjobstatus_1(char **, char **, CLIENT *);
extern  bool_t getjobstatus_1_svc(char **, char **, struct svc_req *);
#define heartBeat 3
extern  enum clnt_stat heartbeat_1(char **, char **, CLIENT *);
extern  bool_t heartbeat_1_svc(char **, char **, struct svc_req *);
extern int jobtracker_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define jobSubmit 1
extern  enum clnt_stat jobsubmit_1();
extern  bool_t jobsubmit_1_svc();
#define getJobStatus 2
extern  enum clnt_stat getjobstatus_1();
extern  bool_t getjobstatus_1_svc();
#define heartBeat 3
extern  enum clnt_stat heartbeat_1();
extern  bool_t heartbeat_1_svc();
extern int jobtracker_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_JOBTRACKER_H_RPCGEN */
