// simple stack implementation for char *
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "stack.h"

struct stack_head {
  struct stack *top;
};

typedef struct stack {
  char *val;
  struct stack *next;
} Stack_child;

// allocates mem for stack
Stack *init ()
//@ensures head != NULL
{
  Stack *head = malloc(sizeof(Stack));
  head->top = NULL;
  return head;
}

// returns true if the stack is empty, false otherwise
bool s_empty(Stack *stack)
//@requires stack != NULL
{
  return stack->top == NULL;
}

// pushes a item on the stack
void push(Stack *stack, char *val)
//@requires stack != NULL
{
  Stack_child *new_node = malloc(sizeof(Stack_child));
  new_node->val = val;
  new_node->next = stack->top;
  stack->top = new_node;
}

// pops an item from the stack
// returns NULL otherwise
char *pop(Stack *stack)
//@requires stack != NULL
{
  if (stack->top == NULL) {
    return NULL;
  }

  Stack_child *to_free = stack->top;
  char *res = stack->top->val;
  
  stack->top = stack->top->next;
  free(to_free);

  return res;
}

// returns the top item of the stack
// without popping it
char *peek(Stack *stack)
{
  return stack->top->val;
}

// frees the stack
void free_stack(Stack *stack)
{
  Stack_child *curr = stack->top;
  Stack_child *tmp;
  while (curr != NULL) {
    tmp = curr->next;
    free(curr);
    curr = tmp;
  }

  free(stack);
}
