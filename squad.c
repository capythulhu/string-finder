// Header guards.
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#include <stdlib.h>
#include <time.h>
#include "file.h"
#include "omp.h"

// Removes the character '\n' from the end of string.
void rem_new_line(char *s){
    int i = 0;
    while(s[i] != '\n') i++;
    s[i] = '\0';
}

// Gets a random word from file, and writes it into buffer.
int get_random_word(file f, char *buffer){
    int line = rand() % f.size, i = 0;

    while(fgets(buffer, MAX_LINE_LENGTH, f.fp) != NULL){
        if(line == i){
            rewind(f.fp);
            rem_new_line(buffer);
            return 1;
        }
        i++;
    }
    rewind(f.fp);
    return 0;
}

int main(int argc, char **argv) {
    // Random seed.
    srand(time(NULL));
    
    // Thread count.
    int t = atoi(argv[1]);
    // Iteration count.
    int j = atoi(argv[3]);
    // Opens file from filename and writes initial phrase.
    FILE *out = fopen(argv[2], "w");
    fputs("You're really gonna mess with my squad? We have", out);
    
    // Opens dictionary files.
    file sup = get_file("dictionary/sup.txt");
    file adj = get_file("dictionary/adj.txt");
    file noun = get_file("dictionary/noun.txt");
    file comp = get_file("dictionary/comp.txt");
    
    // Allocates threads.
    omp_set_num_threads(t);

    #pragma omp parallel
    {
        // Allocates buffers for each word.
        char outBuffer[MAX_LINE_LENGTH];

        char supBuffer[MAX_LINE_LENGTH];
        char adjBuffer[MAX_LINE_LENGTH];
        char nounBuffer[MAX_LINE_LENGTH];
        char compBuffer[MAX_LINE_LENGTH];
    
        int i, l;
        for(i = 0; i < j; i++){
            l = 0;
            // Generates words and fill file.
            get_random_word(sup, supBuffer);
            get_random_word(adj, adjBuffer);
            get_random_word(noun, nounBuffer);
            get_random_word(comp, compBuffer);
            sprintf(outBuffer, " the %s %s %s that's way %s", supBuffer, adjBuffer, nounBuffer, compBuffer);
            get_random_word(sup, supBuffer);
            get_random_word(adj, adjBuffer);
            get_random_word(noun, nounBuffer);
            sprintf(outBuffer, "%s than your %s %s %s", outBuffer, supBuffer, adjBuffer, nounBuffer);
            rem_new_line(outBuffer);
            sprintf(outBuffer, "%s,", outBuffer);
            fputs(outBuffer, out);
        }
    }
    
    // Closes files.
    fclose(out);
    fclose(noun.fp);
    fclose(comp.fp);
    fclose(adj.fp);
    fclose(sup.fp);
}