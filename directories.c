#include <dirent.h>
#include <errno.h>
#include <unistd.h>

DIR *open_dir(char *path) {
	DIR *dir;
	dir = opendir(path);

	if (dir == NULL) {
		perror("path not found");
	}

	return dir;
}
