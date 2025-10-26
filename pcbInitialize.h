#ifndef PCBINITIALIZE_H
#define PCBINITIALIZE_H

#include <stdio.h>

#define MAX_PROCESS 10

// Structure of PCB
struct pcb
{
    int p_id;
    double insNo;
    char p_name[50];
    double arrival_time;
    double cpu_burst_time;
    int priority;
};

extern struct pcb p_queue[MAX_PROCESS];
extern int time_slice;
extern int noOfFiles;

// Function prototype
void pcbInitialize(int ch);

#endif

