#include <stdio.h>
#ifndef PCB_H
#define PCB_H

/*
 * Struct:  PCB 
 * --------------------
 * pid: process(task) id
 * PC: program counter, stores the line that the task is executing
 * start: the first line in shell memory that belongs to this task
 * end: the last line in shell memory that belongs to this task
 * job_length_score: for EXEC AGING use only, stores the job length score
 */
typedef struct
{
    FILE* file;
    char* pid;
    int PC;
    int offset;
    int curr_page;
    int total_pages;
    int last_page_offset;
    int job_length_score;
    int pages[10];
}PCB;

PCB * makePCB(char* pid, FILE* fp);
#endif
