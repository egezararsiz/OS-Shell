#include "pcb.h"
#include "kernel.h"
#include "cpu.h"
#include "shell.h"
#include "shellmemory.h"
#include "interpreter.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BACKSTORE "NEZShellBackStore"
#define QUEUE_LENGTH 10
#define MAX_INT 2147483646
PCB* readyQueue[QUEUE_LENGTH];

void ready_queue_initialize()
{
    for (size_t i = 0; i < QUEUE_LENGTH; ++i)
    {
        readyQueue[i] = (PCB*)malloc(sizeof(PCB));
        (*readyQueue[i]).PC = -1;
        (*readyQueue[i]).pid = NULL;
        (*readyQueue[i]).job_length_score = -1;
        (*readyQueue[i]).offset = -1;
        (*readyQueue[i]).curr_page = -1;
        (*readyQueue[i]).total_pages = -2;
        (*readyQueue[i]).last_page_offset = -2;
        (*readyQueue[i]).file = NULL;
        for(int j = 0; j < 10 ; j++){
            (*readyQueue[i]).pages[j] = -1;
        }
    }
}

void print_queue(){
    for (size_t i = 0; i < QUEUE_LENGTH; ++i)
    {
        printf("%d\n",(*readyQueue[i]).PC);
    }
}

void ready_queue_Empty(){
    for (size_t i = 0; i < QUEUE_LENGTH; ++i)
    {
        (*readyQueue[i]).PC = -1;
        (*readyQueue[i]).pid = NULL;
        (*readyQueue[i]).job_length_score = -1;
        (*readyQueue[i]).offset = -1;
        (*readyQueue[i]).curr_page = -1;
        (*readyQueue[i]).total_pages = -2;
        (*readyQueue[i]).last_page_offset = -2;
        (*readyQueue[i]).file = NULL;
        for(int j = 0; j < 10 ; j++){
            (*readyQueue[i]).pages[j] = -1;
        }
    }
}

void ready_queue_destory()
{
    for (size_t i = 0; i < QUEUE_LENGTH; ++i)
    {
        free(readyQueue[i]);
    }
}

PCB ready_queue_pop(int index, bool inPlace)
{
    PCB head = (*readyQueue[index]);
    if(inPlace){
        for (size_t i = index+1; i < QUEUE_LENGTH; i++){
            (*readyQueue[i-1]).PC = (*readyQueue[i]).PC;
            (*readyQueue[i-1]).pid = (*readyQueue[i]).pid;
            (*readyQueue[i-1]).job_length_score = (*readyQueue[i]).job_length_score;
            (*readyQueue[i-1]).offset = (*readyQueue[i]).offset;
            (*readyQueue[i-1]).curr_page = (*readyQueue[i]).curr_page;
            (*readyQueue[i-1]).total_pages = (*readyQueue[i]).total_pages;
            (*readyQueue[i-1]).last_page_offset= (*readyQueue[i]).last_page_offset;
            (*readyQueue[i-1]).file = (*readyQueue[i]).file;
            for(int j = 0; j < 10 ; j++){
                (*readyQueue[i-1]).pages[j] = (*readyQueue[i]).pages[j];
            }
        }
        (*readyQueue[QUEUE_LENGTH-1]).PC = -1;
        (*readyQueue[QUEUE_LENGTH-1]).pid = NULL;
        (*readyQueue[QUEUE_LENGTH-1]).job_length_score = -1;
        (*readyQueue[QUEUE_LENGTH-1]).offset = -1;
        (*readyQueue[QUEUE_LENGTH-1]).curr_page = -1;
        (*readyQueue[QUEUE_LENGTH-1]).total_pages = -2;
        (*readyQueue[QUEUE_LENGTH-1]).last_page_offset = -2;
        (*readyQueue[QUEUE_LENGTH-1]).file = NULL;
        for(int j = 0; j < 10 ; j++){
            (*readyQueue[QUEUE_LENGTH-1]).pages[j] = -1;
        }
    }
    return head;
}

void removeframePCB(int frame, PCB newPCB){
    for (size_t i = 0; i < QUEUE_LENGTH; ++i){
        for(int j = 0; j < 10 ; j++){
            if((*readyQueue[i]).pages[j] == frame && strcmp((*readyQueue[i]).pid, newPCB.pid) != 0){
                for(i=j-1; i<(*readyQueue[i]).total_pages-1; i++)
            {
                (*readyQueue[i]).pages[i] = (*readyQueue[i]).pages[i+1];
            }
                (*readyQueue[i]).total_pages = (*readyQueue[i]).total_pages - 1;
            }          
        }
    }

}

void ready_queue_add_to_end(PCB *pPCB)
{
    for(int i = 0; i < QUEUE_LENGTH; i++){
        if ( (*readyQueue[i]).PC == -1 ){
            (*readyQueue[i]).PC = (*pPCB).PC;
            (*readyQueue[i]).pid = (*pPCB).pid;
            (*readyQueue[i]).job_length_score = (*pPCB).job_length_score;
            (*readyQueue[i]).offset = (*pPCB).offset;
            (*readyQueue[i]).curr_page = (*pPCB).curr_page;
            (*readyQueue[i]).total_pages = (*pPCB).total_pages;
            (*readyQueue[i]).last_page_offset = (*pPCB).last_page_offset;
            (*readyQueue[i]).file = (*pPCB).file;
            for(int j = 0; j < 10 ; j++){
                (*readyQueue[i]).pages[j] = (*pPCB).pages[j];
            }
            break;
        }
    }
}


void ready_queue_add_to_front(PCB *pPCB){
    for (size_t i = QUEUE_LENGTH-1; i > 0; i--){
        (*readyQueue[i]).PC = (*readyQueue[i-1]).PC;
        (*readyQueue[i]).pid = (*readyQueue[i-1]).pid;
        (*readyQueue[i]).job_length_score = (*readyQueue[i-1]).job_length_score;
        (*readyQueue[i]).offset = (*readyQueue[i-1]).offset;
        (*readyQueue[i]).curr_page = (*readyQueue[i-1]).curr_page;
        (*readyQueue[i]).total_pages = (*readyQueue[i-1]).total_pages;
        (*readyQueue[i]).last_page_offset = (*readyQueue[i-1]).last_page_offset;
        (*readyQueue[i]).file = (*readyQueue[i-1]).file;
        for(int j = 0; j < 10 ; j++){
            (*readyQueue[i]).pages[j] = (*readyQueue[i-1]).pages[j];
        }      
    }
    // readyQueue[0] = pPCB;
    (*readyQueue[0]).PC = (*pPCB).PC;
    (*readyQueue[0]).pid = (*pPCB).pid;
    (*readyQueue[0]).job_length_score = (*pPCB).job_length_score;
    (*readyQueue[0]).offset = (*pPCB).offset;
    (*readyQueue[0]).curr_page = (*pPCB).curr_page;
    (*readyQueue[0]).total_pages = (*pPCB).total_pages;
    (*readyQueue[0]).last_page_offset = (*pPCB).last_page_offset;
    (*readyQueue[0]).file = (*pPCB).file;
    for(int j = 0; j < 10 ; j++){
        (*readyQueue[0]).pages[j] = (*pPCB).pages[j];
    }
}


bool is_ready_empty(){
    for (size_t i = 0; i < QUEUE_LENGTH; ++i)
    {
        if((*readyQueue[i]).PC != -1){
            return false;
        }  
    }
    return true;
}


void terminate_task_in_queue_by_index(int i){
    (*readyQueue[i]).PC = -1; 
    (*readyQueue[i]).pid = NULL;
    (*readyQueue[i]).job_length_score = -1;
    (*readyQueue[i]).offset = -1;
    (*readyQueue[i]).curr_page = -1;
    (*readyQueue[i]).total_pages = -2;
    (*readyQueue[i]).last_page_offset = -2;
    (*readyQueue[i]).file = NULL;
    for(int j = 0; j < 10 ; j++){
        (*readyQueue[i]).pages[j] = -1;
    }
}

int myinit(const char *filename){
    FILE* fp;
    char dest_path[256];
	snprintf(dest_path,sizeof(dest_path),"%s%s%s",BACKSTORE,"/",filename);
    int error_code = 0;
    
    fp = fopen(filename, "rt");
    if(fp == NULL){
        error_code = 11; // 11 is the error code for file does not exist
        return error_code;
    }
    FILE* copy = fopen(dest_path, "w+");

    //generate a random ID as file ID
    char* fileID = (char*)malloc(32);
    sprintf(fileID, "%d", rand());
    PCB* newPCB = makePCB(fileID,copy);
    error_code = copyfiles(fp,copy,filename);
    fclose(fp);
    loadpages(newPCB,copy,1,fileID);
    //error_code = add_file_to_mem(fp, start, end, fileID);
    if(error_code != 0){
        //fclose(fp);
        return error_code;
    }
    newPCB -> job_length_score = 1;

    ready_queue_add_to_end(newPCB);

    //fclose(fp);

    return error_code;
}

int get_scheduling_policy_number(char* policy){
    if(strcmp("FCFS",policy)==0){
        return 0;
    }else if(strcmp("SJF",policy)==0){
        return 1;
    }else if(strcmp("RR",policy)==0){
        return 2;
    }else if(strcmp("AGING",policy)==0){
        return 3;
    }else{
        //error code 15
        return 15;
    }
}


/*
 * Variable:  schedulingPolicy 
 * --------------------
 * 0: FCFS
 * 1: SJF
 * 2: RR
 * 3: AGING
 */
int scheduler(int policyNumber){
    int ln;
    int error_code = 0;
    int cpu_quanta_per_program = 2;

    //FCFS and SJF: running program will stop when it finishes
    if( policyNumber == 0 || policyNumber == 1 ){
        cpu_quanta_per_program = MAX_INT;
    }else if(policyNumber == 3){
        cpu_quanta_per_program = 1;
    }
    //print_mem_index();
    //scheduling logic for 0: FCFS and 2: RR
    if(policyNumber == 2){
        //keep running programs while ready queue is not empty
        while(ready_queue_pop(0,false).PC != -1)
        {
            //printf("kernel\n");
            //print_list();
            PCB firstPCB = ready_queue_pop(0,false);
            firstPCB.PC = firstPCB.pages[firstPCB.curr_page] * 3 + firstPCB.offset;
            //printf("%d\n",firstPCB.PC);
            //printf("%d\n",firstPCB.curr_page);
            load_PCB_TO_CPU(firstPCB.PC,firstPCB.offset);
            if(firstPCB.curr_page == firstPCB.total_pages && (firstPCB.last_page_offset - firstPCB.offset) < cpu_quanta_per_program){
                    //printf("%d,%d, %d\n",firstPCB.last_page_offset,firstPCB.offset, firstPCB.total_pages);
                    cpu_quanta_per_program = (firstPCB.last_page_offset - firstPCB.offset);
            }            
            //removeNode(firstPCB.curr_page);
            //append(firstPCB.curr_page);
            int error_code_load_PCB_TO_CPU = cpu_run(cpu_quanta_per_program);

            if(error_code_load_PCB_TO_CPU == 0 && cpu_get_offset() != 3){
                if(firstPCB.curr_page == firstPCB.total_pages && firstPCB.last_page_offset - firstPCB.offset == 0){
                    //clean_mem(firstPCB.pages[firstPCB.curr_page] * 3, cpu_get_ip());
                    fclose(firstPCB.file);
                    firstPCB.file = NULL;
                    ready_queue_pop(0,true);
                    //print_mem_index();
                }
                else{
                firstPCB.PC = cpu_get_ip();
                firstPCB.offset = cpu_get_offset();
                ready_queue_pop(0,true);
                ready_queue_add_to_end(&firstPCB);
                cpu_quanta_per_program = 2;
                }
            }
            else if(error_code_load_PCB_TO_CPU == 0 && cpu_get_offset() == 3){
                //clean_mem(firstPCB.pages[firstPCB.curr_page] * 3, cpu_get_ip());
                if(firstPCB.curr_page == firstPCB.total_pages){
                    if(firstPCB.file != NULL){
                        for(int i = 0; i < 10; i++) firstPCB.pages[i] = -1;
                        firstPCB.curr_page = 0;
                        firstPCB.offset = 0;
                        ln = loadpages((&firstPCB),firstPCB.file,0,firstPCB.pid);
                        if(ln != 100) removeframePCB(ln, firstPCB);
                        ready_queue_pop(0,true);
                        ready_queue_add_to_end(&firstPCB);
                        cpu_quanta_per_program = 2;        
                    }
                    else{
                    ready_queue_pop(0,true);
                    }
                }
                else{
                firstPCB.curr_page = firstPCB.curr_page+1;
                firstPCB.offset = 0;
                ready_queue_pop(0,true);
                ready_queue_add_to_end(&firstPCB);
                cpu_quanta_per_program = 2;
                }
            }
            else if(error_code_load_PCB_TO_CPU == 1){
                //clean_mem((firstPCB.pages[firstPCB.curr_page] * 3), cpu_get_ip());
                if(firstPCB.curr_page == firstPCB.total_pages){
                    if(firstPCB.file != NULL){
                        for(int i = 0; i < 10; i++) firstPCB.pages[i] = -1;
                        ln = loadpages((&firstPCB),firstPCB.file,0,firstPCB.pid);
                        if(ln != 100) removeframePCB(ln,firstPCB);
                        ready_queue_pop(0,true);
                        ready_queue_add_to_end(&firstPCB);
                    }
                    else{
                    ready_queue_pop(0,true);
                    }
                }
                else{
                firstPCB.curr_page = firstPCB.curr_page+1;
                firstPCB.offset = 0;
                cpu_quanta_per_program = 1;
                ready_queue_pop(0,true);
                ready_queue_add_to_front(&firstPCB);
                }
            }
            else return -1;
            
        }
        ready_queue_Empty();
        cpu_empty();
    }
    else return -1;
    /*
    //scheduling policy for 1: SJF
    if(policyNumber == 1){
        while(!is_ready_empty())
        {
            //task with the lowest lines of codes runs first
            int task_index_with_the_least_lines;
            int task_lines = MAX_INT;
            //get the lowest job length 
            for(int i = 0; i < QUEUE_LENGTH; i++){
                if((*readyQueue[i]).start != -1 && 1 + (*readyQueue[i]).end - (*readyQueue[i]).start < task_lines){
                    task_lines = 1 + (*readyQueue[i]).end - (*readyQueue[i]).start;
                    task_index_with_the_least_lines = i;
                }
            }

            PCB current_task_PCB = (*readyQueue[task_index_with_the_least_lines]);
            load_PCB_TO_CPU(current_task_PCB.PC);
            
            int error_code_load_PCB_TO_CPU = cpu_run(cpu_quanta_per_program, current_task_PCB.end);
            
            //the head PCB program has been done, time to reclaim the shell mem
            clean_mem(current_task_PCB.start, current_task_PCB.end);
            //put the current PCB into invalid mode
            terminate_task_in_queue_by_index(task_index_with_the_least_lines);
        }
    }

    //scheduling policy for 3: Aging
    if(policyNumber == 3){
        int task_index_least_job_length_score;
        int task_job_length_score = MAX_INT;

        //find job with the lowest job score
        for(int i = 0; i < QUEUE_LENGTH; i++){
            //get the lowest job length score
            if((*readyQueue[i]).start != -1 && (*readyQueue[i]).job_length_score < task_job_length_score){
                task_job_length_score = (*readyQueue[i]).job_length_score;
                task_index_least_job_length_score = i;
            }
        }
        //move the task with the lowest job score to the front of the queue
        PCB job_with_lowest_job_score = ready_queue_pop(task_index_least_job_length_score,true);
        ready_queue_add_to_front(&job_with_lowest_job_score);
        
        while(!is_ready_empty())
        {
            //task with the lowest job length score runs first
            //in this case, the task with the lowest job length score is the first task in queue
            task_job_length_score = (*readyQueue[0]).job_length_score;
            task_index_least_job_length_score = 0;

            PCB current_task_PCB = (*readyQueue[task_index_least_job_length_score]);
            load_PCB_TO_CPU(current_task_PCB.PC);
            
            int error_code_load_PCB_TO_CPU = cpu_run(cpu_quanta_per_program, current_task_PCB.end);

            if(error_code_load_PCB_TO_CPU == 2){
                //the head PCB program has been done, time to reclaim the shell mem
                clean_mem((*readyQueue[task_index_least_job_length_score]).start, (*readyQueue[task_index_least_job_length_score]).end);
                ready_queue_pop(task_index_least_job_length_score, true);
                task_job_length_score = MAX_INT;
            }

            if(error_code_load_PCB_TO_CPU == 0){
                //the head PCB program has finished its quanta
                (*readyQueue[task_index_least_job_length_score]).PC = cpu_get_ip(); // update the PC for the PCB
                //Age all the tasks (other than the current executing task) in queue by 1
                for(int i = 0; i < QUEUE_LENGTH; i++){
                    //get the lowest job length score
                    if((*readyQueue[i]).start != -1 && (*readyQueue[i]).job_length_score > 0 && i != task_index_least_job_length_score){
                        (*readyQueue[i]).job_length_score -= 1;
                    }
                }
            }
            
            //if the first task job score is not the lowest, 
            //then move the frst task to the end 
            //and the lowest job score task to the front
            for(int i = 0; i < QUEUE_LENGTH; i++){
                //get the lowest job length score
                if((*readyQueue[i]).start != -1 && (*readyQueue[i]).job_length_score < task_job_length_score){
                    task_job_length_score = (*readyQueue[i]).job_length_score;
                    task_index_least_job_length_score = i;
                }
            }
            if(task_index_least_job_length_score != 0){
                //pop the task with the lowest job score 
                PCB lowest_job_score_task = ready_queue_pop(task_index_least_job_length_score, true);
                //move the frst task to the end
                PCB first_pcb = ready_queue_pop(0, true);
                ready_queue_add_to_end(&first_pcb);
                //move the lowest job score task to the front
                ready_queue_add_to_front(&lowest_job_score_task);
            }
        
        }
    }
    */
   exec_cleanup();
   print_mem_index();
    //clean up
    return error_code;
}