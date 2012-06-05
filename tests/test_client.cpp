#include "testutil.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	cout << "client node init" << endl;

	void *n = nsf_init();

	int rc;
	char *addr = "tcp://127.0.0.1:8010";
	nsf_msg_call_t call={ "test_service", "echo" };
	
	printf("call=%s|%s\n", call.service, call.method);
	
	for(int i = 0; i<1000; i++)
		printf("result: %s\n", nsf_invoke(n, addr, &call, rc));

	return 0;
}