#include "testutil.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	cout << "service node init" << endl;

	void *n = nsf_init();

	nsf_export(n, "test_service");
	nsf_bind(n, "tcp://127.0.0.1:8010");

	while(1)
	{
		nsf_msg_call_t call;
		nsf_recv_call(n, call);
		
		printf("service: %s\n", call.service);
		printf("method: %s\n", call.method);
		
		nsf_reply(n, "hello world!");
	}

	return 0;
}