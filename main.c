#include <semaphore.h>
#include <stdio.h>

#define STR_BUFF_SIZE 200
#define CURE_TIME 2
#define IS_MULTI_PROCESS 0

struct user {
    char name[STR_BUFF_SIZE];
    int arrivalTime;
};

const int n = 2; // consumers count
const int m = 5; // buffer size

struct user users[] = {
    { "user1", 1 },
    { "user2", 1 },
    { "user3", 2 },
    { "user4", 4 },
};

const int totalUsers = sizeof(users) / sizeof(struct user);

void cureDone(struct user u)
{
    printf("cure done for user \"%s\" (from %d to %d)\n", u.name, u.arrivalTime, u.arrivalTime + CURE_TIME);
}

sem_t mutex;

int main()
{
    sem_init(&mutex, IS_MULTI_PROCESS, 1);
    for (int i = 0; i < totalUsers; ++i) {
        cureDone(users[i]);
    }

    sem_destroy(&mutex);
    return 0;
}
