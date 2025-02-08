#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

// code adapted from exploration: directories
int largestFile(void){
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    size_t maxSize = 0;
    char *largestFile;
    currDir = opendir(".");
    
    // checking that the file is a regular file that is prefixed with "movies_" and is a .csv file
    while((entry = readdir(currDir)) != NULL){
        if(stat(entry->d_name, &dirStat) == 0 && S_ISREG(dirStat.st_mode)){
            if(strncmp(entry->d_name, "movies_", 7) == 0){
                //strstr found in man pages 
                if(strstr(entry->d_name, ".csv") != NULL){
                    if(dirStat.st_size > maxSize){
                        maxSize =  dirStat.st_size;
                        largestFile = entry->d_name;
                    }
                }
            }
        }
    }

    printf("Now processing the chosen file named %s\n", largestFile);

    closedir(currDir);
    return 0;
}

// code adapted from exploration: directories
int smallestFile(void){
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    //Size_MAX found in man page for size_t
    size_t minSize = 0;
    char *smallestFile;
    int firstFile = 0; // Flag to track first file to have initial minimum size

    currDir = opendir(".");
    while((entry = readdir(currDir)) != NULL){
        if(stat(entry->d_name, &dirStat) == 0 && S_ISREG(dirStat.st_mode)){
            if(strncmp(entry->d_name, "movies_", 7) == 0){
                if(strstr(entry->d_name, ".csv") != NULL){
                    if(!firstFile){
                        minSize = dirStat.st_size;
                        smallestFile = entry->d_name;
                        firstFile = 1; //update flag so this doesn't trigger again
                    } else if (dirStat.st_size < minSize) {
                        minSize = dirStat.st_size;
                        smallestFile = entry->d_name;
                    }                    
                }
            }
        }
    }

    printf("Now processing the chosen file named %s\n", smallestFile);
    closedir(currDir);
    return 0;
}

//Adapted from my assignment 2 menu code
void menu(){
    int choice;
    do {
        printf("1. Select file to process\n");
        printf("2. Exit the progrm\n");
        printf("\nEnter a choice 1 or 2: ");
        scanf("%d", &choice);
        // creating a submenu
        if(choice == 1){
            int secondchoice;
            while(1){
                printf("\nWhich file you want to process?\n");
                printf("Enter 1 to pick the largest file\n");
                printf("Enter 2 to pick the smallest file\n");
                printf("Enter 3 to specify the name of a file\n");
                printf("\nEnter a choice from 1 to 3: ");
                scanf("%d", &secondchoice);
                if(secondchoice == 1){
                    largestFile();
                    break;
                } else if (secondchoice == 2){
                    smallestFile();
                    break;
                } else if (secondchoice == 3){
                    char fname;
                    struct stat dirStat;

                    printf("Enter the complete file name: ");
                    scanf("%s", &fname);

                    if(stat(&fname, &dirStat) == 0){
                        printf("Processing file: %s\n\n", &fname);
                        break;
                    } else {
                        printf("The file %s was not found. Try again", &fname);
                    }
                } else{
                    printf("You entered an incorrect choice. Try again.\n\n");
                }
            }
        }

    } while (choice != 2);
}

int main(void) {

    menu();
    return 0;
}