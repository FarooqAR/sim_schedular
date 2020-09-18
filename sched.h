#ifndef SCHED
#define SCHED

#include "my_q.h"

void print_ready_q(struct node * head);
void run_fifo(struct node * head, bool is_sjf);
void run_sjf(struct node * head);
void run_stcf(struct node * head);
void run_rr(struct node * head);
void run_scheduler(struct node * head, char * policy);

#endif