#ifndef MY_Q_H
#define MY_Q_H

struct process {
    char* name;
    int pid;
    int ttc;
    int toa;
};

struct node {
	struct process *p;
	struct node *next;
};


// enqueue'd at tail
void enqueue (struct node ** headaddr, const char * name, int pid, int ttc, int toa);
// dequeue'd from head
struct process* dequeue (struct  node **headaddr);

enum sort_type{ARRIVAL, DURATION};
void sort_by_arrival(struct node **head_ref, int queue_size);
void sort_by_duration(struct node **head_ref, int queue_size);

void print (struct node * head);
#endif
