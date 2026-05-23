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

int main(int argc, char *argv[]) {

    //checks for correct number of arguments
    if (argc != 2) {
        printf("Instructions: %s <message>\n", argv[0]);
        exit(1);
    }

    int id;
    struct info *ptr;

    // get existing shared memory segment
    id = shmget(key, sizeof(struct info), 0666);
    // checks if shmget fails
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

    // wait until previous data is consumed
    while (ptr->new_data_flag == 1);

    // copy message into shared memory
    strcpy(ptr->text, argv[1]);

    // signal new data is available
    ptr->new_data_flag = 1;

    //detach
    shmdt(ptr);

    return 0;
}