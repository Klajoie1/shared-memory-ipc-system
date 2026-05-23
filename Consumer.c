#include <stdio.h>
#include <stdlib.h>
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

    while (1)
    {
        printf("Press enter to consume (0 to quit): ");
        scanf("%c", &input);

        if (input == '0') {
            break;
        }
        // checks if buffer is empty
        if (ptr->in == ptr->out)
        {
            printf("Buffer empty\n");
        }
        else
        {
            int value = ptr->content[ptr->out];         // get oldest value from buffer
            ptr->out = (ptr->out + 1) % MAX_SIZE;   // move to next consume position
            printf("Consumed: %d\n", value); // print value

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
    // remove shared memory
    shmctl(id, IPC_RMID, NULL);

    return 0;
}