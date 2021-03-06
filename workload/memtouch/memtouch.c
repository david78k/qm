/* 
 * memtouch 2.0
 * 
 * Author: Tae Seung Kang 
 * Date: 6/15/2014
 *
 * usage: memtouch [size (mbytes)] [speed (mbytes/s)]
 *
 * size: the size of the buffer that memtouch allocates.
 * speed: the speed that memtouch makes pages dirty.
 *
 * It first fills the allocated buffer with random values, then
 * periodically makes pages dirty at a specified speed. You may not need
 * to use the function to fill the buffer with random values, if you use
 * the -no-dedupe option of Qemu.
 *
 * change log:
 * - get the memory information using sysinfo: memory used, total memory, free memory
 * 
 */
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#include <limits.h>
#include <sys/sysinfo.h>

#define DEBUG 0
//#define UNIT 1 //bytes (default)
//#define UNIT 1024 // kbytes
#define UNIT 1048576 // Mbytes 1024*1024

// return memory to be allocated 
//int getMemAlloc (char **argv) {
int getMemAlloc (int mem_size) {
       // total memory allocated in MB
        int mem_alloc = 400;
   //     int mem_alloc_bytes = 0;
  //      double *d;

        int mem_total = 0;
        int mem_free = 0;
        int mem_used = 0;

        struct sysinfo si;

        if(sysinfo(&si) < 0) {
                printf("error! couldn't get sin");
        }

        mem_total = si.totalram/UNIT;
        mem_free = si.freeram/UNIT;
        mem_used = mem_total - mem_free;
        printf ("mem total: %d\n", mem_total);
        printf ("mem free: %d\n", mem_free);
        printf ("mem used: %d\n", mem_used);

        //mem_alloc = atoi (argv[1]);
        mem_alloc = mem_size - mem_used;
    //    mem_alloc_bytes = mem_alloc * 1024 * 1024;

//        d = malloc (mem_alloc_bytes);
        printf ("mem allocated: %d\n", mem_alloc);

        if (mem_alloc <= 0) exit(1);

	return mem_alloc;
}

int main(int argc, char **argv)
{
	int fd, ret;
	struct stat st;
	char *buf = NULL;
	char *membuf = NULL;
	unsigned long size = 0;
	// unsigned long memsize = 1024 * 1024 * 1024 - 100 * 1024 * 1024; 
	unsigned long ncopy = 0;
	int i;

	if (argc != 3) {
		//printf ("usage: %s [size (mbytes)] [speed (mbytes/s)]\n", argv[0]);
		printf ("usage: memtouch [size (mbytes)] [speed (mbytes/s)]\n", argv[0]);
		return 1;
	}

	/* ./a.out [size (mbytes)] [speed (mbytes/s)] */
	//unsigned long memsize = atoi(argv[1]) * 1024 * 1024;
	unsigned long memsize = getMemAlloc(atoi(argv[1])) * 1024 * 1024;

	fd = open("random.dat", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 2;
	}

	ret = fstat(fd, &st);
	if (ret < 0) {
		perror("fstat");
		return 2;
	}

	size = st.st_size;

	buf = malloc(size);
	if (!buf) {
		perror("malloc");
		return 2;
	}
	
	ret = read(fd, buf, size);
	if (ret != size) {
		printf("partial read ?, %d / %lu\n", ret, size);
		return 2;
	}

	membuf = malloc(memsize);
	if (!membuf) {
		perror("malloc");
		return 2;
	}

	ncopy = memsize / size;

	printf("size %lu memsize %lu ncopy %lu\n", size, memsize, ncopy);
	printf("fill up memory with random data ...");

	for (i = 0; i < ncopy; i++) {
		memcpy(membuf + i * size, buf, size);
	}

	close(fd);
	free(buf);

	printf("done\n");

	/* touch memory */
	int npages;
	npages = memsize / 4096;

	int speed = 1024 * 1024 * atoi(argv[2]);
	double sleep_time = 0.01;
	double updated = speed * sleep_time;
	unsigned long updated_pages = updated / 4096;

	printf("sleep %f every %lu pages\n", sleep_time, updated_pages);
	
	unsigned long counter = 0;

	for (;;) {
		printf("sleeping ...\n");

		if (updated_pages == 0) {
			sleep(UINT_MAX);
			continue;
		}
			
		for (i = 0; i < npages; i++) {
			char *ch = membuf + i * 4096;
			char tmp = *ch;
			*ch = ~tmp;

			if (DEBUG)
				printf("%x %x\n", tmp, *ch); 

			if ((counter % updated_pages) == 0) {
				int usec = sleep_time * 1000 * 1000;
				usleep(usec);
			}

			counter += 1;
		}
	}
		
	return 0;
}

