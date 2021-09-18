/* 
MT2020013 Apoorv Panse
Write a program to print the following information about a given file.
a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification
j. time of last change
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <stdlib.h>
#include <time.h> 
#include <errno.h>
#include <string.h>

int main() {
    
    struct stat stats;
    if (stat("09.c", &stats) == -1) {
        
        printf("%s\n",strerror(errno));
    
    }

	printf("Inode number: %ld\n", (long)stats.st_ino);
	printf("Number of hard links: %ld\n", (long)stats.st_nlink);
	printf("User ID: %ld\n", (long)stats.st_uid);
	printf("Group ID: %ld\n", (long)stats.st_gid);
	printf("Total size: %lld bytes\n", (long long)stats.st_size);
	printf("Block size: %ld\n", (long)stats.st_blksize);
	printf("Number of blocks allocated: %lld\n", (long long)stats.st_blocks);
	printf("Time of last access: %s", ctime(&stats.st_atime));
	printf("Time of last modification:  %s", ctime(&stats.st_mtime));
	printf("Time of last change: %s", ctime(&stats.st_ctime));
	return 0;
}