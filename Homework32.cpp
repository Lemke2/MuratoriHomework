#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char* label;
    char* value;
    Node* sibling;
    Node* child;
};

Node* newNode() {
    Node* n = (Node*)malloc(sizeof(Node));
    n->label = NULL;
    n->value = NULL;
    n->child = NULL;
    n->sibling = NULL;
    return n;
}

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

char* ParseString(char* buffer, int* counter)
{
    while(buffer[*counter] != '"')
    {
        (*counter)++;
    }
    (*counter)++;

    int tempcounter = 0;
    while (buffer[*counter + tempcounter] != '"')
    {
        tempcounter++;
    }
    
    char* ret = (char*)malloc(tempcounter + 1);
    strncpy(ret, buffer + *counter, tempcounter);
    ret[tempcounter] = '\0';
    *counter = *counter + tempcounter;

    return ret;
}

char* ParseNumber(char* buffer, int* counter)
{
    int tempcounter = 0;
    while(buffer[*counter + tempcounter] != ',' && buffer[*counter + tempcounter] != '}')
    {
        tempcounter++;
    }

    char* ret = (char*)malloc(tempcounter + 1);
    strncpy(ret, buffer + *counter, tempcounter);
    ret[tempcounter] = '\0';
    *counter = *counter + tempcounter;

    return ret;
}

Node* parseObject(char* buffer, int* counter)
{
    Node* n = newNode();
    n->label = ParseString(buffer, counter);

    return n;
}

void PrintJson(Node* root) {
    if (root == NULL) return;  // Base case to prevent segfault

    printf("%s:", root->label);
    printf("%s", root->value);

    if (root->child != NULL) {
        printf("{");
        PrintJson(root->child);
        printf("}");
    }

    if (root->sibling != NULL) {
        printf(",");
        PrintJson(root->sibling);
    }
}

int main()
{   
    const char* filename = "filename.txt";
    int filesize = 0;
    char* buffer = LoadMemoryFromFile(filename, &filesize);
    
    printf("The file has been loaded in to memory and the filesize is: %d", filesize);
    printf("\n");

    int counter = 0;

    Node* root = newNode();
    Node* curr = root;
    Node* stack[256];
    int stackIndex = 0;

    while (counter < filesize)
    {
        char c = buffer[counter];

        switch (c)
        {

        case '{':
            stack[stackIndex++] = curr;
            curr = curr -> child = parseObject(buffer, &counter);
            //skip closing " for label
            counter++;
            if (buffer[counter] != ':')
            {
                printf("FORMAT ERROR, EXPECTED :");
                return 0;
            }
            break;
        
        case ',':
            curr = curr -> sibling = parseObject(buffer, &counter);
            //skip closing " for label
            counter++;
            if (buffer[counter] != ':')
            {
                printf("FORMAT ERROR, EXPECTED :");
                return 0;
            }
            break;
        
        case '"':
            curr -> value = ParseString(buffer, &counter);
            if (buffer[counter] == '}')
            {
                counter--;
            }
            break;

        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            curr -> value = ParseNumber(buffer, &counter);
            if (buffer[counter] == '}')
            {
                counter--;
            }
            break;

        case '}':
            if (stackIndex > 0) curr = stack[--stackIndex];
            break;

        default:
            break;
        }
        counter++;
    }

    PrintJson(root->child);
    return 0;
}