//libnsf by wskyhx@gmail.com
#ifndef __NSF_H_INCLUDED__
#define __NSF_H_INCLUDED__

#define NSF_MAX_ARGV_SIZE 20

typedef struct
{
	char *source;
	char *key;
} nsf_id_t;

typedef struct
{
	char *addr;
	char *service;
	char *method;
	char *argv [NSF_MAX_ARGV_SIZE];
} nsf_msg_call_t;

void *nsf_init();
int nsf_close(void *n);

int nsf_associate(void *n, char *addr);
void nsf_set_id(void *n, nsf_id_t *id);
void nsf_set_heartbeat(void *n, int h);

int nsf_export(void *n, char *service);
int nsf_bind(void *n, char *addr);

int nsf_invoke(void *n, nsf_msg_call_t *call);
int nsf_invoke(void *n, nsf_msg_call_t *call, char *r);
int nsf_reply(void *n);
int nsf_reply(void *n, char* r);
int nsf_recv_call(void *n, nsf_msg_call_t *call);
int nsf_recv_reply(void *n);
int nsf_recv_relpy(void *n, char *r);

#endif