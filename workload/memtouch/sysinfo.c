#include <sys/sysinfo.h>
#include <stdio.h>

void main() {
	// bytes (default)
	int unit = 1;
	// Kbytes
	unit = 1024;
	// Mbytes
	unit = 1024*1024;

	struct sysinfo si;

	if(sysinfo(&si) < 0) {
		printf("error! couldn't get sin");
	}

	printf("total memory    : %10li\n", si.totalram/unit);
	printf("free memory     : %10li\n", si.freeram/unit);
	printf("shared memory   : %10li\n", si.sharedram/unit);
	printf("buffer memory   : %10li\n", si.bufferram/unit);
}
