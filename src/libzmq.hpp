#ifndef __NSF_ZMQ_HPP_INCLUDED__
#define __NSF_ZMQ_HPP_INCLUDED__
#include "../foreign/libzmq/zmq.hpp"

static char *s_recv(void *socket) {
	zmq_msg_t message;
	zmq_msg_init(&message);
	zmq_recv(socket, &message, 0);
	int size = zmq_msg_size(&message);
	char *string = (char *)malloc(size + 1);
	memcpy(string, zmq_msg_data(&message), size);
	zmq_msg_close(&message);
	string[size] = 0;
	//printf("msg_data=%s\n", string);
	return (string);
}

#endif