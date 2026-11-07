#include <stdbool.h>

struct stack_head;
typedef struct stack_head Stack;

// initializes the stack
// and returns a pointer
// to the stack
Stack *init();

// pushes a item on the
// stack
void push(Stack *stack, char *val);

// pops an item from the stack
// returns NULL otherwise
char *pop(Stack *stack);

// returns the top item of the stack
// without popping it
char *peek(Stack *stack);

// returns true if the stack is empty, false otherwise
bool s_empty(Stack *stack);

// frees the stack
void free_stack(Stack *stack);
