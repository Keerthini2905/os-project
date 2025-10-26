#include "pcbInitialize.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>

struct pcb p_queue[MAX_PROCESS];
int noOfFiles = 0;
int time_slice = 2;

void pcbInitialize(int ch)
{
    srand(time(0));
    time_t t = time(0);

    struct dirent *dp;
    
char pname[10][512];
    DIR *fd = opendir("processes");

    if (!fd) {
        perror("Failed to open processes directory");
        return;
    }

    while ((dp = readdir(fd)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
            continue;

        // Ensure the string never exceeds buffer size
snprintf(pname[noOfFiles], sizeof(pname[noOfFiles]), "processes/%s", dp->d_name);
pname[noOfFiles][sizeof(pname[noOfFiles]) - 1] = '\0';  // force null-termination

        noOfFiles++;
    }
    closedir(fd);

    FILE *f1, *fplog;
    double count = 0;
    char c;
    int i;

    if (ch == 1)
        fplog = fopen("logs_rr_with_thread", "w");
    else
        fplog = fopen("logs_rr_without_thread", "w");

    fprintf(fplog, "<--- START of Round Robin Log file --->\n\nTime slice = %d instructions\n\nReady Queue:\nFront -> ", time_slice);

    for (i = 0; i < noOfFiles; i++)
    {
        count = 0;
        if (i == noOfFiles - 1)
            fprintf(fplog, "%s\n\n----------", pname[i]);
        else
            fprintf(fplog, "%s | ", pname[i]);

        p_queue[i].p_id = i + 1;
        p_queue[i].insNo = 0;
        strcpy(p_queue[i].p_name, pname[i]);
        p_queue[i].arrival_time = difftime(time(0), t);
        p_queue[i].priority = rand() % 10;

        f1 = fopen(p_queue[i].p_name, "r");
        if (!f1) continue;
        for (c = getc(f1); c != EOF; c = getc(f1))
            if (c == '\n') count++;
        p_queue[i].cpu_burst_time = count;
        fclose(f1);
    }

    fprintf(fplog, "\n");
    fclose(fplog);
}

