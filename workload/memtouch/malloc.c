#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>

#define DEBUG 0

// bytes (default)
//int unit = 1;
// Kbytes
//int unit = 1024;
// Mbytes
int unit = 1024*1024;

int getFreeMemory() {
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

	int freemem = si.freeram/unit;

        printf("total memory    : %10li\n", si.totalram/unit);
        printf("free memory     : %10li\n", freemem);
        printf("shared memory   : %10li\n", si.sharedram/unit);
        printf("buffer memory   : %10li\n", si.bufferram/unit);

	return freemem;
}

main (int argc, char **argv) { 
	//printf ("double %d bytes\n", sizeof (double));

	int i = 0;
	// total memory allocated in MB
	int mem_alloc = 400; 
	int mem_alloc_bytes = 0; 
	// memory update rate in MB
	int mem_update = 4; 
	int sleep_time = 65534; 
	double *d;

	int mem_total = 0;
	int mem_free = 0;
	int mem_used = 0;

	if(argc < 2) {
		printf("usage: %s size\n", argv[0]);
		//perror("usage: %s size", argv[0]);
		exit (1);
	}

        struct sysinfo si;

        if(sysinfo(&si) < 0) {
                printf("error! couldn't get sin");
        }

	mem_total = si.totalram/unit;
	mem_free = si.freeram/unit;
	mem_used = mem_total - mem_free;
	printf ("mem total: %d\n", mem_total);
	printf ("mem free: %d\n", mem_free);
	printf ("mem used: %d\n", mem_used);

	mem_alloc = atoi (argv[1]);
	mem_alloc = mem_alloc - mem_used;
	mem_alloc_bytes = mem_alloc * 1024 * 1024; 

	d = malloc (mem_alloc_bytes);
	printf ("mem allocated: %d\n", mem_alloc);

	if (mem_alloc <= 0) exit(1);

	if(!DEBUG) 	
		for (i = 0; i < mem_alloc_bytes/sizeof(double); i ++) {
			d[i] = i;
		}

	while (1) {
		sleep (sleep_time);
	}
}

