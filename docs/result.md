# kresult
kresult provides an error handling mechanism for the C language.
It allows functions to return detailed error information including error codes and system errno values, making it easier to track and handle errors consistently across your codebase.

## Types
- KResult
```c
typedef struct {
  K_RESULT code;
  int sys_errno;
} KResult;
```
The result type that wraps both an error code and optional system errno. Functions return this to indicate success or failure with detailed error information.

### K_RESULT
```c
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
```
Error code enumeration. `K_OK(0)` indicates success, and other values indicate specific error conditions.

## Macros
### K_FAILED()
```c
#define K_FAILED(r) ((r).code != K_OK)
```
Returns true if the result indicates an error.

### K_SUCCEEDED()
```c
#define K_SUCCEEDED(r) ((r).code == K_OK)
```
Returns true if the result indicates success.

### K_WRAP()
```c
#define K_WRAP(expr, label)
```
Evaluates the expression, and if it fails, prints the error and jumps to the specified label.

Arguments:
- expr - Expression that returns KResult
- label - Label to jump to on error

### K_WRAP_OR_GOTO()
```c
#define K_WRAP_OR_GOTO(expr_call, on_error, label)
```
Evaluates the expression, and if it fails, executes the on_error statement before jumping to the label.

Arguments:
- expr_call - Expression that returns KResult
- on_error - Statement to execute on error
- label - Label to jump to on error

### K_CATCH()
```c
#define K_CATCH(res, expr_call, on_error)
```
Evaluates the expression, stores the result in res, and if it fails, executes the on_error statement.

Arguments:
- res - Variable to store the KResult
- expr_call - Expression that returns KResult
- on_error - Statement to execute on error

## Functions
### k_result_from_errno()
```c
KResult k_result_from_errno(int err)
```
Converts a system errno value to a KResult. Maps common errno values to corresponding K_RESULT codes.

Arguments:
- err - errno value (typically from errno global variable)

Return values:
- Returns a KResult with the appropriate code and sys_errno set

Errno mappings:
- 0 → K_OK
- ENOENT → K_ERR_FILE_NOT_FOUND
- EACCES, EPERM → K_ERR_PERMISSION
- EISDIR → K_ERR_IS_DIRECTORY
- ENOSPC → K_ERR_NO_SPACE
- ENOMEM → K_ERR_NO_MEMORY
- EINVAL → K_ERR_INVALID_ARG
- EIO → K_ERR_IO
- Others → K_ERR_UNKNOWN

### k_result_str()
```c
const char* k_result_str(KResult result)
```
Returns a human-readable string describing the error code.

Arguments:
- result - The KResult to convert to a string

Return values:
- Returns a string constant describing the error (e.g., "OK", "Out of memory", "Permission denied")

### k_result_print()
```c
void k_result_print(KResult r)
```
Prints the error information to stdout. If sys_errno is set, it also prints the errno value and the system error description.

Arguments:
- r - The KResult to print

Output format:
- With errno: `<error message> (errno=<num>: <strerror>)`
- Without errno: `<error message>`

## Notes
KResult is designed to be returned by value from functions. The K_SUCCEEDED and K_FAILED macros provide a convenient way to check for success or failure.

The sys_errno field is optional. If set to 0, it means no system error is associated with the result. Non-zero values indicate that a system error occurred and can be used for more detailed error reporting.

When using k_result_from_errno(), it's recommended to capture errno immediately after a failed system call, as errno can be modified by subsequent operations.

The K_WRAP family of macros simplifies error handling by reducing boilerplate code. They are particularly useful in functions that need to perform multiple operations that can fail, allowing for clean error propagation and resource cleanup using labels and goto statements.
