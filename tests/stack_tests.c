// runs tests on the stack implementation
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include "../src/stack.h"

void simple_popping()
{
  Stack *stack = init();
  push(stack, "1");
  push(stack, "2");
  push(stack, "3");
  push(stack, "4");

  char *curr = pop(stack);
  assert(curr != NULL);
  assert(strcmp(curr, "4") == 0);
  curr = pop(stack);
  assert(curr != NULL);
  assert(strcmp(curr, "3") == 0);
  curr = pop(stack);
  assert(curr != NULL);
  assert(strcmp(curr, "2") == 0);
  curr = pop(stack);
  assert(curr != NULL);
  assert(strcmp(curr, "1") == 0);
  curr = pop(stack);
  assert(curr == NULL);

  free_stack(stack);
}

void alternate_push_pop()
{
  char *curr;
  Stack *stack = init();

  push(stack, "1");
  curr = pop(stack);
  assert(strcmp(curr, "1") == 0);

  push(stack, "1");
  push(stack, "2");
  curr = pop(stack);
  assert(strcmp(curr, "2") == 0);
  curr = pop(stack);
  assert(strcmp(curr, "1") == 0);

  push(stack, "1");
  push(stack, "2");
  push(stack, "3");
  curr = pop(stack);
  assert(curr != NULL);
  assert(strcmp(curr, "3") == 0);
  curr = pop(stack);
  assert(curr != NULL);
  assert(strcmp(curr, "2") == 0);
  curr = pop(stack);
  assert(curr != NULL);
  assert(strcmp(curr, "1") == 0);

  free_stack(stack);
}

void pop_nothing() {
  Stack *stack = init();
  char *curr = pop(stack);
  assert(curr == NULL);
  free_stack(stack);
}

int main() {
  simple_popping();
  alternate_push_pop();
  printf("All tests passed;");
}
