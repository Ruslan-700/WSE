#include "rgl_memory.h"

using namespace rgl;

unsigned int register_store::esp = 0;
x86_register_set register_store::sets[MAX_RECURSIVE_CALLS];
x86_register_set *register_store::current_set = &sets[MAX_RECURSIVE_CALLS];

unsigned int register_store_net::esp = 0;
x86_register_set register_store_net::sets[MAX_RECURSIVE_CALLS];
x86_register_set *register_store_net::current_set = &sets[MAX_RECURSIVE_CALLS];

unsigned int register_store_gfx::esp = 0;
x86_register_set register_store_gfx::sets[MAX_RECURSIVE_CALLS];
x86_register_set *register_store_gfx::current_set = &sets[MAX_RECURSIVE_CALLS];

void *rgl::_alloc(size_t size)
{
	return rgl_new_func(size);
}

void rgl::_free(void *ptr)
{
	if (!ptr)
		return;

	rgl_delete_func(ptr);
}
