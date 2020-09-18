# Simulate Process Scheduler

Simulate following scheduling policies on dummy processes:
- *FIFO* (First In First Out): The oldest process (smallest `toa`) completes first.
- *SJF* (Shortest Job First): If there are multiple processes with the same `toa`, the shortest process (smallest `ttc`) completes first. Other processes are put in ready queue (always sorted by `ttc`).
- *STCF* (Shortest Time to Completion First): If a process is running and a process with a shorter `ttc` comes in future, the current process is stopped and put in ready queue while the incoming shorter process starts running. Also called preemptive scheduling. The ready queue is always in a sorted (by `ttc`) state.
- *RR* (Round Robin): Each process is run for a time slice (assumed to be a single cpu cycle) while other processes wait in the ready queue. The currently running process is put at the end of the ready queue after it completes its time slice. Similar to context switching.

## Making the program
```shell
make main
```
(optional) with debug info (symbols etc):
```shell
make debug
```

## Running the program
You will need a file containing processes' information. A sample file `processes.dat` has been already created. Following command runs the FIFO policy on processes given in the sample file:

```shell
./main processes.dat FIFO
```
*Any line in the sample processes.dat is considered a comment and is ignored.*

The output is the state of the CPU from start to end. Each line represents the CPU state at any given time (again, one unit of time is assumed to be a single cycle). Example output:
```
1:P1:empty:
2:P1:P2(1):
3:P2:empty:
```
Above output tells us that:
- At time 1, a process P1 arrives (whose `ttc` is 2 unit of time). There is no other process arriving at the same time so the ready queue is empty 
- At time 2, P1 completes its duration (or `ttc`) and is discarded. At the same time, another process P2 (`ttc`=1) comes and put in the ready queue.
- At time 3, P2 is dequeued from ready queue and starts running. It takes 1 unit of time to complete.

### Terminologies:
`ttc` is time to completion. Also called duration.

`toa` is time of arrival.

`P1` is the process name. You'll find process id (pid) in the sample data file. It has currently no use.

Written in C
