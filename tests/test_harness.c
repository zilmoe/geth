// simple testing harness for geth
#include <stdio.h>
#include <spawn.h>

typedef struct geth_output {
  int contracts; // number of contracts found
  bool compiled; // wether the geth output file compiled
  int err_code; // error code in case geth encounters an error
} Geth_output;

// run geth on the given file and
// returns a Geth_output struct with the
// results of the geth run. The result
// will be NULL in the case that geth cannot
// be run, otherwise, the struct will
// contain the error code from geth
// you will be required to free the pointer
// given back from run_geth
Geth_output *run_geth(const char *file)
//@requires file != NULL
{
  Geth_output *geth_info = malloc(sizeof(Geth_output));
  const char *geth_path = "../bin/geth";
  const char *geth_argv[2];
  geth_argv[0] = geth_path;
  geth_argv[1] = NULL; // I doubt this...
  const char *geth_environ[1];
  geth_environ[0] = NULL;
  int res = posix_spawnp(NULL, geth_path,
			 NULL, NULL,
			 geth_argv, geth_environ);
  if (res < 0) {
    return NULL;
  }
  
  return geth_info;
}

int main(int argc, char *argv[]) {
  // for now, assume the binary
  // is already built
  if (res != 0) {
    printf("Failed to run binary!");
    return 1;
  }
  return 0;
}
