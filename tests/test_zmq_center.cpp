#include "testutil.hpp"

#ifdef TEST_ZMQ

#include "../src/libzmq.hpp"
#include "../foreign/libzmq/zmq.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	char *addr = argv[1];
	void *context = zmq_init(1);

	void *router = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(router, "tcp://*:8020");

	void *dealer = zmq_socket(context, ZMQ_DEALER);
	zmq_connect(dealer, addr);

	zmq_device(ZMQ_QUEUE, router, dealer);
}

#endif