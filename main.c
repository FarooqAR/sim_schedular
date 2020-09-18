#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <ctype.h>
#include "my_q.h"
#include "sched.h"

int get_file_size(FILE* fp) {
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

void validate_char(char c, const char * field) {
    if (!isdigit(c)) {
        fprintf(stdout, "Invalid value of %s\n", field);
        exit(1);
    }
}

struct node* read_processes(FILE* fp, int * queue_size) {
    struct node *head = NULL;
    int filesz = get_file_size(fp); 
    char c = getc(fp);
    while (c != EOF) {
        bool iscomment = false;
        if (c == '#') iscomment = true;

        while (c != '\n') {
            if (!iscomment) {
                char name[10];

                // filesz is a safe size to pick (it may even be too much for a single info)
                char pid[filesz];
                char ttc[filesz];
                char toa[filesz];
                
                int i = 0;
                while (c != ':' && i < 10) {
                    name[i] = c;
                    c = getc(fp);
                    i++;
                }
                name[i] = '\0';
                c = getc(fp);

                i = 0;
                while (c != ':' && i < filesz) {
                    validate_char(c, "pid");
                    pid[i] = c;
                    c = getc(fp);
                    i++;
                }
                pid[i] = '\0';
                c = getc(fp);
                i = 0;
                while (c != ':' && i < filesz) {
                    validate_char(c, "duration");
                    ttc[i] = c;
                    c = getc(fp);
                    i++;
                }
                ttc[i] = '\0';

                c = getc(fp);
                i = 0;
                while (c != '\n' && i < filesz) {
                    validate_char(c, "arrivaltime");
                    toa[i] = c;
                    c = getc(fp);
                    i++;
                }
                ttc[i] = '\0';
                
                enqueue(&head, name, atoi(pid), atoi(ttc), atoi(toa));           
                (*queue_size)++;
            }
            if (iscomment)
                c = getc(fp);
        }
        
        c = getc(fp);
    }
    return head;
} 

int main (int argc, char * argv[]){
    char * filename = argv[1];
    char * policy = argv[2];
    if (argc != 3 || (
        strcmp(policy, "FIFO") &&
        strcmp(policy, "SJF") &&
        strcmp(policy, "STCF") &&
        strcmp(policy, "RR")
    )) {
        fprintf(stderr, "Error. Usage: ./mysched filename POLICY\nwhere POLICY can be one of the following strings:\nFIFO\nSFJ\nSTCF\nRR\n");
        return 1;
    }
    
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error. Unable to open file %s\n", filename);
        return 1;
    }
    int queue_size = 0;
    struct node *head = read_processes(fp, &queue_size);
    sort_by_arrival(&head, queue_size);
    run_scheduler(head, policy);
    fclose(fp);
    return 0;
}

