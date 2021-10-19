#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "recdir.h"

// TODO: compute hashes of the files
// TODO: build the hash table
int main(int argc, char **argv){

	(void) argc;
	(void) argv;

	RECDIR *recdir = recdir_open(".");

	errno = 0;
	struct dirent *ent = recdir_read(recdir);
	while(ent){
		printf("recdir file: %s/%s\n", recdir_top(recdir)->path, ent->d_name);
		ent = recdir_read(recdir);
	}

	if(errno != 0){
		fprintf(stderr, "ERROR: could not read the dir: %s\n",  recdir_top(recdir)->path);
		exit(1);
	}

	recdir_close(recdir);

	return 0;
}