
#ifndef _NAMENODE_H_RPCGEN
#define _NAMENODE_H_RPCGEN

#include <rpc/rpc.h>
#ifdef __cplusplus
extern "C" {
#endif


#define NAMENODE 0x20110000
#define NN 1

#if defined(__STDC__) || defined(__cplusplus)
#define openFile 1
extern  char ** openfile_1(char **, CLIENT *);
extern  char ** openfile_1_svc(char **, struct svc_req *);
#define getBlockLocations 2
extern  char ** getblocklocations_1(char **, CLIENT *);
extern  char ** getblocklocations_1_svc(char **, struct svc_req *);
#define assignBlock 3
extern  char ** assignblock_1(char **, CLIENT *);
extern  char ** assignblock_1_svc(char **, struct svc_req *);
#define closeFile 4
extern  char ** closefile_1(char **, CLIENT *);
extern  char ** closefile_1_svc(char **, struct svc_req *);
#define list 5
extern  char ** list_1(char **, CLIENT *);
extern  char ** list_1_svc(char **, struct svc_req *);
#define sendBlockReport 8
extern  char ** sendblockreport_1(char **,CLIENT *);
extern  char ** sendblockreport_1_svc(char **,struct svc_req *);
#define sendHeartBeat 9
extern  char ** sendheartbeat_1(char **, CLIENT *);
extern  char ** sendheartbeat_1_svc(char **, struct svc_req *);
extern int namenode_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define openFile 1
extern  char ** openfile_1();
extern  char ** openfile_1_svc();
#define getBlockLocations 2
extern  char ** getblocklocations_1();
extern  char ** getblocklocations_1_svc();
#define assignBlock 3
extern  char ** assignblock_1();
extern  char ** assignblock_1_svc();
#define closeFile 4
extern  char ** closefile_1();
extern  char ** closefile_1_svc();
#define list 5
extern  char ** list_1();
extern  char ** list_1_svc();
#define sendBlockReport 8
extern  char ** sendblockreport_1();
extern  char ** sendblockreport_1_svc();
#define sendHeartBeat 9
extern  char ** sendheartbeat_1();
extern  char ** sendheartbeat_1_svc();
extern int namenode_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_NAMENODE_H_RPCGEN */
