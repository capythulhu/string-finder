// Header guards.
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#define MAX_LINE_LENGTH 1<<9

// Struct containing the file pointer and size.
typedef struct _file{
    FILE    *fp;
    int     size;
} file;

// Gets file from path and calculate it's size.
file get_file(char *path){
    file f;
    char buffer[MAX_LINE_LENGTH];
    f.fp = fopen(path, "r");
    f.size = 0;
    while(fgets(buffer, MAX_LINE_LENGTH, f.fp) != NULL) f.size++;
    rewind(f.fp);
    return f;
}
