#include "node.h"
#include "../foreign/libzmq/zmq.h"

nsf::node::node()
{
	//this->libzmq=zmq_init(1);
}

nsf::node::~node()
{

}

int nsf::node::close()
{
	return -1;
}

int nsf::node::associate(char *addr)
{
	this->associate_addr=addr;
	return 1;
}

void nsf::node::set_heartbeat(int h)
{
	this->heartbeat=h;
}

void nsf::node::set_id(nsf_id_t *id)
{
	this->id=id;
}