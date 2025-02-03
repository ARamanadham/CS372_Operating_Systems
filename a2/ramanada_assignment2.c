#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Define movie structure adapted from exploration code
struct movie {
    const *title;
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

/*Main function call adapted from assignment code example*/
int main(int argc, char **argv) {
    if(argc < 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example: ./movies movie_list.csv\n");
        return EXIT_FAILURE;
    }
    
    struct movie *head = processMovieFile(argv[1]);
    freeMovies(head);
    return EXIT_SUCCESS;
}
