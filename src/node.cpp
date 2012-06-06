#include "platform.hpp"
#include "node.hpp"
#include "libzmq.hpp"

#include <string.h>

using namespace std;

nsf::node_t::node_t()
{

}

nsf::node_t::~node_t()
{

}

int nsf::node_t::set_heartbeat(int h)
{
	this->heartbeat = h;
	return 1;
}

int nsf::node_t::set_id(nsf_id_t *id)
{
	this->id = id;
	return 1;
}

int nsf::node_t::associate(char *addr)
{
	this->associate_addr = addr;
	return 1;
}

int nsf::node_t::export_service(char *service)
{
	nsf_service_t s = { service };
	this->local.push_back(s);
	return 1;
}

int nsf::node_t::close()
{
	if(this->libzmq_s)
		zmq_close(this->libzmq_s);
	if(libzmq_ctx)
		zmq_term(this->libzmq_ctx);
	return 1;
}

int nsf::node_t::bind(char *addr)
{
	if(!this->bind_addr)
		return -1;

	this->bind_addr = addr;

	this->libzmq_ctx = zmq_init(1);
	this->libzmq_s = zmq_socket(this->libzmq_ctx, ZMQ_REP);
	zmq_bind(this->libzmq_s, addr);

	return 1;
}

int nsf::node_t::reply(char *r)
{
	zmq_msg_t reply;
	zmq_msg_init_size(&reply, strlen(r) + 1);
	memcpy(zmq_msg_data(&reply), r, strlen(r) + 1);
	zmq_send(this->libzmq_s, &reply, 0);
	zmq_msg_close(&reply);

	return 1;
}

char *nsf::node_t::invoke(nsf_msg_call_t *call, int &rc)
{
	//service not found
	if(this->all.find(call->service) == this->all.end())
	{
		rc = -1;
		return NULL;
	}
	//do not have valid addr
	if(this->all[call->service]->addr_count <= 0)
	{
		rc = -1;
		return NULL;
	}

	return invoke(this->all[call->service]->addrs[0], call, rc);
}

char *nsf::node_t::invoke(char *addr, nsf_msg_call_t *call, int &rc)
{
	void *context = zmq_init (1);
	void *requester = zmq_socket (context, ZMQ_REQ);
	zmq_connect (requester, addr);

	zmq_msg_t request;
	zmq_msg_init_size (&request, strlen(call->service)+1);
	memcpy(zmq_msg_data(&request), call->service, strlen(call->service)+1);
	zmq_send(requester, &request, 0);
	zmq_msg_close(&request);

	char *r = s_recv(requester);

	zmq_close(requester);
	zmq_term(context);	

	return r;
}

int nsf::node_t::recv_call(nsf_msg_call_t &call_)
{
	if(!this->libzmq_s)
		return -1;

	char *msg = s_recv(this->libzmq_s);
	/*zmq_msg_t request;
	zmq_msg_init (&request);
	zmq_recv(this->libzmq_s, &request, 0);
	msg = (char *)zmq_msg_data(&request);
	zmq_msg_close(&request);*/

	nsf_msg_call_t call= { msg };
	call_ = call;

	return 1;
}

