#include <stdio.h>

// Declare the function from mainFunc.c
int mainFunc(int ch);

int main() {
    int choice;
    printf("<--- OS Project: Context Switching --->\n\n");
    printf("MENU\n1. Implementation with threads\n2. Implementation without threads\n\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    mainFunc(choice);
    return 0;
}

