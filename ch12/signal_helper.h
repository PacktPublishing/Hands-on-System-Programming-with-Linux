#ifndef __SIGNAL_HELPER_H___
#define __SIGNAL_HELPER_H___

#include <signal.h>
/**
 * setup_altsigstack - Helper function to set alternate stack for sig-handler
 * @stack_sz:	required stack size
 */
void setup_altsigstack(size_t stack_sz)
{
	stack_t ss;

	printf("Alt signal stack size = %zu\n", stack_sz);
	ss.ss_sp = malloc(stack_sz);
	if (!ss.ss_sp){
		FATAL("malloc(%zu) for alt sig stack failed\n", stack_sz);
	}

	ss.ss_size = stack_sz;
	ss.ss_flags = 0;
	if (sigaltstack(&ss, NULL) == -1){
		FATAL("sigaltstack for size %zu failed!\n", stack_sz);
	}

	return ;
}
#endif /*signal_helper.h*/
