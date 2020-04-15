// Header guards.
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>
#include "file.h"
#define MAX_BUFFER_LENGTH 2

int main(int argc, char **argv){
    // Thread count.
    int t = atoi(argv[1]);
    // Get file from filename.
    file f = get_file(argv[2]);

    char buffer[MAX_BUFFER_LENGTH + 1];
    
    int j = 0, s = 0;
    while(fgets(buffer, sizeof buffer, f.fp) != NULL){
        int i;
        for(i = 0; i < sizeof buffer; i++){
            if(buffer[i] == '\0') continue;
            if(buffer[i] == argv[3][j]){
                if(j == strlen(argv[3]) - 1) s++;
                else j++;
            } else j = 0;
        }
    }
    fclose(f.fp);

    printf("Count: %i", s);
}