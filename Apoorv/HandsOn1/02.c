/* 
MT2020013 Apoorv Panse
Write a simple program to execute in an infinite loop at the background. Go to /proc directory and
identify all the process related information in the corresponding proc directory.
*/

int main(){
    while(1){}
    return 0;
}

/*
PID of the process is 31560
The process related information in /proc/31560 :
Name:	a.out
Umask:	0002
State:	R (running)
Tgid:	31560
Ngid:	0
Pid:	31560
PPid:	31553
TracerPid:	0
Uid:	1000	1000	1000	1000
Gid:	1000	1000	1000	1000
FDSize:	256
Groups:	4 24 27 30 46 121 132 133 1000 
NStgid:	31560
NSpid:	31560
NSpgid:	31553
NSsid:	31359
VmPeak:	    2448 kB
VmSize:	    2376 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	     768 kB
VmRSS:	     768 kB
RssAnon:	      88 kB
RssFile:	     680 kB
RssShmem:	       0 kB
VmData:	      76 kB
VmStk:	     132 kB
VmExe:	       4 kB
VmLib:	    1616 kB
VmPTE:	      40 kB
VmSwap:	       0 kB
HugetlbPages:	       0 kB
CoreDumping:	0
THP_enabled:	1
Threads:	1
SigQ:	0/63291
SigPnd:	0000000000000000
ShdPnd:	0000000000000000
SigBlk:	0000000000000000
SigIgn:	0000000000000006
SigCgt:	0000000000000000
CapInh:	0000000000000000
CapPrm:	0000000000000000
CapEff:	0000000000000000
CapBnd:	000001ffffffffff
CapAmb:	0000000000000000
NoNewPrivs:	0
Seccomp:	0
Seccomp_filters:	0
Speculation_Store_Bypass:	thread vulnerable
SpeculationIndirectBranch:	conditional enabled
Cpus_allowed:	ff
Cpus_allowed_list:	0-7
Mems_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000001
Mems_allowed_list:	0
voluntary_ctxt_switches:	0
nonvoluntary_ctxt_switches:	0

*/