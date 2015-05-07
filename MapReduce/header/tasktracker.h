
#ifndef _TASKTRACKER_H_RPCGEN
#define _TASKTRACKER_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define TASKTRACKER 0x20110003
#define TT 1

#if defined(__STDC__) || defined(__cplusplus)
#define mapper 1
extern  char ** mapper_1(char **, CLIENT *);
extern  char ** mapper_1_svc(char **, struct svc_req *);
#define reducer 2
extern  char ** reducer_1(char **, CLIENT *);
extern  char ** reducer_1_svc(char **, struct svc_req *);
#define reducerInput 3
extern  char ** reducerinput_1(char **, CLIENT *);
extern  char ** reducerinput_1_svc(char **, struct svc_req *);
extern int tasktracker_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define mapper 1
extern  char ** mapper_1();
extern  char ** mapper_1_svc();
#define reducer 2
extern  char ** reducer_1();
extern  char ** reducer_1_svc();
#define reducerInput 3
extern  char ** reducerinput_1();
extern  char ** reducerinput_1_svc();
extern int tasktracker_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TASKTRACKER_H_RPCGEN */
