#include "cpu.h"
#include "pcb.h"
#include "interpreter.h"
#include "shell.h"
#include "shellmemory.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Struct:  CPU 
 * --------------------
 * IP: serve as a pointer to shell memory. Ex. IP = 101 means CPU is executing the 101th line in shellmemory
 * IR: stores the line of code that CPU is executing
 * quanta: how many lines of code could the current task run until it finishes or being switched to another task
 */
struct CPU
{
    int IP;
    int offset;
    char IR[1000];
    int quanta;
};
struct CPU aCPU = {.quanta =0};

int cpu_get_ip(){
    return aCPU.IP;
}

void cpu_set_ip(int pIP){
    aCPU.IP = pIP;
}

int cpu_get_offset(){
    return aCPU.offset;
}

void cpu_set_offset(int pOffset){
    aCPU.offset = pOffset;
}

void cpu_empty(){
    aCPU.quanta = 2;
}

void load_PCB_TO_CPU(int PC, int offset){
    cpu_set_ip(PC);
    cpu_set_offset(offset);
}

/*
 * Function:  cpu_run 
 * 	Added in A2
 * --------------------
 * run "quanta"(first input parameter) lines of code, starting from the cpu.IP
 *
 * quanta: number of lines that the CPU will run before it switches task or the task ends
 * end: the last line of the task in shell memory
 * returns: error code, 2: file reaches an end, 0: no error
 */
int cpu_run(int quanta){
    aCPU.quanta = quanta;
    int error_code;
    while (aCPU.quanta != 0){
        
        strncpy(aCPU.IR, mem_get_value_by_line(aCPU.IP),1000);
        parseInput(aCPU.IR);
        aCPU.quanta -= 1;
        aCPU.offset = aCPU.offset + 1;
        if(aCPU.offset > 2){
            return aCPU.quanta;
        }
        aCPU.IP = aCPU.IP + 1;
    }
    
    error_code = 0;
    return error_code;
}