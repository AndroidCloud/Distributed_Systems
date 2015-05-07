
#ifndef _DATANODE_H_RPCGEN
#define _DATANODE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define DATANODE 0x20110001
#define DN 1

#if defined(__STDC__) || defined(__cplusplus)
#define readBlock 6
extern  char ** readblock_1(char **, CLIENT *);
extern  char ** readblock_1_svc(char **, struct svc_req *);
#define writeBlock 7
extern  char ** writeblock_1(char **, CLIENT *);
extern  char ** writeblock_1_svc(char **, struct svc_req *);
extern int datanode_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define readBlock 6
extern  char ** readblock_1();
extern  char ** readblock_1_svc();
#define writeBlock 7
extern  char ** writeblock_1();
extern  char ** writeblock_1_svc();
extern int datanode_1_freeresult ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_DATANODE_H_RPCGEN */
