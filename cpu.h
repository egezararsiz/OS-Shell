#ifndef CPU_H
#define CPU_H

int cpu_get_ip();
int cpu_get_offset();
void cpu_empty();
void load_PCB_TO_CPU(int PC, int offset);
int cpu_run(int quanta);
#endif
