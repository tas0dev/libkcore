#include "kcore.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


KResult k_defer_init(KDefer *d) {
	if (!d)
		return (KResult) {K_ERR_INVALID_ARG, 0};

	d->capacity = 0;
	d->count = 0;
	d->entries = malloc(sizeof(KDefer));

	if (!d->entries)
		return k_result_from_errno(errno);

	return (KResult) {K_OK, 0};
}



KResult k_defer_append(KDefer *d, KDeferFunc func, void *arg) {
	if (!d || !func) 
		return (KResult) {K_ERR_INVALID_ARG, 0};

	if (d->count == d->capacity) {
		size_t new_cap = d->capacity == 0 ? 8 : d->capacity * 2;

		KDeferEntry* tmp = realloc(d->entries, new_cap * sizeof(KDeferEntry));
		if (!tmp) 
			return (KResult) {K_ERR_NO_MEMORY, errno};

		d->entries = tmp;
		d->capacity = new_cap;
	}

	d->entries[d->count++] = (KDeferEntry){func, arg};

	return (KResult) {K_OK, 0};
}


void k_defer_exec(KDefer* d) {
	if (!d) return;

	for (size_t i = d->count; i > 0; i--) {
		KDeferEntry e = d->entries[i - 1];
		e.func(e.arg);
	}

	d->count = 0;
}


void k_defer_free(KDefer *d) {
	if (!d) return;

	free(d->entries);
	d->capacity = 0;
	d->count = 0;
}


void k_defer_exec_free(KDefer *d) {
	k_defer_exec(d);
	k_defer_free(d);
}


