#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YEAR_MAX 2021
#define YEAR_MIN 1900

//Define movie structure adapted from exploration code
struct movie {
    char *title;
    int year;
    char *languages;
    float rating;
    struct movie *next;
};

/*Creates each movie node within the linked list, code adapted from explorations*/
struct movie* createMovie(char *line) {
    struct movie *newMovie = malloc(sizeof(struct movie));

    //initializing a token
    char *token;

    // Extracting title, code adapted from exploration
    token = strtok(line, ",");
    newMovie->title = malloc(strlen(token) + 1);
    strcpy(newMovie->title, token);

    // Extracting year, code adapted from exploration
    token = strtok(NULL, ",");
    newMovie->year = atoi(token); //atoi converts string to int

    // Extracting languages as one string, code adapted from exploration
    token = strtok(NULL, ",");
    newMovie->languages = malloc(strlen(token) + 1);

    // Extract rating, code adapted from exploration
    token = strtok(NULL, ",");
    newMovie->rating = atof(token); //atof converst string to double, found on the linux Man page from atoi
    
    // Set pointer to next node
    newMovie->next = NULL;
    return newMovie;
}

/*
* Function to process movie file adapted from code provided in assignment
* adapted functionality to create a linked list of the movies using code from explorations
*/

struct movie* processMovieFile(char* filePath) {
    char *currLine = NULL;
    size_t len = 0;
    struct movie *head = NULL;
    struct movie *tail = NULL;
    int movieCount = 0;

    FILE *movieFile = fopen(filePath, "r");

    //Get the header line and skip over it
    getline(&currLine, &len, movieFile);

    //Iterate through each line creating a movie structure and add it to the linked list
    while(getline(&currLine, &len, movieFile) != -1) {
        struct movie *newMovie = createMovie(currLine);
        if(head == NULL) {
            head = newMovie;
            tail = newMovie;
        } else {
            tail->next = newMovie;
            tail = newMovie;
        }
        movieCount++;
    }

    free(currLine);
    fclose(movieFile);
    printf("Processed file %s and parsed data for %d movies\n", filePath, movieCount);
    return head;
}

/*Interactive Functions*/

/*Takes the year indicated by user and iterates through the linked list returning titles if there is data for that year*/
void moviesByYear(struct movie *head, int year){
    struct movie *curr = head;
    int found = 0;
    while (curr){
        if(curr->year == year){
            printf("%s\n", curr->title);
            found = 1;
        }
        curr = curr->next;
    }
    if(!found){
        printf("No data about movies released in the year %d\n", year);
    }
}

/*Displays the highest-rated movie for each year in the list*/
void highestRatedMovies(struct movie *head){
    struct movie *curr = head;
    
    //array length of year max - year min
    struct movie *rateList[122] = {NULL};

    while(curr){ 
        int index = curr->year-1900;
        if(!rateList[index] || curr->rating > rateList[index]->rating) {
            rateList[index] = curr;
        }
        curr = curr->next;
    }

    for(int i = 0; i < 122; i++){
        if(rateList[i]){
            printf("%d %.1f %s\n", rateList[i]->year, rateList[i]->rating, rateList[i]->title);
        }
    }
}

/*Finding a movie based on language*/
void moviesByLanguage(struct movie *head, char *language) {
    struct movie *curr = head;
    int found = 0;

    // make use of strtok_r to parse through the list of strings, code adapated from explorations
    while(curr) {
        char *langcopy = malloc(strlen(curr->languages) + 1);
        strcpy(langcopy, curr->languages);

        char *savePtr;
        char *token = strtok_r(langcopy + 1, ";]", &savePtr);

        while(token){
            if(strcmp(token, language) == 0){
                printf("%d %s\n", curr->year, curr->title);
                found = 1;
                break;
            }
            token = strtok_r(NULL, ";]", &savePtr);
        }

        free(langcopy);
        curr = curr->next;
    }

    if(!found) {
        printf("No data about movies released in %s\n", language);
    }
}

/*Freeing up memory allocated for linked list*/
void freeMovies(struct movie *head) {
    struct movie *temp;
    while(head) {
        temp = head;
        head = head->next;
        free(temp->title);
        free(temp->languages);
        free(temp);
    }
}

/*options to be presented to user*/
void menu(struct movie *head) {
    int choice;
    do {
        printf("\n1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n");
        printf("\nEnter a choice from 1 to 4: ");
        scanf("%d", &choice);

        if(choice == 1) {
            int year;
            printf("Enter the year for which you want to see movies: ");
            scanf("%d", &year);
            moviesByYear(head, year);
        } else if(choice == 2) {
            highestRatedMovies(head);
        } else if(choice ==3) {
            char language[21];
            printf("Enter the language for which you want to see movies: ");
            scanf("%s", language);
            moviesByLanguage(head, language);
        }
    } while (choice != 4);
}

/*Main function call adapted from assignment code example*/
int main(int argc, char **argv) {
    if(argc < 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example: ./movies movie_list.csv\n");
        return EXIT_FAILURE;
    }
    
    struct movie *head = processMovieFile(argv[1]);
    menu(head);
    freeMovies(head);
    return EXIT_SUCCESS;
}
