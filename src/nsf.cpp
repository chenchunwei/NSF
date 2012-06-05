#include "platform.hpp"

#include "../include/nsf.h"
#include "node.hpp"

void *nsf_init()
{
	return (void*)new nsf::node_t();
}

int nsf_close(void *n)
{
	return ((nsf::node_t*)n)->close();
}

int nsf_associate(void *n, char *addr)
{
	return ((nsf::node_t*)n)->associate(addr);
}

int nsf_set_id( void *n, nsf_id_t *id )
{
	return ((nsf::node_t*)n)->set_id(id);
}

int nsf_set_heartbeat( void *n, int h )
{
	return ((nsf::node_t*)n)->set_heartbeat(h);
}

int nsf_bind(void *n, char *addr)
{
	return ((nsf::node_t*)n)->bind(addr);
}

int nsf_export(void *n, char *service)
{
	return ((nsf::node_t*)n)->export_service(service);
}

char *nsf_invoke(void *n, nsf_msg_call_t *call, int &rc)
{
	return ((nsf::node_t*)n)->invoke(call, rc);
}

char *nsf_invoke(void *n, char *addr, nsf_msg_call_t *call, int &rc)
{
	return ((nsf::node_t*)n)->invoke(addr, call, rc);
}

int nsf_recv_call(void *n, nsf_msg_call_t &call)
{
	return ((nsf::node_t*)n)->recv_call(call);
}

int nsf_reply(void *n, char* r)
{
	return ((nsf::node_t*)n)->reply(r);
}
