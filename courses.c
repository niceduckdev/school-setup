#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "directories.c"

void list_courses() {
	DIR *dir = open_dir(PATH "/current-semester");
	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type != DT_DIR || entry->d_name[0] == '.') {
			continue;
		}
		
    	printf("%s\n", entry->d_name);
	}
	
	closedir(dir);
}

int course_exists(char* course) {
	DIR *dir = open_dir(PATH "/current-semester");
	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type != DT_DIR || entry->d_name[0] == '.') {
			continue;
		}
		
		if (!strcmp(entry->d_name, course)) {
			return 1;
		}
	}
	
	closedir(dir);
	return 0;
}

void main(int argc, char* argv[]) {
	// show all courses in current semester
	if (argc <= 1) {
		list_courses();
		return;
	}

	// check if the course exists
	if (!course_exists(argv[1])) {
		return;
	}

	// remove the symlink if it already exists
	if (access(PATH "/current-course", F_OK) == 0) {
		unlink(PATH "/current-course");
	}

	// create a new symlink to the new current course
	char path[256];
	snprintf(path, sizeof(path), "%s/%s", PATH "/current-semester/", argv[1]);
	symlink(path, PATH "/current-course");
}
