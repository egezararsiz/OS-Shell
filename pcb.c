#include "pcb.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//In this implementation, Pid is the same as file ID 
PCB* makePCB(char* pid,FILE* fp){
    PCB * newPCB = malloc(sizeof(PCB));
    newPCB->file = fp;
    newPCB->pid = pid;
    newPCB->PC = 0;
    newPCB -> offset = 0;
    newPCB -> curr_page = 0;
    newPCB->job_length_score = 1;
    newPCB->total_pages = 0;
    newPCB->last_page_offset=0;
    for(int i = 0; i < 10; i++) newPCB -> pages[i] = -1;
    return newPCB;
}