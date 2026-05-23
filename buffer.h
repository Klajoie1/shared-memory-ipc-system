#define MAX_SIZE 8
#define KEY 123

typedef struct bufferStruct {
int in;
int out;
int content[MAX_SIZE];// will hold ASCII code of characters
}bufferStruct;