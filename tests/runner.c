#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  DIR* FD;
  struct dirent* in_file;
  FD = opendir(".");
  if(NULL == FD) {
    fprintf(stderr, "Failed to open directory\n");   
  }

  // check if binary exist
  if(access("../src/bfc", F_OK) != 0) {
    fprintf(stderr, "Binary bfc does not exist\n");
    exit(-1);
  }
  int ctr = 0;
  while((in_file = readdir(FD))) {
    char curr_file[256];
    strncpy(curr_file, in_file->d_name, 256);
    if(strcmp(curr_file, __FILE__) == 0 || strcmp(curr_file, ".") == 0 || strcmp(curr_file, "..") == 0 || strcmp(curr_file, "a.out") == 0) {
      printf("This is excluded file: %s\n", curr_file);
      continue;
    }
    int rc;
    rc = fork();

    if(rc == 0) {
      printf("executing: %s\n", curr_file);
      char* command = "../src/bfc";
      char* argList[] = {
        "../src/bfc", curr_file, NULL
      };
      execvp(command, argList);
      printf("will not be exec\n");
    }
    else {
      wait(NULL);
      ++ctr;
    }
  }
  printf("Already executed %d file\n", ctr);


  return 0;
}

