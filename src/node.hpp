#ifndef __NSF_NODE_HPP_INCLUDED__
#define __NSF_NODE_HPP_INCLUDED__

#include "../include/nsf.h"
#include "thread.hpp"

#include <vector>
#include <map>
#include <string>

using namespace std;

namespace nsf{
	class node_t
	{
	private:
		void *libzmq_ctx;
		void *libzmq_s;

		char *bind_addr;
		char *associate_addr;
		int heartbeat;
		nsf_id_t *id;

		vector<nsf_service_t> local;
		vector<nsf_service_t> client;
		map<string, nsf_service_t *> all;
		
		thread_t worker;


	public:
		node_t();
		~node_t();

		int set_heartbeat(int h);
		int set_id(nsf_id_t *id);
		int associate(char *addr);
		int export_service(char *service);

		int close();

		int bind(char *addr);
		int recv_call(nsf_msg_call_t &call);
		int reply(char *r);

		char *invoke(nsf_msg_call_t *call, int &rc);
		char *invoke(char *addr, nsf_msg_call_t *call, int &rc);
	};
}

#endif