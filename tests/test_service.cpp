#include "testutil.hpp"
#include "../src/libzmq.hpp"
#include "../foreign/libzmq/zmq.hpp"

#include <assert.h>
#include <string.h>
#include <iostream>
#include <process.h>

using namespace std;

int trace = 0;

//NSF
//int main(int argc, char *argv[])
//{
//	cout << "service node init" << endl;
//
//	void *n = nsf_init();
//
//	nsf_export(n, "test_service");
//	nsf_bind(n, "tcp://127.0.0.1:8010");
//
//	while(1)
//	{
//		nsf_msg_call_t call;
//		nsf_recv_call(n, call);
//		
//		printf("service: %s\n", call.service);
//		printf("method: %s\n", call.method);
//		
//		nsf_reply(n, "hello world!");
//	}
//
//	return 0;
//}


//zmq recv
int main(int argc, char *argv[])
{
	trace = atoi(argv[1]);

	char *addr = "tcp://*:8010";
	void *ctx = zmq_init(1);
	void *responser = zmq_socket(ctx, ZMQ_REP);
	zmq_bind(responser, addr);

	cout << "run at " << addr << endl;

	while(1)
	{
		char *r = s_recv(responser);
		
		if(trace)
			printf("recv: %s\n", r);
		zmq_msg_t reply;
		zmq_msg_init_size(&reply, strlen(r) + 1);
		memcpy(zmq_msg_data(&reply), r, strlen(r) + 1);
		zmq_send(responser, &reply, 0);
		zmq_msg_close(&reply);
	}
}


//zmq poll
//int main(int argc, char *argv[])
//{
//	char *addr = "tcp://127.0.0.1:8010";
//	void *ctx = zmq_init(1);
//	void *responser = zmq_socket(ctx, ZMQ_XREP);
//	zmq_bind(responser, addr);
//
//	cout << "run at " << addr << endl;
//
//	const int n = 1;
//	zmq_pollitem_t items[n];
//	items[0].socket = responser;
//	items[0].events = ZMQ_POLLIN;
//
//	/*items[1].socket = responser;
//	items[1].events = ZMQ_POLLOUT;*/
//
//	while(1)
//	{
//		printf("polling...\n");
//		zmq_poll(items, n, -1);
//
//		if (items [0].revents & ZMQ_POLLIN) 
//		{
//			printf("get item \n");
//			char *r = s_recv(responser);
//			printf("recv: %s\n", r);
//			s_send(responser, r);
//			printf("reply: %s\n", r);
//		}
//	}
//}

/*
//zmq worker
void worker_routine (void *arg)
{
	zmq::context_t *ctx = (zmq::context_t*)arg;
	zmq::socket_t s(*ctx, ZMQ_REP);
	s.connect("inproc://workers");

	cout << "worker running" << endl;

	while (true) {
		if(trace)
			printf("work_%d waiting...\n", GetCurrentThreadId());
		char *r = s_recv(s);
		if(trace)
			printf("work_%d recv: %s\n", GetCurrentThreadId(), r);
		Sleep(1);
		s_send(s, r);
		if(trace)
			printf("work_%d reply: %s\n", GetCurrentThreadId(), r);
		//delete r;
	}
}
int main(int argc, char *argv[])
{
	trace = atoi(argv[1]);

	zmq::context_t ctx(1);

	zmq::socket_t clients(ctx, ZMQ_XREP);
	//clients.bind("tcp://10.13.23.84:8010");
	clients.bind("tcp://*:8010");
	cout << "tcp frontend at tcp://*:8010" << endl;

	zmq::socket_t workers(ctx, ZMQ_XREQ);
	workers.bind("inproc://workers");
	cout << "inproc backend" << endl;

	for (int i = 0; i != 10; i++)
		_beginthread(worker_routine, 0, &ctx);

	cout << "workers ready" << endl;

	zmq::device(ZMQ_QUEUE, clients, workers);

	return 0;
}
*/