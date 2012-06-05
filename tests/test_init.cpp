#include "testutil.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	char *addr = "inproc://nsf_center";
	char *service = "test_service";

	cout << "center node init" << endl;
	void* n_center = nsf_init();
	//bind
	nsf_bind(n_center, addr);

	cout << "service node init" << endl;
	void* n_service = nsf_init();
	nsf_associate(n_service, addr);
	nsf_bind(n_service, "inproc://nsf_service");
	//export
	nsf_export(n_service, service);

	cout << "client node init" << endl;
	void* n_client = nsf_init();
	nsf_associate(n_client, addr);
	//invoke
	nsf_msg_call_t call = { service, "echo", { "hello", "world" } };
	char *r = 0;
	nsf_invoke(n_client, &call, r);

	return 0;
}