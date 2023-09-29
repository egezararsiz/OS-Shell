#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "pcb.h"

#define SHELL_MEM_LENGTH 1000
#define INITIAL_FRAMESTORE_SIZE 100
#define INITIAL_VARSTORE_SIZE 100
#define BACKSTORE "NEZShellBackStore"

int framesize=FRAMESIZE;
int varmemsize=VARMEMSIZE;

struct node{
    int frame;
    struct node *next;
};
struct node *head = NULL;

//appends node to the end of the ready queue
void append(int frame){
    struct node* add = malloc(sizeof(struct node));
    add -> frame = frame;
    add -> next = NULL; //dynamically create new node and make its data passed PCB
   
    //if queue is empty or not, act accordingly
    if(head == NULL){
        head = add; //empty queue, this is the head (also tail but i didn't use tail)
    }
    else{   //if not empty queue, go to end of the queue and append
        struct node *curr = head;
        while (curr -> next != NULL){
            curr = curr -> next;
        }
        curr -> next = add;
    }
}

void print_list(){
    struct node *current = head;
    if (head == NULL) printf("%s\n", "empty"); //if empty
    while (current != NULL){    //print each element
        printf("%d\n", current -> frame);
        current = current -> next;
    }
}

void removeNode(int frame){
	// Store head node
    struct node *temp = head, *prev;
 
    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->frame == frame) {
    	head = temp->next; // Changed head
        free(temp); // free old head
        return;
    }
 
    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->frame != frame) {
        prev = temp;
        temp = temp->next;
    }
 
    // If key was not present in linked list
    if (temp == NULL)
        return;
 
    // Unlink the node from linked list
    prev->next = temp->next;
 
    free(temp); // Free memory
	
}

//removes the head of the linked list / ready queue
int pop(){
    int retval;
    struct node *next_node = NULL;
    struct node *toremove;  //initialize necessary structs
    
    if (head == NULL) return -1; //head empty return null
    
    next_node = head->next; //remove head but don't forget to assign links and new head
    retval = head->frame; 
    toremove = head;
    head = next_node; 
    free(toremove); //free the memory
    return retval;
}


struct memory_struct{
	char *var;
	char *value;
};

struct memory_struct shellmemory[SHELL_MEM_LENGTH];

int removeBackStoreFiles(){
    DIR *folder = opendir(BACKSTORE);
    struct dirent *next_file;
    char filepath[300];

    while ( (next_file = readdir(folder)) != NULL )
    {
        sprintf(filepath, "%s/%s", BACKSTORE, next_file->d_name);
        remove(filepath);
    }
    closedir(folder);
    return 0;
}

// Shell memory functions
void print_mem_index(){
	int i;
	for (i=0; i<SHELL_MEM_LENGTH; i++){		
		if(shellmemory[i].var != "none") printf("%d\n",i);
	}
}

void exec_cleanup(){
	int i;
	for (i=0; i<SHELL_MEM_LENGTH; i++){		
		if(shellmemory[i].var != "none"){
			shellmemory[i].var = "none";
			shellmemory[i].value = "none";
		}
	}
}

void mem_init(){
	printf("Frame Store Size = %d; Variable Store Size = %d\n",framesize,varmemsize);
	char command[55];
	int i;
	for (i=0; i<SHELL_MEM_LENGTH; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
	struct stat st = {0};
    if (stat("NEZShellBackStore", &st) == -1) {
    	mkdir("NEZShellBackStore", 0700);
	}
	else{
		removeBackStoreFiles();
	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {

	int i;

	for (i=SHELL_MEM_LENGTH - 1; i>SHELL_MEM_LENGTH - varmemsize; i--){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=SHELL_MEM_LENGTH - 1; i>900; i--){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;

	for (i=SHELL_MEM_LENGTH - 1; i>SHELL_MEM_LENGTH - varmemsize; i--){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";

}

char* mem_get_value_by_line(int line){
	return shellmemory[line].value;
}


void clean_mem(int start, int end){
    for(int i = start; i <= end; i ++){
        shellmemory[i].var = "none";
		shellmemory[i].value = "none";
    }
}

/*
 * Function:  addFileToMem 
 * 	Added in A2
 * --------------------
 * Load the source code of the file fp into the shell memory:
 * 		Loading format - var stores fileID, value stores a line
 *		Note that the first 100 lines are for set command, the rests are for run and exec command
 *
 *  pStart: This function will store the first line of the loaded file 
 * 			in shell memory in here
 *	pEnd: This function will store the last line of the loaded file 
 			in shell memory in here
 *  fileID: Input that need to provide when calling the function, 
 			stores the ID of the file
 * 
 * returns: error code, 21: no space left
 */
int add_file_to_mem(FILE* fp, int* pStart, int* pEnd, char* fileID)
{
    char line[SHELL_MEM_LENGTH];
    size_t i;
    int error_code = 0;
	bool hasSpaceLeft = false;

    for (i = 100; i < SHELL_MEM_LENGTH; i++){
        if(strcmp(shellmemory[i].var,"none") == 0){
            *pStart = (int)i;
			hasSpaceLeft = true;
            break;
        }
    }

	//shell memory is full
	if(hasSpaceLeft == 0){
		error_code = 21;
		return error_code;
	}
    
    for (size_t j = i; j < SHELL_MEM_LENGTH; j++){
        if(feof(fp))
        {
            *pEnd = (int)j-1;
            break;
        }else{
            fgets(line, 999, fp);
			shellmemory[j].var = strdup(fileID);
            shellmemory[j].value = strdup(line);
        }
    }

	//no space left to load the entire file into shell memory
	if(!feof(fp)){
		error_code = 21;
		//clean up the file in memory
		for(int j = 1; i <= SHELL_MEM_LENGTH; i ++){
			shellmemory[j].var = "none";
			shellmemory[j].value = "none";
    	}
		return error_code;
	}

    return error_code;
}

int firstAvailablePage()
{
	int i;
	for (i=0; i<framesize -2; i++){		
		if(shellmemory[i].var == "none" && 
		shellmemory[i+1].var == "none" &&
		shellmemory[i+2].var == "none" && i%3 == 0){
			//printf("%d\n",i);
			return i/3;
	}}
	return -1;
}
int copyfiles(FILE* fp, FILE* copy, const char *filename)
{
	while (1)
    {
        char buffer[256];
        int ret = fread(buffer, 1, sizeof(buffer), fp);
        if (ret == 0)
            break;
        fwrite(buffer, 1, ret, copy);
    }
	rewind(copy);
	rewind(fp);
	return 0;
}

int loadpages(PCB* newPCB, FILE* fp, int initial, char* fileID){
	//printf("loadpages\n");
	//print_list();
	int ln = -100;
	char line[1000];
	int tempfrno = firstAvailablePage();
	//printf("tempfrno: %d\n",tempfrno);
	int pgno = 0;
	int tracker = 0;
	int ct = 0;
	if(tempfrno == -1 && !feof(fp)){
		ln = pop();
		char* line1 = mem_get_value_by_line(ln * 3);
		char* line2 = mem_get_value_by_line(ln * 3 + 1);
		char* line3 = mem_get_value_by_line(ln * 3 + 2);
		printf("Page fault! Victim page contents:\n \
%s\n \
%s\n \
%s\n \
End of victim page contents.\n",line1,line2,line3);
		clean_mem(0,3);
		//newPCB->curr_page = newPCB->curr_page + 1;
		//newPCB->total_pages = newPCB->total_pages - 1;
		tempfrno = firstAvailablePage();
	}
	removeNode(tempfrno);
	append(tempfrno);
	if(initial != 1){
		ct = 3;
	}
	while (!feof(fp) && ct < 6){
		newPCB -> pages[pgno] = tempfrno;	
		fgets(line,999,fp);
		ct++;
		shellmemory[tempfrno * 3 + tracker].var = strdup(fileID);
		shellmemory[tempfrno * 3 + tracker].value = strdup(line);
		tracker++;
		if(tracker > 2){
			pgno++;
			tempfrno = firstAvailablePage();
			if(tempfrno != -1){
			removeNode(tempfrno);
			append(tempfrno);
			}
			tracker = 0;
		}
	}
	
	if(tracker == 0){
		//print_list();
		removeNode(tempfrno);
		//printf("---------------\n");
		//print_list();
		//printf("$$$$$$$$$$\n");
		pgno--;
		tracker = 3;
	}
	newPCB -> file = fp;
	newPCB -> total_pages = pgno;
	newPCB -> last_page_offset = tracker;
	newPCB -> PC = newPCB -> pages[0];
	if(ln != -100) return ln;
	return 100;

}

int resetmem(){
	int error_code = 0;
	for(int i = 900; i < SHELL_MEM_LENGTH; i++){
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
    }
	return error_code;

}