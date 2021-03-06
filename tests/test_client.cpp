#include "testutil.hpp"

#ifndef TEST_ZMQ

#include <iostream>
#include <time.h>

#include "../src/libzmq.hpp"
#include "../foreign/libzmq/zmq.hpp"
#include <process.h>

using namespace std;

//int main(int argc, char *argv[])
//{
//	int count = atoi(argv[1]);
//
//	cout << "client node init" << endl;
//
//	void *n = nsf_init();
//
//	int rc;
//	char *addr = "tcp://127.0.0.1:8010";
//	nsf_msg_call_t call={ "test_service", "echo" };
//	
//	printf("call=%s|%s\n", call.service, call.method);
//	
//	unsigned int start = clock();
//	for(int i = 0; i< count; i++)
//		printf("result: %s\n", nsf_invoke(n, addr, &call, rc));
//	std::cout << "Time taken in millisecs: " << clock() - start;
//	return 0;
//}

int thread_count;
int message_count;
char *message;
char *addr;

//no router
//void worker_routine(void *arg)
//{
//	zmq::context_t *ctx = (zmq::context_t *)arg;
//	zmq::socket_t s (*ctx, ZMQ_REQ);
//	s.connect (addr);
//
//	unsigned int start = clock();
//
//	for (int i = 0; i != message_count; i++) {
//		zmq::message_t request(strlen(message) + 1);
//		memcpy(request.data(), message, strlen(message) + 1);
//		s.send (request);
//		zmq::message_t reply;
//		s.recv(&reply);
//	}
//
//	int cost = clock() - start;
//	std::cout << "Time taken in milliseconds: " << cost << endl;
//	std::cout << "message per second: " << ((float)message_count / (float)cost) * 1000 << endl;
//	std::cout << "milliseconds per message: " << (float)cost / (float)message_count << endl;
//}
void worker_routine_sync(void *arg)
{
	void *ctx = (zmq::context_t *)arg;
	void *requester = zmq_socket(ctx, ZMQ_REQ);
	zmq_connect(requester, addr);

	unsigned int start = clock();

	for(int i = 0; i < message_count; i++)
	{
		zmq_msg_t request;
		zmq_msg_init_size(&request, strlen(message) + 1);
		memcpy(zmq_msg_data(&request), message, strlen(message) + 1);
		zmq_send(requester, &request, 0);
		//printf("send: %s\n", message);
		zmq_msg_close(&request);
		s_recv(requester);
		//printf("return: %s\n", s_recv(requester));
	}
	int cost = clock() - start;
	std::cout << "Time taken in milliseconds: " << cost << endl;
	std::cout << "message per second: " << ((float)message_count / (float)cost) * 1000 << endl;
	std::cout << "milliseconds per message: " << (float)cost / (float)message_count << endl;

	zmq_close(requester);
}
void worker_routine_async(void *arg)
{
	void *ctx = (zmq::context_t *)arg;
	void *requester = zmq_socket(ctx, ZMQ_XREQ);//ZMQ_XREQ?
	zmq_connect(requester, addr);

	zmq_pollitem_t items[] = { { requester, 0, ZMQ_POLLIN, 0 }, { requester, 0, ZMQ_POLLOUT, 0 } };
	
	int i = 0;

	unsigned int start = clock();
	while (i < message_count) {
		s_send(requester, message);
		//printf("send: %s\n", message);

		int centitick;
		for (centitick = 0; centitick < 100; centitick++) {
			zmq_poll(items, 2, 1);
			if(!items[0].revents && !items[1].revents)
				continue;
			//printf("item 0 polled: %d\n", items[0].revents);
			//printf("item 1 polled: %d\n", items[1].revents);
			if (items[0].revents & ZMQ_POLLIN) {
				i++;
				//s_recv(requester);
				printf("return: %s\n", s_recv(requester));
			}
			if (items[1].revents & ZMQ_POLLOUT) {
				i++;
				//s_send(requester, message);
				//printf("send: %s\n", message);
			}
		}
	}
	int cost = clock() - start;
	std::cout << "Time taken in milliseconds: " << cost << endl;
	std::cout << "message per second: " << ((float)message_count / (float)cost) * 1000 << endl;
	std::cout << "milliseconds per message: " << (float)cost / (float)message_count << endl;

	zmq_close(requester);
	zmq_term(ctx);
}
//int main(int argc, char *argv[])
//{
//	addr = argv[1];
//	message_count = atoi(argv[2]);
//	message = argv[3];
//	thread_count = atoi(argv[4]);
//
//	void *ctx = zmq_init(1);
//
//	if(thread_count <= 1) {
//		worker_routine_sync(ctx);
//		return 0;
//	}
//
//	for (int i = 0; i != thread_count; i++){
//		//_beginthread(worker_routine_sync, 0, ctx);
//		//_beginthread(worker_routine_async, 0, ctx);
//		_beginthread(worker_routine_sync, 0, ctx);
//	}
//
//	char c;
//	cin >> c;
//
//	return 0;
//}

//local router
void worker_routine(void *arg)
{
	void *ctx = (zmq::context_t *)arg;
	void *requester = zmq_socket(ctx, ZMQ_REQ);
	zmq_connect(requester, "inproc://router");
	
	unsigned int start = clock();
	for(int i = 0; i < message_count; i++)
	{
		zmq_msg_t request;
		zmq_msg_init_size(&request, strlen(message) + 1);
		memcpy(zmq_msg_data(&request), message, strlen(message) + 1);
		zmq_send(requester, &request, 0);
		zmq_msg_close(&request);
		char *r = s_recv(requester);
		if(strlen(r) <= 0)
			printf("wrong return: %s\n", r);
	}
	int cost = clock() - start;
	std::cout << "Time taken in milliseconds: " << cost << endl;
	std::cout << "message per second: " << ((float)message_count / (float)cost) * 1000 << endl;
	std::cout << "milliseconds per message: " << (float)cost / (float)message_count << endl;
}
int main(int argc, char *argv[])
{
	addr = argv[1];
	message_count = atoi(argv[2]);
	message = argv[3];
	thread_count = atoi(argv[4]);

	void *context = zmq_init (1);

    void *router = zmq_socket (context, ZMQ_ROUTER);
    zmq_bind(router, "inproc://router");

    void *dealer = zmq_socket (context, ZMQ_DEALER);
    zmq_connect(dealer, addr);

	for (int i = 0; i != thread_count; i++)
		_beginthread(worker_routine, 0, context);

    zmq_device(ZMQ_QUEUE, router, dealer);

	char c;
	cin >> c;
}

//remote router

#endif