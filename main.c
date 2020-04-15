// Header guards.
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "omp.h"
#define MAX_BUFFER_LENGTH 2

typedef struct _search{
    unsigned long   occurrences;
    double          time;
} search;

search search_concurrent(FILE *f, char *word){
    search output = {0, 0};
    int j = 0;
    output.time = omp_get_wtime();
    char buffer[MAX_BUFFER_LENGTH + 1];
    while(fgets(buffer, sizeof buffer, f) != NULL){
        int i;
        for(i = 0; i < sizeof buffer; i++){
            if(buffer[i] == '\0') continue;
            if(buffer[i] == word[j]){
                if(j == strlen(word) - 1) output.occurrences++;
                else j++;
            } else j = 0;
        }
    }
    output.time = omp_get_wtime() - output.time;
    return output;
}

int main(int argc, char **argv){
    // Thread count.
    int t = atoi(argv[1]);
    // Get file from filename.
    file f = get_file(argv[2]);
    
    search s = search_concurrent(f.fp, argv[3]);

    printf("Count: %li, Time: %lf", s.occurrences, s.time);
    fclose(f.fp);
}