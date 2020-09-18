#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_q.h"

// enqueue'd at tail
void enqueue (struct node ** headaddr, const char * name, int pid, int ttc, int toa) {
	if (headaddr==NULL){
		fprintf(stderr, "NULL ptr passed\n"); exit(1);
	}

	struct node * n = malloc(sizeof(struct node));
	if (n==NULL){
		fprintf(stderr, "memory allocation failed\n"); exit(1);
	}
    struct process * p = malloc(sizeof(struct process));
    p->name = (char *) malloc(strlen(name) + 1);
    strcpy(p->name, name);
    p->pid = pid;
    p->ttc = ttc;
    p->toa = toa;
    n->p = p;
	n->next = NULL;


	if( *headaddr==NULL){ // empty list
		*headaddr = n;
	}
	else {
		// get to tail
		struct node *tmp = *headaddr;
		while (tmp->next != NULL)
			tmp = tmp->next;

		tmp->next = n;
	}
}
		
	
// dequeue'd from head
struct process* dequeue (struct  node **headaddr) {
	
	if (headaddr==NULL){
		return NULL;
	}

	if (*headaddr==NULL) { // list is empty
		return NULL;
	}
	else {
		struct node *n = *headaddr;
		*headaddr = (*headaddr)->next;
		struct process* p = n->p;
		free(n);
		return p;
	}
}


int compare_toa(const void * p1, const void * p2) {
	return ((struct process *)p1)->toa - ((struct process *)p2)->toa;
}

int compare_ttc(const void * p1, const void * p2) {
	return ((struct process *)p1)->ttc - ((struct process *)p2)->ttc;
}

void sort(struct node **head_ref, int queue_size, enum sort_type stype) {
	struct process * new_queue = malloc(queue_size * sizeof(struct process));
	struct node * tmp = *head_ref;
	for(int i = 0; i < queue_size; i++) {
		new_queue[i] = *dequeue(&tmp);
	}
	if (stype == ARRIVAL) {
		qsort(new_queue, queue_size, sizeof(struct process), compare_toa);
	}
	else {
		qsort(new_queue, queue_size, sizeof(struct process), compare_ttc);
	}
	struct node * sorted = NULL;
	for(int i = 0; i < queue_size; i++) {
		struct process p = new_queue[i];
		enqueue(&sorted, p.name, p.pid, p.ttc, p.toa);
		// when enqueuing, name is copied to heap so we have to delete the original p.name
		free(p.name);
	}
    *head_ref = sorted;
	free(new_queue);
}
void sort_by_arrival(struct node **head_ref, int queue_size)
{ 
	sort(head_ref, queue_size, ARRIVAL);
} 


void sort_by_duration(struct node **head_ref, int queue_size)
{
	sort(head_ref, queue_size, DURATION);
}


void print (struct node * head) {
	if (head==NULL)
		fprintf(stdout, "empty queue\n");

	else {
		while (head!=NULL){
			fprintf(stdout, "name: %s\n", head->p->name);
            fprintf(stdout, "pid: %d\n", head->p->pid);
            fprintf(stdout, "ttc: %d\n", head->p->ttc);
            fprintf(stdout, "toa: %d\n", head->p->toa);
			head = head->next;
            fprintf(stdout, "\n");
		}
	}
}
