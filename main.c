#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define STR_BUFF_SIZE 200
#define CURE_TIME 2
#define IS_MULTI_PROCESS 0

struct user {
    char name[STR_BUFF_SIZE];
    int arrivalTime;
    int startCure;
    int doneCure;
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

unsigned long programStart;

void printStats(struct user* u)
{
    printf("cure done for user \"%s\" [arrival:%d] (cure from %d to %d)\n", u->name, u->arrivalTime, u->startCure, u->doneCure);
}

int getCurrentTime()
{
    unsigned long l = ((unsigned long)time(NULL)) - programStart;
    return l;
}

sem_t mutex;
void* userThread(void* arg)
{
    int ind = *((int*)arg);
    struct user* me = &users[ind];

    sleep(me->arrivalTime);

    //wait
    sem_wait(&mutex);
    me->startCure = getCurrentTime();

    //critical section
    sleep(CURE_TIME);

    //signal
    me->doneCure = getCurrentTime();
    printStats(me);
    sem_post(&mutex);
    return NULL;
}

pthread_t* createThreads(int totalUsers)
{
    pthread_t* userThreads = (pthread_t*)calloc(totalUsers, sizeof(pthread_t));
    for (int i = 0; i < totalUsers; ++i) {
        int* arrivalTime = calloc(1, sizeof(int));
        *arrivalTime = i;
        pthread_create(&(userThreads[i]), NULL, userThread, arrivalTime);
    }
    return userThreads;
}

void joinThreads(pthread_t* threadsToJoin, int totalUsers)
{
    for (int i = 0; i < totalUsers; ++i) {
        pthread_join(threadsToJoin[i], NULL);
    }
}

int main()
{
    programStart = (unsigned long)time(NULL);
    sem_init(&mutex, IS_MULTI_PROCESS, n);

    pthread_t* userThreads = createThreads(totalUsers);
    joinThreads(userThreads, totalUsers);

    sem_destroy(&mutex);
    free(userThreads);
    return 0;
}
