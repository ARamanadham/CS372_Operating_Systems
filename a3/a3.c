#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

#define MAX_LANGS 5
#define MAX_LEN 20

struct movie{
    char *title;
    int year;
    char languages[MAX_LANGS][MAX_LEN];
    double rating;
    struct movie *next;
};

struct movie* createMovie(char* title, int year, char* languages, double rating){
    struct movie* newMovie = malloc(sizeof(struct movie));

    newMovie->title = calloc(strlen(title) + 1, sizeof(char));
    strcpy(newMovie->title, title);

    newMovie->year = year;

    int langIdx = 0;
    char* token = strtok(languages, "[];");
    while(token !=NULL && langIdx < MAX_LANGS){
        strncpy(newMovie->languages[langIdx], token, MAX_LEN-1);
        newMovie->languages[langIdx][MAX_LEN-1] = '\0';
        langIdx++;
        token = strtok(NULL, "[];");
    }

    newMovie->rating = rating;

    newMovie->next = NULL;
    return newMovie;
}

//moved the linked list logic to seperate function
void appendMovie(struct movie** head, struct movie* newMovie){
    if (*head == NULL){
        *head = newMovie;
    } else {
        struct movie* temp = *head;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newMovie;
    }
}


void moviesByYear(struct movie *head, const char *outputDir){
    struct movie *curr = head;

    while(curr){
        char *outputFilePath = malloc(strlen(outputDir) + 10);
        sprintf(outputFilePath, "%s/%d.txt", outputDir, curr->year);

        int fd = open(outputFilePath, O_WRONLY | O_CREAT| O_APPEND, 0640);
        if(fd == -1){
            printf("Error: couln't open file %s\n", outputFilePath);
            perror("Error");
            free(outputFilePath);
            curr = curr->next;
            continue;
        }

        write(fd, curr->title, strlen(curr->title));
        write(fd, "\n", 1);

        close(fd);

        free(outputFilePath);
        curr = curr->next;
    }
}

void freeMovie(struct movie* head){
    while (head != NULL){
        struct movie* temp = head;
        head = head->next;
        free(temp->title);
        free(temp);
    }
}

// code adapted from exploration: directories
char* largestFile(void){
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    size_t maxSize = 0;
    char *largestFile = NULL, *tempFile = NULL;
    currDir = opendir(".");
    
    // checking that the file is a regular file that is prefixed with "movies_" and is a .csv file
    while((entry = readdir(currDir)) != NULL){
        if(stat(entry->d_name, &dirStat) == 0 && S_ISREG(dirStat.st_mode)){
            if(strncmp(entry->d_name, "movies_", 7) == 0){
                //strstr found in man pages 
                if(strstr(entry->d_name, ".csv") != NULL){
                    if(dirStat.st_size > maxSize){
                        maxSize =  dirStat.st_size;
                        tempFile = strdup(entry->d_name);
                        if (tempFile){
                            free(largestFile);
                            largestFile = tempFile;
                        }
                    }
                }
            }
        }
    }

    closedir(currDir);
    return largestFile;
}

// code adapted from exploration: directories
char* smallestFile(void){
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    //Size_MAX found in man page for size_t
    size_t minSize = SIZE_MAX;
    char *smallestFile = NULL, *tempFile = NULL;
    //int firstFile = 0; // Flag to track first file to have initial minimum size

    currDir = opendir(".");
    while((entry = readdir(currDir)) != NULL){
        if(stat(entry->d_name, &dirStat) == 0 && S_ISREG(dirStat.st_mode)){
            if(strncmp(entry->d_name, "movies_", 7) == 0){
                if(strstr(entry->d_name, ".csv") != NULL){
                    if (dirStat.st_size < minSize) {
                        minSize = dirStat.st_size;
                        tempFile = strdup(entry->d_name);
                        if(tempFile){
                            free(smallestFile);
                            smallestFile = tempFile;
                        }
                    }                    
                }
            }
        }
    }

    closedir(currDir);
    return smallestFile;
}

//process file adapated from exploration code
void processFile(char* inputFilePath){
    //char *currLine = NULL;
    //size_t len = 0;

    srandom(time(NULL));
    int randNum = random() % 100000;

    char *outputDir = malloc(22); //ramanada.moves. (16 chars) + 5 (random num) + null terminator

    printf("\nNow processing the chosen file named: %s\n", inputFilePath);
    sprintf(outputDir, "ramanada.movies.%d", randNum);

    if(mkdir(outputDir, 0750) == 0){
        printf("Created directory with name %s\n", outputDir);
    } else{
        perror("Error creating directory");
        free(outputDir);
        return;
    }

    FILE *inputFile = fopen(inputFilePath, "r");
    if(inputFile == NULL){
        printf("Error: Could not open file %s\n", inputFilePath);
        return;
    }

    char buffer[1024];
    struct movie* movieList = NULL;

    fgets(buffer, sizeof(buffer), inputFile);

    while(fgets(buffer, sizeof(buffer), inputFile) != NULL){
        char* title;
        char* yearStr;
        char* langs;
        char* ratingStr;

        title = strtok(buffer, ",");
        yearStr = strtok(NULL, ",");
        langs = strtok(NULL, ",");
        ratingStr = strtok(NULL, ",");

        int year = atoi(yearStr);
        double rating = atof(ratingStr);
        struct movie* newMovie = createMovie(title, year, langs, rating);
        appendMovie(&movieList, newMovie);
    }
    moviesByYear(movieList, outputDir);
    fclose(inputFile);
    //printMovie(movieList);
    freeMovie(movieList);
    free(outputDir);
}
//Adapted from my assignment 2 menu code
void menu(){
    int choice;
    do{
        printf("1. Select file to process\n");
        printf("2. Exit the progrm\n");
        printf("\nEnter a choice 1 or 2: ");
        scanf("%d", &choice);

        if (choice != 1 && choice != 2){
            printf("You entered an incorrect choice. Try again.\n\n");
        }
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
                    char *largest = largestFile();
                    //printf("Now processing the chosen file named %s\n\n", largest);
                    processFile(largest);
                    break;
                } else if (secondchoice == 2){
                    char *smallest = smallestFile();
                    //printf("Now processing the chosen file named %s\n\n", smallest);
                    processFile(smallest);
                    break;
                } else if (secondchoice == 3){
                    char fname;
                    struct stat dirStat;

                    printf("Enter the complete file name: ");
                    scanf("%s", &fname);

                    if(stat(&fname, &dirStat) == 0){
                        processFile(&fname);
                        //printf("Processing file: %s\n\n", &fname);
                        break;
                    } else{
                        printf("The file %s was not found. Try again\n", &fname);
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