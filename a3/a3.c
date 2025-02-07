#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int dirsearch(void){
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    currDir = opendir(".");

    while((entry = readdir(currDir)) != NULL){
        stat(entry->d_name, &dirStat);
        printf("%s modified at %ld\n", entry->d_name, dirStat.st_size);
    }

    closedir(currDir);
    return 0;
}

void menu(){
    int choice;
    do {
        printf("1. Select file to process\n");
        printf("2. Exit the progrm\n");
        printf("\nEnter a choice 1 or 2: ");
        scanf("%d", &choice);

        if(choice == 1){
            int secondchoice;
            printf("\nWhich file you want to process?\n");
            printf("Enter 1 to pick the largest file\n");
            printf("Enter 2 to pick the smallest file\n");
            printf("Enter 3 to specify the name of a file\n");
            printf("\nEnter a choice from 1 to 3: ");
            scanf("%d", &secondchoice);
            if(secondchoice == 1){
                dirsearch();
            } else if (secondchoice == 2){
                printf("smallest file\n\n");
            } else if (secondchoice == 3){
                printf("file name\n\n");
            } else{
                printf("You entered an incorrect choice. Try again.\n\n");
            }
        }

    } while (choice != 2);
}

int main(void) {

    menu();
    return 0;
}