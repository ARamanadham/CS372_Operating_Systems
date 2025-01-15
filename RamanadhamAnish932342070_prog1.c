#include <stdio.h>
#include <math.h>

// Macro definition for squares
#define square(x) x*x 

// Const value for PI
const float PI = 3.14159265359;

// Global variables
int segments; // number of segments the user will provide the dimensions for
float total_surface_area; //total surface area for every segment
float total_volume; // total volume for every for every segment
float avg_surface_area; // average surface area for all segments
float avg_volume; //average volume for all segments

// function to check that all user inputs for dimensions are positive real values
int num_check(float number){
    if(number < 0){
        return 1;
    }
    return 0;
}

// calculating the a/b value needed for surface area and volume calculations
float get_area_val(float radius, float height){
    float val;
    val = sqrt(square(radius) - square(height));
    return val;
}

// function for each segments individual surface area
float get_surface_area(float radius, float ha, float hb){
    float total_area; // total surface area
    float top_area; // area of the top segment using radius and ha
    float bottom_area; // area of the bottom segment using radius and hb
    float lateral_area; // area of the middle segment
    float height; // variable of ha-hb, used to avoid issues with the square Macro

    height = ha - hb;
    top_area = PI * square(get_area_val(radius, ha));
    bottom_area = PI * square(get_area_val(radius, hb));
    lateral_area = 2 * PI * radius * height;
    total_area = top_area + bottom_area + lateral_area;
    return total_area;
}

// function to get the volume of each individual segment
float get_volume(float radius, float ha, float hb){
    float volume; //total volume of the segment
    float height;

    height = ha - hb;
    volume = (PI * height * ((3 * square(get_area_val(radius, ha))) + (3 * square(get_area_val(radius, hb))) + square(height)))/6;
    return volume;
}

/*
    Main program responsibilities: 
    Gather user input for the number of segments we will calculate (2-10 inclusive)
    Gather radius, top and bottom heights from the user, ensuring they are all positive numbers and that radius >= top height >= bottom height
    Calculate each individuals segments surface area + volume
    Calculate the total and average surface area + volume
*/
void main(){
    float radius; // radius of each segment
    float top_height; // height of the top segment
    float bottom_height; // height of the bottom segment
    float surface_area; // surface_area of each segment
    float volume; // volume of each segment

    // using a do-while loop to determine the number of segements the user wishes to calculate the surface area/volume for
    do {
        printf("How many spherical segments you want to evaluate [2-10]?\n");
        scanf("%d", &segments);
    } while (segments < 2 || segments > 10);

    // loop through each individual segment, gathering dimensions from the user and calculating the surface area and volume
    for(int i = 1; i <= segments; i++){
        printf("Obtaining data for spherical segment number %d\n", i);
        printf("What is the radius of the sphere (R)?\n");
        scanf("%f", &radius);
        printf("what is the height of the top area of the spherical segment (ha)?\n");
        scanf("%f", &top_height);
        printf("what is the height of the bottom area of the spherical segment (hb)?\n");
        scanf("%f", &bottom_height);
        printf("Entered Data: R = %.2f ha = %.2f hb = %.2f.\n", radius, top_height, bottom_height);

        // ensuring numbers are all positive
        if(num_check(radius) == 1){
            printf("Invalid Input: R = %.2f. R must be a positive real number.\n", radius);
            // decrement the segment count since the input is invalid
            --i;
        }

        if(num_check(top_height) == 1){
            printf("Invalid Input: ha = %.2f. ha must be a positive real number.\n", top_height);
            --i;
        }

        if(num_check(bottom_height) == 1){
            printf("Invalid Input: hb = %.2f. hb must be a positive real number.\n", bottom_height);
            --i;
        }

        // ensuring radius >= ha >= hb
        if(radius >= top_height && radius >= bottom_height){
            if(bottom_height > top_height){
                printf("Invalid Input: ha = %.2f hb = %.2f. ha must be greater than or equal to hb.\n", top_height, bottom_height);
                --i;
                //moving to the next iteration of the loop
                continue;
            }
            surface_area = get_surface_area(radius, top_height, bottom_height);
            volume = get_volume(radius, top_height, bottom_height);
            printf("Total Surface Area = %.2f Volume = %.2f. \n", surface_area, volume);
            total_surface_area += surface_area;
            total_volume += volume;
        } else{
            if(radius < top_height){
                printf("Invalid Input: R = %.2f ha = %.2f. R must be greater than or equal to ha.\n", radius, top_height);
                --i;
                continue;
            }
            printf("Invalid Input: R = %.2f hb = %.2f. R must be greater than or equal to hb.\n", radius, bottom_height);
            --i;
        }
    }
    avg_surface_area = total_surface_area/segments;
    avg_volume = total_volume/segments;
    printf("Total average results:\n");
    printf("Average Surface Area = %.2f Average Volume = %.2f.", avg_surface_area, avg_volume);
}