/* 
MT2020013 Apoorv Panse
19. Create a FIFO file by
a. mknod command
b. mkfifo command
c. use strace command to find out, which command (mknod or mkfifo) is better.
c. mknod system call
d. mkfifo library function
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
    int status = mknod("test19d", S_IFIFO | 0777, 0);
    if(status < 0){
        printf("%s\n",strerror(errno));
    }
    else{
        printf("test19d created successfully using mknod system call...\n");
    }

    status = mkfifo("test19e", 0777);
    if(status < 0){
        printf("%s\n",strerror(errno));
    }
    else{
        printf("test19e created successfully using mkfifo system call...\n");
    }
        
    return 0;
}

/*
=================a. mknod command===============================
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ mknod test19a1 p

=================b. mkfifo command==============================
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ mkfifo test19b1

=================c. strace command==============================
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ strace -c mknod test19a2 p

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0         7           read
  0.00    0.000000           0        10           close
  0.00    0.000000           0        27           mmap
  0.00    0.000000           0         7           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         2           rt_sigaction
  0.00    0.000000           0         1           rt_sigprocmask
  0.00    0.000000           0         6           pread64
  0.00    0.000000           0         2         2 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         2         2 statfs
  0.00    0.000000           0         2         1 arch_prctl
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         8           openat
  0.00    0.000000           0         1           mknodat
  0.00    0.000000           0         8           newfstatat
  0.00    0.000000           0         1           set_robust_list
  0.00    0.000000           0         1           prlimit64
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000           0        91         5 total
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ strace -c mkfifo test19b2
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0         7           read
  0.00    0.000000           0        10           close
  0.00    0.000000           0        27           mmap
  0.00    0.000000           0         7           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         2           rt_sigaction
  0.00    0.000000           0         1           rt_sigprocmask
  0.00    0.000000           0         6           pread64
  0.00    0.000000           0         2         2 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         2         2 statfs
  0.00    0.000000           0         2         1 arch_prctl
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         8           openat
  0.00    0.000000           0         1           mknodat
  0.00    0.000000           0         8           newfstatat
  0.00    0.000000           0         1           set_robust_list
  0.00    0.000000           0         1           prlimit64
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000           0        91         5 total
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ strace -c mknod test19a2 p
mknod: test19a2: File exists
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0         9           read
  0.00    0.000000           0         4           write
  0.00    0.000000           0        11           close
  0.00    0.000000           0        27           mmap
  0.00    0.000000           0         7           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         2           rt_sigaction
  0.00    0.000000           0         1           rt_sigprocmask
  0.00    0.000000           0         6           pread64
  0.00    0.000000           0         2         2 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         2         2 statfs
  0.00    0.000000           0         2         1 arch_prctl
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0        13         4 openat
  0.00    0.000000           0         1         1 mknodat
  0.00    0.000000           0         9           newfstatat
  0.00    0.000000           0         1           set_robust_list
  0.00    0.000000           0         1           prlimit64
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000           0       104        10 total
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ strace -c mkfifo test19b2
mkfifo: cannot create fifo 'test19b2': File exists
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 21.63    0.000202          11        17         7 openat
 15.52    0.000145          20         7           mprotect
  8.99    0.000084           7        12           close
  8.89    0.000083           9         9           read
  8.24    0.000077          19         4           write
  6.42    0.000060           6        10           newfstatat
  4.93    0.000046          23         2         2 statfs
  4.82    0.000045          45         1           munmap
  4.50    0.000042           1        28           mmap
  3.21    0.000030          10         3           brk
  2.89    0.000027          13         2           rt_sigaction
  2.14    0.000020          10         2         2 access
  2.03    0.000019          19         1         1 mknodat
  1.50    0.000014          14         1           set_tid_address
  1.50    0.000014          14         1           prlimit64
  1.39    0.000013          13         1           rt_sigprocmask
  1.39    0.000013          13         1           set_robust_list
  0.00    0.000000           0         6           pread64
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         2         1 arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000934           8       111        13 total

----mknod is a bit faster----

=================d. and e. Using system calls==============================
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc 19.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
test19d created successfully using mknod system call...
test19e created successfully using mkfifo system call...
*/