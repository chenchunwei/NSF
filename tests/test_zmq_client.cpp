#include "testutil.hpp"

#ifdef TEST_ZMQ

#include "../src/libzmq.hpp"
#include "../foreign/libzmq/zmq.hpp"

using namespace std;

int trace;
int thread_count;
int message_count;
char *message;
char *addr;
int local;
void worker_routine(void *arg)
{
	void *requester = zmq_socket(arg, ZMQ_REQ);
#ifdef TEST_ZMQ_CLIENT_ROUTER
	zmq_connect(requester, "inproc://router");
#else
	zmq_connect(requester, addr);
#endif

	unsigned int start = clock();

	for(int i = 0; i < message_count; i++)
	{
		zmq_msg_t request;
		zmq_msg_init_size(&request, strlen(message) + 1);
		memcpy(zmq_msg_data(&request), message, strlen(message) + 1);
		zmq_send(requester, &request, 0);
		zmq_msg_close(&request);
		
		if(trace) {
			printf("return: %s\n", s_recv(requester));
		} else {
			zmq_msg_t reply;
			zmq_msg_init(&reply);
			zmq_recv(requester, &reply, 0);
		}
	}
	int cost = clock() - start;
	std::cout << "Time taken in milliseconds: " << cost << endl;
	std::cout << "message per second: " << ((float)message_count / (float)cost) * 1000 << endl;
	std::cout << "milliseconds per message: " << (float)cost / (float)message_count << endl;

	zmq_close(requester);
}
int main(int argc, char *argv[])
{
	addr = argv[1];
	message_count = atoi(argv[2]);
	message = argv[3];
	thread_count = atoi(argv[4]);
	trace = atoi(argv[5]);

	void *context = zmq_init (1);

    void *router = zmq_socket (context, ZMQ_ROUTER);
    zmq_bind(router, "inproc://router");

    void *dealer = zmq_socket (context, ZMQ_DEALER);
    zmq_connect(dealer, addr);
	zmq_connect(dealer, "tcp://127.0.0.1:8010");

	for (int i = 0; i != thread_count; i++)
		_beginthread(worker_routine, 0, context);

	//TODO:use poll instead
	zmq_device(ZMQ_QUEUE, router, dealer);
}

#endif