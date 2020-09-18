#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "my_q.h"
#include "sched.h"


void print_ready_q(struct node * head) {
    if (head == NULL) {
        fprintf(stdout, "empty:");
        return;
    }
    while(head != NULL) {
        fprintf(stdout, "%s(%d),", head->p->name, head->p->ttc);
        head = head->next;
    }
    fprintf(stdout, ":");
}

void free_process(struct process * p) {
    free(p->name);
    free(p);
}

void run_fifo(struct node * head, bool is_sjf) {
    struct process * current = NULL;
    struct node * ready_q_head = NULL;
    int ready_q_size = 0;
    int time = 1;
    while (head != NULL || current != NULL || ready_q_head != NULL) {
        struct node * tmp = head;
        while (tmp != NULL) {
            if (tmp->p->toa == time - 1) {
                if (current == NULL) {
                    current = dequeue(&tmp); 
                }
                else {
                    struct process * p = dequeue(&tmp);
                    // we may want to enqueue only if ttc > 0
                    enqueue(&ready_q_head, p->name, p->pid, p->ttc, p->toa);
                    ready_q_size++;
                    if (is_sjf) {
                        sort_by_duration(&ready_q_head, ready_q_size);
                    }
                    free_process(p);
                }
                // Dequeue operation deletes current node at `tmp` 
                // and update `tmp` with `tmp->next`, 
                // this means the node `head` becomes dangling
                // so we need to manually update `head` 
                head = tmp;
            } 
            else {
                tmp = tmp->next;
            }
        }
        if (current != NULL) {
            if (current->ttc <= 0) {
                free_process(current);
                if (ready_q_head == NULL) {
                    break;
                }
                current = dequeue(&ready_q_head);
                ready_q_size--;
            }
            if (current != NULL) {
                if (current->ttc == 0)
                    fprintf(stdout, "%d:idle:", time);
                else 
                    fprintf(stdout, "%d:%s:", time, current->name);
                
                print_ready_q(ready_q_head);
                fprintf(stdout, "\n");
                current->ttc--;
            } else {
                fprintf(stdout, "%d:idle:empty:\n", time);    
            }
        } else {
            fprintf(stdout, "%d:idle:empty:\n", time);
        }
        time++;
    }

}

void run_sjf(struct node * head) {
    // sjf is really just fifo except we sort the ready queue
    // by duration
    run_fifo(head, true);
}

void run_stcf(struct node * head) {
    struct process * current = NULL;
    struct node * ready_q_head = NULL;
    int ready_q_size = 0;
    int time = 1;
    while (head != NULL || current != NULL || ready_q_head != NULL) {
        struct node * tmp = head;
        while (tmp != NULL) {
            if (tmp->p->toa == time - 1) {
                if (current == NULL) {
                    current = dequeue(&tmp); 
                }
                else {
                    struct process * p = dequeue(&tmp);
                    // we may want to enqueue only if ttc > 0
                    enqueue(&ready_q_head, p->name, p->pid, p->ttc, p->toa);
                    ready_q_size++;
                    sort_by_duration(&ready_q_head, ready_q_size);
                    free_process(p);
                }
                // Dequeue operation deletes current node at `tmp` 
                // and update `tmp` with `tmp->next`, 
                // this means the node `head` becomes dangling
                // so we need to manually update `head` 
                head = tmp;
            } 
            else {
                tmp = tmp->next;
            }
        }
        if (current != NULL) {
            if (ready_q_head != NULL && ready_q_head->p->ttc < current->ttc) {
                enqueue(&ready_q_head, current->name, current->pid, current->ttc, current->toa);
                free_process(current);
                current = dequeue(&ready_q_head);
                sort_by_duration(&ready_q_head, ready_q_size);
            } 
            if (current->ttc <= 0) {
                free_process(current);
                if (ready_q_head == NULL) {
                    break;
                }
                current = dequeue(&ready_q_head);
                ready_q_size--;
            }
            if (current != NULL) {
                if (current->ttc == 0)
                    fprintf(stdout, "%d:idle:", time);
                else 
                    fprintf(stdout, "%d:%s:", time, current->name);
                
                print_ready_q(ready_q_head);
                fprintf(stdout, "\n");
                current->ttc--;
            } else {
                fprintf(stdout, "%d:idle:empty:\n", time);    
            }
        } else {
            fprintf(stdout, "%d:idle:empty:\n", time);
        }
        time++;
    }
}

void run_rr(struct node * head) {
    struct process * current = NULL;
    struct node * ready_q_head = NULL;
    int time = 1;
    while (head != NULL || current != NULL || ready_q_head != NULL) {
        struct node * tmp = head;
        while (tmp != NULL) {
            if (tmp->p->toa == time - 1) {
                if (current == NULL) {
                    current = dequeue(&tmp); 
                }
                else {
                    struct process * p = dequeue(&tmp);
                    // we may want to enqueue only if ttc > 0
                    enqueue(&ready_q_head, p->name, p->pid, p->ttc, p->toa);
                    free_process(p);
                }
                // Dequeue operation deletes current node at `tmp` 
                // and update `tmp` with `tmp->next`, 
                // this means the node `head` becomes dangling
                // so we need to manually update `head` 
                head = tmp;
            } 
            else {
                tmp = tmp->next;
            }
        }
        if (current != NULL) {
            if (current->ttc <= 0) {
                free_process(current);
                if (ready_q_head == NULL) {
                    break;
                }
                current = dequeue(&ready_q_head);
            }
            if (current != NULL) {
                if (current->ttc == 0)
                    fprintf(stdout, "%d:idle:", time);
                else 
                    fprintf(stdout, "%d:%s:", time, current->name);
                
                print_ready_q(ready_q_head);
                fprintf(stdout, "\n");
                current->ttc--;
                if (current->ttc > 0)
                    enqueue(&ready_q_head, current->name, current->pid, current->ttc, current->toa);
                free_process(current);
                current = dequeue(&ready_q_head);
            } else {
                fprintf(stdout, "%d:idle:empty:\n", time);    
            }
        } else {
            fprintf(stdout, "%d:idle:empty:\n", time);
        }
        time++;
    }
}


void run_scheduler(struct node * head, char * policy) {
    if (strcmp(policy, "FIFO") == 0) {
        run_fifo(head, false);
    }
    else if (strcmp(policy, "SJF") == 0) {
        run_sjf(head);
    }
    else if (strcmp(policy, "STCF") == 0) {
        run_stcf(head);
    }
    else if (strcmp(policy, "RR") == 0) {
        run_rr(head);
    }
}