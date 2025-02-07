#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu(){
    int choice;
    do {
        printf("1. Select file to process\n");
        printf("2. Exit the progrm\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%d", &choice);

        if(choice == 1){
            int secondchoice;
            printf("Which file you want to process?\n");
            printf("Enter 1 to pick the largest file\n");
            printf("Enter 2 to pick the smallest file\n");
            printf("Enter 3 to specify the name of a file\n");
            printf("Enter a choice from 1 to 3: ");
            scanf("%d", &secondchoice);
            if(secondchoice == 1){
                printf("Largest file\n");
            } else if (secondchoice == 2){
                printf("smallest file\n");
            } else if (secondchoice == 3){
                printf("file name\n");
            } else{
                printf("You entered an incorrect choice. Try again.\n");
            }
        }

    } while (choice != 2);
}

int main(void) {

    menu();
    return 0;
}