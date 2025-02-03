#include <stdio.h>
#include <stdlib.h>

struct Tree
{
    char* label;
    Tree* next;
    char* str_value;
    int int_value;
    double double_value;
};

static char* LoadMemoryFromFile(const char* filename, int* filesize)
{
    FILE* file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(*filesize);
    fread(buffer, 1, *filesize, file);

    fclose(file);
    return buffer;
}

int main()
{   
    const char* filename = "filename.txt";
    int filesize = 0;
    char* buffer = LoadMemoryFromFile(filename, &filesize);

    printf("The filesize: %d", filesize);
    return 0;
}