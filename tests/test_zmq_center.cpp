#include "testutil.hpp"

#ifdef TEST_ZMQ

#include "../src/libzmq.hpp"
#include "../foreign/libzmq/zmq.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	void *context = zmq_init(1);

	void *frontend = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(frontend, "tcp://*:8020");
	cout << "broker frontend run at tcp://*:8020" << endl;

	void *backend = zmq_socket(context, ZMQ_DEALER);
	zmq_bind(backend, "tcp://*:8030");
	cout << "broker backend run at tcp://*:8030" << endl;

	zmq_device(ZMQ_QUEUE, frontend, backend);
}

//int main(int argc, char *argv[])
//{
//	void *ctx = zmq_init(1);
//
//	void *frontend = zmq_socket(ctx, ZMQ_ROUTER);
//	zmq_bind(frontend, "tcp://*:8020");
//
//	void *backend = zmq_socket(ctx, ZMQ_DEALER);
//	zmq_bind(backend, "tcp://*:8030");
//
//	while(1){
//		zmq_pollitem_t items[] ={ 
//			{ frontend, 0, ZMQ_POLLIN, 0 },
//			{ backend, 0, ZMQ_POLLIN, 0 }
//		};
//		zmq_poll(items, 2, -1);
//
//		//front
//		if(items[0].revents & ZMQ_POLLIN) {
//			char * msg = s_recv(frontend);
//			printf("frontend recv: %s\n", msg);
//			s_send(backend, msg);
//			/*char *client_addr = s_recv(frontend);
//			printf("frontend recv: %s\n", client_addr);
//
//			char *empty = s_recv(frontend);
//			printf("frontend recv: %s\n", empty);
//			assert (empty [0] == 0);
//			free (empty);
//
//			char *request = s_recv (frontend);
//			printf("frontend recv: %s\n", request);
//
//			s_send_more(backend, "1");
//			s_send_more(backend, "");
//			s_send_more(backend, client_addr);
//			s_send_more(backend, "");
//			s_send(backend, request);*/
//		}
//		//backend
//		if(items[1].revents & ZMQ_POLLIN) {
//			//printf("backend recv: %s\n", s_recv(backend));
//			char *worker_addr = s_recv(backend);
//			printf("recv: %s\n", worker_addr);
//
//			char *empty = s_recv (backend);
//			printf("recv: %s\n", empty);
//			assert (empty [0] == 0);
//			free (empty);
//
//			char *client_addr = s_recv (backend);
//		}
//	}
//}
#endif