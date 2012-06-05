//libnsf by wskyhx@gmail.com
#ifndef __NSF_H_INCLUDED__
#define __NSF_H_INCLUDED__

#if defined _WIN32
#   if defined DLL_EXPORT
#       define ZMQ_EXPORT __declspec(dllexport)
#   else
#       define NSF_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define NSF_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define NSF_EXPORT __attribute__ ((visibility("default")))
#   else
#       define NSF_EXPORT
#   endif
#endif


typedef struct
{
	char *source;
	char *key;
} nsf_id_t;

typedef struct
{
	char *service;
	char *method;
	char *argv[];
} nsf_msg_call_t;

typedef struct
{
	char *name;
	short addr_count;
	char *addrs[];
} nsf_service_t;

NSF_EXPORT void *nsf_init();
NSF_EXPORT int nsf_close(void *n);

NSF_EXPORT int nsf_associate(void *n, char *addr);
NSF_EXPORT int nsf_set_id(void *n, nsf_id_t *id);
NSF_EXPORT int nsf_set_heartbeat(void *n, int h);

NSF_EXPORT int nsf_bind(void *n, char *addr);
NSF_EXPORT int nsf_export(void *n, char *service);

NSF_EXPORT char *nsf_invoke(void *n, char *addr, nsf_msg_call_t *call, int &rc);
NSF_EXPORT char *nsf_invoke(void *n, nsf_msg_call_t *call, int &rc);

NSF_EXPORT int nsf_recv_call(void *n, nsf_msg_call_t &call);
//NSF_EXPORT int nsf_recv_reply(void *n);
//NSF_EXPORT int nsf_recv_relpy(void *n, char *r);

//NSF_EXPORT int nsf_reply(void *n);
NSF_EXPORT int nsf_reply(void *n, char* r);

#undef NSF_EXPORT
#endif