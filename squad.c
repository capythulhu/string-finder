/**
 * Dictionary by:
 * George A. Miller (1995). WordNet: A Lexical Database for English.
 * Communications of the ACM Vol. 38, No. 11: 39-41.

 * Christiane Fellbaum (1998, ed.) WordNet: An Electronic Lexical Database. Cambridge, MA: MIT Press.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"

#define MAX_LINE_LENGTH 1<<9

// Struct containing the file pointer and size.
typedef struct _file{
    FILE *fp;
    int size;
} file;

// Get file from path and calculate it's size.
file getFile(char *path){
    file f;
    char buffer[MAX_LINE_LENGTH];
    f.fp = fopen(path, "r");
    f.size = 0;
    while(fgets(buffer, MAX_LINE_LENGTH, f.fp) != NULL) f.size++;
    rewind(f.fp);
    return f;
}

// Remove the character '\n' from the end of string
void rem_new_line(char *s){
    int i = 0;
    while(s[i] != '\n') i++;
    s[i] = '\0';
}

// Get random word from file, and writes it into buffer
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

    // Opens target file and writes initial phrase.
    FILE *out = fopen("out.txt", "w");
    fputs("You're really gonna mess with my squad? We have", out);

    // Opens dictionary files.
    file sup = getFile("sup.txt");
    file adj = getFile("adj.txt");
    file noun = getFile("noun.txt");
    file comp = getFile("comp.txt");
    
    // Get number of threads and desired iterations.
    int j = atoi(argv[1]), k = atoi(argv[2]);
    // Allocate threads.
    omp_set_num_threads(k);

    #pragma omp parallel
    {
        // Allocate buffers for each word.
        char outBuffer[MAX_LINE_LENGTH];

        char supBuffer[MAX_LINE_LENGTH];
        char adjBuffer[MAX_LINE_LENGTH];
        char nounBuffer[MAX_LINE_LENGTH];
        char compBuffer[MAX_LINE_LENGTH];

        int i, l;
        for(i = 0; i < j; i++){
            l = 0;
            // Generate words and fill file.
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