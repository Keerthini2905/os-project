#include "decodeExec.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void decodeExec(char instruction[], FILE *fplog)
{
    fprintf(fplog, "Instruction: %s", instruction);

    char *ch;
    char insParams[10][BUFSIZ];
    int i = 0, j = 0, l = 0;

    ch = strtok(instruction, " ");
    while (ch != NULL)
    {
        strcpy(insParams[i], ch);
        ch = strtok(NULL, " ");
        i++;
    }

    char tempT[BUFSIZ];
    for (l = 0; insParams[3][l] != '\n' && insParams[3][l] != '\0'; l++)
        tempT[l] = insParams[3][l];
    tempT[l] = '\0';

    char operation[4];
    strcpy(operation, insParams[0]);

    FILE *fin = fopen("variables.txt", "r");
    char line[BUFSIZ], var1[BUFSIZ], var2[BUFSIZ];
    int val1, val2, var1Found = 0, var2Found = 0;

    if (fin)
    {
        while (fgets(line, sizeof(line), fin) && (!(var1Found && var2Found)))
        {
            ch = strtok(line, " ");
            if (strcmp(ch, insParams[1]) == 0)
            {
                var1Found = 1;
                ch = strtok(NULL, " ");
                strcpy(var1, ch);
            }
            if (strcmp(ch, insParams[2]) == 0)
            {
                var2Found = 1;
                ch = strtok(NULL, " ");
                strcpy(var2, ch);
            }
        }
        fclose(fin);
    }

    val1 = var1Found ? atoi(var1) : atoi(insParams[1]);
    val2 = var2Found ? atoi(var2) : atoi(insParams[2]);

    int opCodes[10], finalOpCode = 0;
    char opNames[10][4];

    fin = fopen("opcode.txt", "r");
    if (fin)
    {
        i = 0;
        while (fgets(line, sizeof(line), fin))
        {
            ch = strtok(line, " ");
            strcpy(opNames[i], ch);
            ch = strtok(NULL, " ");
            opCodes[i] = atoi(ch);
            i++;
        }
        fclose(fin);
    }

    for (j = 0; j < i; j++)
        if (strcmp(opNames[j], operation) == 0)
            finalOpCode = opCodes[j];

    fprintf(fplog, "Decoded instruction: %d %d %d %s\n", finalOpCode, val1, val2, tempT);

    int result;
    switch (finalOpCode)
    {
    case 1: result = val1 + val2; break;
    case 2: result = val1 - val2; break;
    case 3: result = val1 * val2; break;
    case 4: result = val1 / val2; break;
    case 5: result = val1 % val2; break;
    }

    FILE *fr = fopen("variables.txt", "r");
    FILE *fw = fopen("variablesNew.txt", "w");
    int flag = 0;

    if (fr && fw)
    {
        while (fgets(line, sizeof(line), fr))
        {
            char temp1[BUFSIZ];
            strcpy(temp1, line);
            ch = strtok(line, " ");
            if (strcmp(ch, tempT) == 0)
            {
                flag = 1;
                char temp[BUFSIZ];
                strcpy(temp, ch);
                strcat(temp, " ");
                char resultStr[BUFSIZ];
                sprintf(resultStr, "%d", result);
                strcat(temp, resultStr);
                strcat(temp, "\n");
                fputs(temp, fw);
            }
            else
                fputs(temp1, fw);
        }
        fclose(fr);
        fclose(fw);
        system("rm variables.txt");
        system("mv variablesNew.txt variables.txt");
    }

    if (!flag)
    {
        fw = fopen("variables.txt", "a");
        char temp[BUFSIZ];
        strcpy(temp, tempT);
        strcat(temp, " ");
        char resultStr[BUFSIZ];
        sprintf(resultStr, "%d", result);
        strcat(temp, resultStr);
        strcat(temp, "\n");
        fputs(temp, fw);
        fclose(fw);
    }

    fprintf(fplog, "%s: %d\n", tempT, result);
}

