#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char* label;
    char* value;
    Node* sibling;
    Node* child;
    bool isArray;
    bool isArrayElement;
};

Node* newNode() {
    Node* n = (Node*)malloc(sizeof(Node));
    n->label = NULL;
    n->value = NULL;
    n->child = NULL;
    n->sibling = NULL;
    n->isArray = false;
    n->isArrayElement = false;
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
            if (curr -> isArray)
            {   
                Node* arrayElement = newNode();
                arrayElement->isArrayElement = true;
                curr->child = arrayElement;
                curr = curr->child;
            }

            else if (curr -> isArrayElement)
            {
                Node* arrayElement = newNode();
                arrayElement->isArrayElement = true;
                curr->sibling = arrayElement;
                curr = curr->sibling;
            }
            
            stack[stackIndex] = curr;
            stackIndex++;
            curr -> child = parseObject(buffer, &counter);
            curr = curr -> child;
            //skip closing " for label
            counter++;
            if (buffer[counter] != ':')
            {
                printf("FORMAT ERROR, EXPECTED :");
                return 0;
            }
            break;
        
        case ',':
            if (curr -> isArrayElement) break;

            curr -> sibling = parseObject(buffer, &counter);
            curr = curr -> sibling;
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

        case '[':
        {
            Node* Array = newNode();
            Array->isArray = true;

            if (curr->child == NULL) curr->child = Array;
            else
            {
                curr->sibling = Array;
                curr = curr->sibling;
            }

            stack[stackIndex] = curr;
            stackIndex++;
            curr = Array;
            break;
        }

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
            if (buffer[counter] == '}' || buffer[counter] == ',')
            {
                counter--;
            }
            break;

        case ']':
        case '}':
            if (stackIndex > 0) curr = stack[--stackIndex];
            break;

        default:
            break;
        }
        counter++;
    }


    // Haversine goal: 7937.655392

    Node* test = root->child->child->child;

    while (test != NULL)
    {
        Node* innerCurr = test->child;
        while (innerCurr != NULL)
        {
            printf(innerCurr->label);
            printf(" : ");
            printf(innerCurr->value);
            printf("\n");
            innerCurr = innerCurr -> sibling;
        }
        test = test->sibling;
    }
    
    
    return 0;
}