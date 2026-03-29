#include "kcore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
	KDefer df;
	KResult res;

	K_CATCH(res,
			k_defer_init(&df),
			exit(res.sys_errno)
	);

	const char* static_str = "If the Address Sanitizer does not report a memory leak when this program terminates, KDefer is working correctly.";

	char* string = malloc(sizeof(char) * strlen(static_str) + 1);
	k_defer(&df, free, string); // execute free on end of function

	strcpy(string, static_str);
	printf("%s\n", static_str);

	k_defer_exec_free(&df);
}


