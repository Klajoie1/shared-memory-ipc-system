#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "buffer.h"

int main()
{
    int id;
    bufferStruct *ptr;
    char input;

    // connect to shared memory segment
    id = shmget(KEY, sizeof(bufferStruct), IPC_CREAT | 0666);
    // checks if shmget failed
    if (id < 0)
    {
        perror("shmget failed");
        exit(1);
    }

    // attach shared memory to this process
    ptr = (bufferStruct *)shmat(id, NULL, 0);
    
    // check if shmat fails
    if (ptr == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }
    
    // intialize to avoid garbage values
    ptr->in = 0;
    ptr->out = 0;

    srand(time(NULL));

    while (1)
    {
        printf("Press enter to produce (0 to quit): ");
        scanf("%c", &input);

        if (input == '0') {
            break;
        }
        // check if buffer is full
        if ((ptr->in + 1) % MAX_SIZE == ptr->out)
        {
            printf("Buffer full\n");
        }
        else
        {
            int value = rand() % 100;        // random value between 0-99
            ptr->content[ptr->in] = value;              // store value in shared buffer
            ptr->in = (ptr->in + 1) % MAX_SIZE;     // move to next insert position
            printf("Produced: %d\n", value); // print value

            int i = ptr->out;
            // loop through buffer from oldest to newest
            while (i != ptr->in)
            {
                printf("%d ", ptr->content[i]);
                i = (i + 1) % MAX_SIZE;
            }
            printf("\n");
        }
        
    }
    //detach shared memory
    shmdt(ptr);
    
    return 0;
}