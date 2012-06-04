#include "../include/nsf.h"
#include "node.h"

void *nsf_init()
{
	return (void*)new nsf::node();
}

int nsf_close(void *n)
{
	return ((nsf::node*)n)->close();
}

int nsf_associate(void *n, char *addr)
{
	return ((nsf::node*)n)->associate(addr);
}