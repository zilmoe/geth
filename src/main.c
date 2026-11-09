// this is a contract checker program for simple c files
// [https://gatherer.wizards.com/ONE/en-us/95/geth-thane-of-contracts]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "stack.h"

// Defines
#define MAX_FILE_LENGTH 4096 // TODO this should check the file size

// Structs
typedef struct file_s {
  char *file_name;
  int file_len;
  char *buf;
  // stack and other operating stuff here
} File_s;

// Helper functions

// counts bytes until the next specified char
int strncnt(const char *str, char delim)
//@requires str != NULL
{
  int i = 0;
  while (str[i] != delim) {
    i += 1;
  }
  return i;
}

// Program Logic

// creates a new File struct for parsing
// and operating
File_s *create_file_s(char *file_name)
//@requires file_name != NULL
//@ensures file_s != NULL
{
  File_s *file_s = malloc(sizeof(File_s));
  file_s->file_name = file_name;

  // read the file
  file_s->buf = malloc(MAX_FILE_LENGTH);
  int fd = open(file_s->file_name, O_RDONLY);
  if (fd < 0) {
    printf("Please supply a valid file.\n");
    exit(0);
  }
  int len = read(fd, file_s->buf, MAX_FILE_LENGTH);
  close(fd);

  file_s->file_len = len;
  file_s->buf[MAX_FILE_LENGTH-1] = '\x00'; // ensure it is null byte terminated
  
  return file_s;
}

// parses for contracts
// and turns them into ASSERT
// statements
void preprocess_contracts(File_s *file_s)
//@requires file_s != NULL
{
  // Overview of plan
  // 1. read file into buffer
  // 2. find "//" or "{" occurance
  // 3. (//) parse the statement
  // 4. store it
  // 5. ({)
  // 6. pop all stored items
  // 7. continue from step 2 until EOF
  // 8. we can add some function checking later...

  // init structures
  Stack *req_stack = init();
  Stack *ens_stack = init();
  char *new_file = malloc(MAX_FILE_LENGTH * 2);
  int new_file_i = 0;

  // Find "//@" or "{"
  int requires_cnt = 0;
  int ensures_cnt = 0;

  // start by including a header
  int header_size = strlen("#include <assert.h>\n");
  strcpy(new_file, "#include <assert.h>\n");
  new_file_i += header_size;
  
  for (size_t i = 0; i < file_s->file_len; i++) {    
    if (strncmp(&file_s->buf[i], "//@", strlen("//@")) == 0) {
      if (strncmp(&file_s->buf[i], "//@requires", strlen("//@requires")) == 0) {
	requires_cnt++;

	// want to start at the statement of the requires
	i += 11; // TODO this should also be fixed to not be a constant but find the next space
	
	// allocate to store the new string
	int str_len = strncnt(&file_s->buf[i], '\n') + 1; // for null terminator
	char *itm = malloc(str_len); // TODO change so that geth doesn't require file to be properly formatted
	strncpy(itm, &file_s->buf[i], str_len);
	itm[str_len - 1] = '\0';
	
	// store what comes after the requires
	push(req_stack, itm);

	// increment i so we don't keep the comment
	i += str_len - 1;
      } else if (strncmp(&file_s->buf[i], "//@ensures", strlen("//@ensures")) == 0) {
	ensures_cnt++;

	i += 10; // is this right?

	int str_len = strncnt(&file_s->buf[i], '\n') + 1; // for null terminator
	char *itm = malloc(str_len); // @118
	strncpy(itm, &file_s->buf[i], str_len);
	itm[str_len - 1] = '\0';

	// store what comes after the ensures
	push(ens_stack, itm);

	// increment i so we don't keep the comment
	i += str_len - 1;
      }
    } else if (file_s->buf[i] == '{') {
      new_file[new_file_i] = file_s->buf[i]; // add byte to the new file
      new_file_i += 1;
      
      // check if we have assert statements to dump
      while (!s_empty(req_stack)) {
	// add newline
	new_file[new_file_i] = '\n';
	new_file_i += 1; // maybe it would be nice to make this a .h?

	// add assert!
	strncpy(new_file+new_file_i, "assert(", strlen("assert("));
	new_file_i += strlen("assert(");

	// statement
	char *itm = pop(req_stack);
	strncpy(new_file+new_file_i, itm, strlen(itm));
	new_file_i += strlen(itm);

	strncpy(new_file+new_file_i, ");", 2);
	new_file_i += 2;
	free(itm);
      }
    } else if (file_s->buf[i] == '}') {
      while (!s_empty(ens_stack)) {
	// add the assert statement
	strncpy(new_file+new_file_i, "assert(", strlen("assert("));
	new_file_i += strlen("assert(");

	// statement
	char *itm = pop(ens_stack);
	strncpy(new_file+new_file_i, itm, strlen(itm));
	new_file_i += strlen(itm);

	strncpy(new_file+new_file_i, ");", 2);
	new_file_i += 2;

	new_file[new_file_i] = '\n';
	new_file_i += 1; // maybe it would be nice to make this a .h?

	free(itm);
      }
      
      // add the byte at the end of the file!
      new_file[new_file_i] = file_s->buf[i];
      new_file_i += 1; // TODO this should def be a abstraction
    } else {
      new_file[new_file_i] = file_s->buf[i]; // add byte to the new file
      new_file_i += 1;
    }
  }
  printf("Found %d requires contracts!\n", requires_cnt);
  printf("Found %d ensures contracts!\n", ensures_cnt);
  int fd = open("./out-file.c", O_RDWR | O_CREAT); // make file name a magic var
  if (fd < 0) {
    puts("Error writing contracts\n");
  } else {
    write(fd, new_file, new_file_i);
    chmod("./out-file.c", S_IRWXU);
    close(fd);
  }
  // change the file perms!
  free_stack(req_stack);
  free_stack(ens_stack);
  free(new_file);
}

// deallocates a file struct
void destroy_file(File_s *file_s)
{
  free(file_s->buf);
  free(file_s);
}

int main(int argc, char *argv[]) {
  // add arg to specify outfile
  if (argc == 1) {
    printf("Please supply a file for geth.\n");
    exit(0);
  }

  File_s *file_s = create_file_s(argv[1]);
  preprocess_contracts(file_s);

  destroy_file(file_s);
  return 0;
}
