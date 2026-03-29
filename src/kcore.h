#ifndef _KCORE_H
#define _KCORE_H

#define K_FAILED(r) ((r).code != K_OK)
#define K_SUCCEEDED(r) ((r).code == K_OK)

#define K_WRAP(expr, label) \
  do {                      \
    KResult _r = (expr);    \
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



#endif /* _KCORE_H */
