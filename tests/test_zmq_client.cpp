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
	int zero = 0;
	void *requester = zmq_socket(arg, ZMQ_REQ);
	zmq_setsockopt (requester, ZMQ_LINGER, &zero, sizeof(zero));
#ifdef TEST_ZMQ_CLIENT_ROUTER
	zmq_connect(requester, "inproc://router");
#else
	zmq_connect(requester, addr);
#endif

	unsigned int start = clock();
	for(int i = 0; i < message_count; i++)
	{
		int j = 0;
		while(j++<1){
			zmq_msg_t request;
			zmq_msg_init_size(&request, strlen(message) + 1);
			memcpy(zmq_msg_data(&request), message, strlen(message) + 1);
			zmq_send(requester, &request, 0);
			zmq_msg_close(&request);

			zmq_pollitem_t items[] = { { requester, 0, ZMQ_POLLIN, 0 } };
			zmq_poll (items, 1, 2000 * 1000);
			if (items [0].revents & ZMQ_POLLIN) {
				if(trace) {
					printf("return: %s\n", s_recv(requester));
				} else {
					zmq_msg_t reply;
					zmq_msg_init(&reply);
					zmq_recv(requester, &reply, 0);
				}
				break;
			} else {
				printf("no response from %s, retry\n", addr);
				zmq_close(requester);
				requester = zmq_socket(arg, ZMQ_REQ);
				zmq_setsockopt (requester, ZMQ_LINGER, &zero, sizeof (zero));
				zmq_connect(requester, addr);
			}
		}
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
	trace = atoi(argv[5]);

	void *context = zmq_init (1);

#ifdef TEST_ZMQ_CLIENT_ROUTER
	void *router = zmq_socket (context, ZMQ_ROUTER);
	zmq_bind(router, "inproc://router");

	void *dealer = zmq_socket (context, ZMQ_DEALER);
	zmq_connect(dealer, addr);
	zmq_connect(dealer, "tcp://127.0.0.1:8010");
#endif

	for (int i = 0; i != thread_count; i++)
		_beginthread(worker_routine, 0, context);
#ifdef TEST_ZMQ_CLIENT_ROUTER
	zmq_device(ZMQ_QUEUE, router, dealer);
#endif

	char a;
	cin >> a;
}

#endif