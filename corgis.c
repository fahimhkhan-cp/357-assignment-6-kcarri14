#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "corgis.h"

int load_data(char *filename, Info **info){
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("Error: can't open file");
        return -1;
    }
    char line[1024];
    int line_num = 0;
    int count = 0;
    *info = malloc(100 * sizeof(Info));
    while(fgets(line, sizeof(line), file)){
        line_num++;
        if(line_num == 1){
            continue;
        }
        Info information;
        if(parsing(line, &information) == -1){
            fprintf(stderr, "Error: line entry is malformed %d", line_num);
            continue;
        }
        (*info)[count++] = information;

    }
    fclose(file);
    return count;
}

void display_counties(Info *information, int count){
    for(int i = 0; i < count; i++){
        printf("County name: %s\nCounty State: %s\nPopluation: %d\n", information[i].county, information[i].state, information[i].Population_2014_Population);
    }
}

void filter_state(Info *information, int *count, char *state){
    int new_count = 0;
    for(int i = 0; i < *count; i++){
        if(strcmp(information[i].state, state) ==0){
            information[new_count++] = information[i];
        }
    }
    *count = new_count;
    printf("Filter: state == %s (%d entries)\n", state, new_count);
}
void filter_field(Info *information, int *count, char *field, char *comparison, float value){
    int new_count = 0;
    for(int i = 0; i < *count; i++){
        float field_value;
        if (strcmp(field, "Education.High School or Higher") == 0) {
            field_value = information[i].Education_Bachelors_Degree_or_Higher;
        } else if (strcmp(field, "Education.Bachelors or Higher") == 0) {
            field_value = information[i].Education_High_School_or_Higher;
        } else if (strcmp(field, "Income.Persons Below Poverty Level") == 0) {
            field_value = information[i].Income_Persons_Below_Poverty_Level;
        } else if (strcmp(field, "Ethnicities.American Indian and Alaska Native Alone") == 0) {
            field_value = information[i].Ethnicities_American_Indian_and_Alaska_Native_Alone;
        } else if (strcmp(field, "Ethnicities.Asian Alone") == 0) {
            field_value = information[i].Ethnicities_Asian_Alone;
        } else if (strcmp(field, "Ethnicities.Black Alone") == 0) {
            field_value = information[i].Ethnicities_Black_Alone;
        } else if (strcmp(field, "Ethnicities.Hispanic or Latino") == 0) {
            field_value = information[i].Ethnicities_Hispanic_or_Latino;
        } else if (strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
            field_value = information[i].Ethnicities_Native_Hawaiian_and_Other_Pacific_Islander_Alone;
        } else if (strcmp(field, "Ethnicities.Two or More Races") == 0) {
            field_value = information[i].Ethnicities_Two_or_More_Races;
        } else if (strcmp(field, "Ethnicities.White Alone") == 0) {
            field_value = information[i].Ethnicities_White_Alone;
        } else if (strcmp(field, "Ethnicities.White Alone not Hispanic or Latino") == 0) {
            field_value = information[i].Ethnicities_White_Alone_not_Hispanic_or_Latino;  
        }else {
            fprintf(stderr, "Error: Unsupported field '%s'\n", field);
            return;
        }

        if(strcmp(comparison, "ge") == 0){
            if(field_value >= value){
                information[new_count++] = information[i];
            }else{
                continue;
            }

        }else if(strcmp(comparison, "le") == 0){
            if(field_value <= value){
                information[new_count++] = information[i];
            }else{
                continue;
            }
        }
    }

    *count = new_count;
    printf("Filter: %s, %s, %.2f (%d entries)", field, comparison, value, new_count);

}
void population_total(Info *information, int count){
    int population_total_number = 0;
    for(int i = 0; i < count; i++){
        population_total_number += information[i].Population_2014_Population;
    }
    printf("2014 population: %d\n", population_total_number);
}
void population_field(Info *information, int *count, char *field){

}
void percent(Info *information, int *count, char *field){

}

int execute_operation(char *line, Info *information, int count){
    char *function_wanted = strtok(line, ":\n");
    if(strcmp(function_wanted,"population-total")){
        population_total(information, count);
    }else if(strcmp(function_wanted, "filter-state")){
        char *state_wanted = strtok(NULL, "\n");
        filter_state(information, count, state_wanted);
    }else if(strcmp(function_wanted, "filter")){
        char *field_wanted = strtok(NULL, ":");
        char *ge_or_le = strtok(NULL, ":");
        float number = strok(NULL,"\n");
        filter_field(information, count, field_wanted, ge_or_le, number);
    }else if(strcmp(function_wanted, "percent")){
        char *field_wanted = strtok(NULL, "\n");
        percent(information, count, field_wanted);
    }else if(strcmp(function_wanted, "population")){
        char *field_wanted = strtok(NULL, "\n");
        population_field(information, count, field_wanted);
    }else if(strcmp(function_wanted, "display")){
        display_counties(information, count);
    }else{
        fprintf(stderr, "malformed line");
        return -1;
    }
    return 0;
}


int process_operations(char *filename, Info *information, int count){
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("Error opening file");
        return -1;
    }
    char line[256];
    while(fgets(line, sizeof(line), file)){
        if(strlen(line) == 0 || line[0] == '\n'){
            continue;
        }
        if(execute_operation(line, information, count) == -1){
            fprintf(stderr, "Error: malformed operation: %s\n", line);
            continue;
        }
    }
    fclose(file);
    return 0;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Error: not enough arguments");
        return 1;
    }
    Info *information = NULL;
    int count = load_data(argv[1], &information);

    if(count == -1){
        perror("Error: fail to load data");
        return 1;
    }
    printf("Loaded %d entries.\n", count);

    if(process_operations(argv[2], information, count)){
        perror("Error: failed to process operations");
        free(information);
        return 1;
    }
    free(information);
    return 0;

}