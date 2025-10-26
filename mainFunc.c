#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "pcbInitialize.h"
#include "decodeExec.h"

#define MAX 5

double noOfTimes[MAX];
int turn = 0;
FILE *fplog;

void SwitchThread(int tid)
{
    int i;
    for (i = (tid + 1) % MAX; noOfTimes[i] == 0; i = (i + 1) % MAX)
        if (i == tid) return;
    turn = i;
}

void *thread_execution(void *pid)
{
    intptr_t i = (intptr_t)pid;
    char line[BUFSIZ];
    int cnt2 = 0;

    FILE *fptr = fopen(p_queue[i].p_name, "r");
    if (!fptr) pthread_exit(0);

    while (noOfTimes[i] != 0)
    {
        while (turn != i);
        int cnt = (noOfTimes[i] > time_slice) ? time_slice : noOfTimes[i];

        for (cnt2 = 0; cnt2 < cnt; cnt2++)
        {
            fprintf(fplog, "\nProcess name: %s\n", p_queue[i].p_name);
            if (!fgets(line, sizeof(line), fptr)) break;
            decodeExec(line, fplog);
            p_queue[i].insNo++;
        }
        noOfTimes[i] -= cnt;
        fprintf(fplog, "\n----------\n");
        SwitchThread(i);
    }
    fclose(fptr);
    pthread_exit(0);
}

int mainFunc(int ch)
{
    pcbInitialize(ch);

    if (ch == 1)
    {
        fplog = fopen("logs_rr_with_thread", "a");
        pthread_t threads[MAX];
        int i, status;

        for (i = 0; i < MAX; i++)
            noOfTimes[i] = p_queue[i].cpu_burst_time;

        for (i = 0; i < MAX; i++)
        {
            status = pthread_create(&threads[i], NULL, thread_execution, (void *)(intptr_t)i);
            if (status != 0)
            {
                printf("Thread %d creation failed\n", i);
                exit(-1);
            }
        }

        for (i = 0; i < MAX; i++)
            pthread_join(threads[i], NULL);

        fprintf(fplog, "\n<--- END of Log file --->");
        fclose(fplog);
        printf("\nLog file 'logs_rr_with_thread' generated!\n");
    }
    else
    {
        fplog = fopen("logs_rr_without_thread", "a");
        int i, j, counter = 0, checkProcessCompleted[MAX] = {0};
        char line[BUFSIZ];

        for (i = 0; i < MAX; i++)
            noOfTimes[i] = p_queue[i].cpu_burst_time;

        i = 0;
        while (counter < MAX)
        {
            if (i == MAX) i = 0;
            if (checkProcessCompleted[i])
            {
                i++;
                continue;
            }

            FILE *fptr = fopen(p_queue[i].p_name, "r");
            if (!fptr) { i++; continue; }

            for (int ln = 0; ln < p_queue[i].insNo; ln++)
                fgets(line, sizeof(line), fptr);

            int cnt = (p_queue[i].cpu_burst_time - p_queue[i].insNo > time_slice) ? time_slice : (p_queue[i].cpu_burst_time - p_queue[i].insNo);

            for (j = 0; j < cnt; j++)
            {
                if (!fgets(line, sizeof(line), fptr)) break;
                decodeExec(line, fplog);
                p_queue[i].insNo++;
            }

            if (p_queue[i].insNo == p_queue[i].cpu_burst_time)
            {
                checkProcessCompleted[i] = 1;
                counter++;
            }
            fclose(fptr);
            fprintf(fplog, "\n----------\n");
            i++;
        }

        fprintf(fplog, "\n<--- END of Log file --->");
        fclose(fplog);
        printf("\nLog file 'logs_rr_without_thread' generated!\n");
    }

    return 0;
}

