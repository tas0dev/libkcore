#ifndef _KCORE_H
#define _KCORE_H
#include <stddef.h>

#define K_FAILED(r) ((r).code != K_OK)
#define K_SUCCEEDED(r) ((r).code == K_OK)

#define K_WRAP(expr, label) \
  do {                      \ KResult _r = (expr);    \
    if (_r.code != K_OK) {  \
      k_result_print(_r);   \
      goto label;           \
    }                       \
  } while (0)

#define K_WRAP_OR_GOTO(expr_call, on_error, label) \
  do {                                             \
    KResult _r = (expr_call);                      \
    if (_r.code != K_OK) {                         \
      on_error;                                    \
      goto label;                                  \
    }                                              \
  } while (0)

#define K_CATCH(res, expr_call, on_error) \
  do {                                    \
    (res) = (expr_call);                  \
    if ((res).code != K_OK) {             \
      on_error;                           \
    }                                     \
  } while (0)

/*********** KResult ***********/

typedef enum {
  K_OK = 0,
  K_ERR_FILE_NOT_FOUND = 1,
  K_ERR_PERMISSION,
  K_ERR_IS_DIRECTORY,
  K_ERR_NO_SPACE,
  K_ERR_NO_MEMORY,
  K_ERR_INVALID_ARG,
  K_ERR_IO,
  K_ERR_ALREADY_OPEN,
  K_ERR_NOT_OPEN,
  K_ERR_UNKNOWN,
} K_RESULT;

typedef struct {
  K_RESULT code;
  int sys_errno;
} KResult;


/************ KDefer *************/
typedef void (*KDeferFunc)(void*);

typedef struct {
	KDeferFunc func;
	void* arg;
} KDeferEntry;

typedef struct {
	KDeferEntry* entries;
	size_t count;
	size_t capacity;
} KDefer;

/*********** Functions ***********/ 
/* KResult */
KResult k_result_from_errno(int errno);
const char* k_result_str(KResult result);
void k_result_print(KResult r);

/* KDefer */
KResult k_defer_init(KDefer* d);
KResult k_defer_append(KDefer* d, KDeferFunc func, void* arg);
#define k_defer(d, func, arg) k_defer_append(d, func, arg)
void k_defer_exec(KDefer* d);
void k_defer_free(KDefer* d);
void k_defer_exec_free(KDefer* d);

#endif /* _KCORE_H */
