#include "../include/nsf.h"

namespace nsf{
	class node
	{
	private:
		void *libzmq;
		char *bind_addr;
		char *associate_addr;
		int heartbeat;
		nsf_id_t *id;
	public:
		node();
		~node();

		int close();
		int associate(char *addr);
		void set_heartbeat(int h);
		void set_id(nsf_id_t *id);
	};
}