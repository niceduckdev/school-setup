#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "directories.c"

void list_semesters() {
	DIR *dir = open_dir(PATH);
	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type != DT_DIR || entry->d_name[0] == '.') {
			continue;
		}
		
    	printf("%s\n", entry->d_name);
	}
	
	closedir(dir);
}

int semester_exists(char* semester) {
	DIR *dir = open_dir(PATH);
	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type != DT_DIR || entry->d_name[0] == '.') {
			continue;
		}
		
		if (!strcmp(entry->d_name, semester)) {
			return 1;
		}
	}
	
	closedir(dir);
	return 0;
}

void main(int argc, char* argv[]) {
	// show all semesters
	if (argc <= 1) {
		list_semesters();
		return;
	}

	// check if the semester exists
	if (!semester_exists(argv[1])) {
		return;
	}
	
	// remove the symlink if it already exists
	if (access(PATH "/current-semester", F_OK) == 0) {
		unlink(PATH "/current-semester");
	}

	// create a new symlink to the new current semester	
	char path[256];
	snprintf(path, sizeof(path), "%s/%s", PATH, argv[1]);
	symlink(path, PATH "/current-semester");
}
