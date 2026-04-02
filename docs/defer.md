# kdefer
kdefer provides a cleanup mechanism for the C language.
It allows you to pre-register operations you want to execute when a function exits (such as freeing memory, closing files, or releasing locks) and execute them all at once in reverse order of registration at any desired timing.

## Types
- KDefer
The core of the defer queue. It should be allocated on the stack or heap and initialized with k_defer_init() before use.

### KDeferFunc
```c
typedef void (*KDeferFunc) (void *arg);
```
The type for the callback function to be registered. It takes a single void* argument.

- KDeferEntry
```c
typedef struct {
	KDeferFunc func;
	void* arg;
} KDeferEntry;
```
A pair of a function pointer and an argument. Do not manipulate this directly.

## Functions
### k_defer_init()
```c
KResult k_defer_init(KDefer *d)
```
Initializes KDefer. It allocates the internal buffer on the heap.

Arguments:
- d - Pointer to the object to be initialized (NULL is not allowed)

Return values:
- Success: K_OK
- If d is NULL: K_ERR_INVALD_ARG
- Memory allocation failure: K_ERR_NO_MEMORY

> Note: Always call k_defer_free() or k_defer_exec_free() after use.

### k_defer_append
```c
KResult k_defer_append(KDefer *d, KDeferFunc func, void *arg)
```
Adds a cleanup operation to be executed to the queue. If the queue is full, the internal buffer is automatically doubled.

Arguments:
- d - Target queue
- func - Function to execute
- arg - Argument to pass to func (can be null)

Return values:
- Success: K_OK
- Invalid argument: K_ERR_INVALID_ARG
- Out of memory: K_ERR_NO_MEMORY

### k_defer_exec
```c
void k_defer_exec(KDefer* d)
```
Executes all registered operations in LIFO (Last-In, First-Out) order. After execution, the entry count is reset to 0 (the buffer is retained).

### k_defer_exec_free
```c
void k_defer_exec_free(KDefer *d);
```
Calls k_defer_exec() followed by k_defer_free(). Use this for normal exit paths.

## Notes
k_defer_exec ignores errors. Even if a callback function fails, execution continues.
If you need to handle errors during cleanup, pass the state through a wrapper function.

Be mindful of the lifetime of arg. k_defer_append only copies the pointer. The object pointed to by arg must remain valid until k_defer_exec is called.
Using k_defer_free by itself is discouraged. It frees only the buffer without executing registered cleanup operations, which can lead to resource leaks. Unless there is a specific reason to skip processing in an error path, please use k_defer_exec_free.