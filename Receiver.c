#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define MAX_SIZE 128

key_t key = 123;

struct info {
    int new_data_flag;
    char text[MAX_SIZE];
};

int main()
{
    int id;
    struct info *ptr;

    // create or connect to shared memory segment
    id = shmget(key, sizeof(struct info), IPC_CREAT | 0666);
    // checks if shmget failed
    if (id < 0) {
        perror("shmget failed");
        exit(1);
    }

    // attach shared memory to this process
    ptr = (struct info *) shmat(id, NULL, 0);

    // check if shmat fails
    if (ptr == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    // initialize flag
    ptr->new_data_flag = 0;

    while(1) {
        // wait for message
        while(ptr->new_data_flag == 0);

        // print message
        printf("%s\n", ptr->text);

        // check for quit
        if (strcmp(ptr->text, "quit") == 0) {
            break;
        }

        // reset flag
        ptr->new_data_flag = 0;
    }

    // detach
    shmdt(ptr);
    // remove shared memory
    shmctl(id, IPC_RMID, NULL);

    return 0;
}