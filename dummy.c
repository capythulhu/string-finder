#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "omp.h"

#define MAX_LINE_LENGTH 1<<8
#define MAX_BUFFER_ITERACTIONS 10

// Dictionary structure
typedef struct _dictionary{
    char    **array;
    int    size;
} dictionary;

// Gets a dictionary from file and loads to RAM.
dictionary get_dictionary(char *path){
    dictionary out;
    FILE *f = fopen(path, "r");
    char buffer[MAX_LINE_LENGTH];
    
    out.size = 0;
    while(fgets(buffer, MAX_LINE_LENGTH, f) != NULL)
        out.size++;
    rewind(f);
    out.array = malloc(out.size * sizeof(char *));

    int i = 0;
    while(fgets(buffer, MAX_LINE_LENGTH, f) != NULL){
        out.array[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
        buffer[strlen(buffer) - 1] = '\0';
        strcpy(out.array[i], buffer);
        i++;
    }
        
    fclose(f);
    return out;
}

// Frees a dictionary.
void free_dictionary(dictionary d){
    free(d.array);
}

// Gets a random word from file, and writes it into buffer.
char *get_random_word(dictionary d){
    return d.array[rand() % d.size];
}

int main(int argc, char **argv) {
    // Random seed.
    srand(time(NULL));
    // Thread count.
    int t = atoi(argv[1]);
    // Iteration count.
    long j = atol(argv[3]);
    
    // Opens dictionary files.
    dictionary sup = get_dictionary("dictionary/sup.txt");
    dictionary adj = get_dictionary("dictionary/adj.txt");
    dictionary noun = get_dictionary("dictionary/noun.txt");
    dictionary comp = get_dictionary("dictionary/comp.txt");
    
    // Opens file from filename and writes initial phrase.
    FILE *f = fopen(argv[2], "w");
    fputs("Do you really think you can win? We have", f);

    // Allocates threads.
    omp_set_num_threads(t);

    double time = omp_get_wtime();
    #pragma omp parallel
    {
        // Allocates buffers for each word.
        char *outBuffer = malloc((MAX_LINE_LENGTH) * MAX_BUFFER_ITERACTIONS);
        
        // Opens output file again.
        FILE *out = fopen(argv[2], "w");

        int i, k;
        #pragma omp for
        for(i = 0; i < j; i++){
            strcpy(outBuffer, "\0");
            for(k = 0; k < MAX_BUFFER_ITERACTIONS; k++){
                // Generates words and fill file.
                sprintf(outBuffer, " the %s %s %s that's way %s", get_random_word(sup), 
                    get_random_word(adj), get_random_word(noun), get_random_word(comp));
                sprintf(outBuffer, "%s than your %s %s %s", outBuffer, get_random_word(sup),
                    get_random_word(adj), get_random_word(noun));
                sprintf(outBuffer, "%s,", outBuffer);
                fputs(outBuffer, out);
            }
        }
        // Closes files.
        fclose(out);
    }
    time = omp_get_wtime() - time;
    
    // Frees dictionaries.
    free_dictionary(sup);
    free_dictionary(adj);
    free_dictionary(noun);
    free_dictionary(comp);
    fclose(f);

    printf("Iteractions: %li, Time: %lf", j, time);
}