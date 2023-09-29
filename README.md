# OS-Shell

## Introduction

This repository contains the code for a simulated OS developed. The simulated OS supports various Shell commands, process management, scheduling techniques, and implements demand paging with demand loading and LRU page replacement.

## Functionalities

### Implementing the scheduling infrastructure
First, I built the basic scheduling infrastructure.

• Code Loading
  - Load the entire source code of the SCRIPT file into the OS Shell memory.
  - Create a data structure (PCB) to hold the SCRIPT information.
  - Track the spot in the Shell memory where the SCRIPT instructions are loaded.

• Ready Queue
  - Create a data structure for the ready queue.
  - Add PCBs of all processes currently executing.

• Scheduler Logic
  - Add the PCB for SCRIPT to the tail of the ready queue.
  - Implement FCFS scheduling policy.
  - Switch processes in and out of the ready queue.

• Clean-up
  - Remove the SCRIPT source code from the Shell memory after termination.

### Extend the OS Shell with the exec command
Second, I added concurrent process execution to our shell.

• Extend Shell Interface
  - Add the exec command to execute up to 3 concurrent programs with a given scheduling policy.

• Exec Behavior
  - Allow multiple instances of the same program to be executed.

• Scheduling Policies
  - Extend scheduler to support SJF and RR policies.

• SJF with Job Aging
  - Implement aging mechanism to promote longer running jobs.

 ### Implement Demand Paging
 Third, I allowed programs larger than the shell memory size to be run by my OS.

• Paging Infrastructure
  - Set up the backing store for paging.
  - Partition the Shell memory into frame store and variable store.
    
• Code Loading
  - Load first two pages of each program into the frame store dynamically.

• Handling Page Faults
  - Implement page fault handling, including loading missing pages and eviction.

• Page Replacement Policy
  - Implement Least Recently Used (LRU) page replacement policy.
