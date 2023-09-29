#include<stdlib.h>
#include<stdio.h>
#include "pcb.h"

struct node{
    int frame;
    struct node *next;
};

int removeBackStoreFiles();
void print_mem_index();
void exec_cleanup();
void mem_init();
char *mem_get_value(char *var);
char* mem_get_value_by_line(int line);
void mem_set_value(char *var, char *value);
void clean_mem(int start, int end);
int add_file_to_mem(FILE* fp, int* pStart, int* pEnd, char* fileID);
int copyfiles(FILE* fp, FILE* copy, const char *filename);
int loadpages(PCB* newPCB, FILE* fp, int initial, char* fileID);
int resetmem();
void append(int frame);
void removeNode(int frame);
int pop();
void print_list();
