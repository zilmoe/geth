# Geth

## Intro
Geth is a simple C preprocessor for C contracts. It is meant to be a lightweight
binary that can be run on single files to quickly turn comments into assertion
statements. It is based on the contracts seen in the
[c0 intro programming language](https://www.cs.cmu.edu/~15122/).

## Example
Your code before running geth:
```c
int divide(int a, int b)
//@requires b != 0
{
  return a / b;
}
```

Your code after (hopefully):
```c
#include <assert.h>

int divide(int a, int b)
{
  assert(b != 0);
  return a / b;
}
```

## Running
Geth will add `#include <assert.h>`.
Run geth with
```sh
geth ./main.c
```
geth will output a file called "out-file.c"
which contains all of your changes.

You can now compile the file with
```sh
gcc out-file.c
```

## Building
It is very simple :)
```sh
git clone git@github.com:zilmoe/geth.git
cd geth
mkdir bin
cd src
make build
```

## Small roadmap :)
- [x] `\\@requires` (preconditions)
- [x] `\\@ensures` (postconditions)
- [ ] `\\@loop_invariant`

## Why comments?
I know that there are those who believe that code should not be encoded in comments. I agree.
However, I really enjoy this style of commenting and I like the look more as compared to a bunch of
assert statements in my source code. Also, I formed a habit of writing
code like this after taking CMU's 15-122.

## License
MIT

