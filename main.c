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
    pthread_t thread;
};

const int n = 1; // no of doctors
const int m = 1; // buffer size

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
    if (u->doneCure == 0) {
        // left building
        printf("cure failed for user \"%s\" [arrival:%d] \n", u->name, u->arrivalTime);
    } else {
        printf("cure done for user \"%s\" [arrival:%d] (cure from %d to %d)\n", u->name, u->arrivalTime, u->startCure, u->doneCure);
    }
}

int getCurrentTime()
{
    unsigned long l = ((unsigned long)time(NULL)) - programStart;
    return l;
}

void* userThread(void* arg);
void createThreads(struct user* users, int totalUsers)
{
    for (int i = 0; i < totalUsers; ++i) {
        int* arrivalTime = calloc(1, sizeof(int));
        *arrivalTime = i;
        pthread_create(&(users[i].thread), NULL, userThread, arrivalTime);
    }
}

void joinThreads(struct user* users, int totalUsers)
{
    for (int i = 0; i < totalUsers; ++i) {
        pthread_join(users[i].thread, NULL);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

sem_t seat_mutex;
sem_t doctor_mutex;

void takeASeat(struct user* me)
{
    int startLookingTime = getCurrentTime();
    sem_wait(&seat_mutex);
    int endLookingTime = getCurrentTime();
    if (startLookingTime != endLookingTime) {
        // there is no seat. leave building
        sem_post(&seat_mutex);
        pthread_exit(&(me->thread));
        printStats(me);
    }
}

void* userThread(void* arg)
{
    int ind = *((int*)arg);
    struct user* me = &users[ind];

    sleep(me->arrivalTime);
    takeASeat(me);

    //wait
    sem_wait(&doctor_mutex);
    me->startCure = getCurrentTime();

    //critical section
    sleep(CURE_TIME);

    //signal
    me->doneCure = getCurrentTime();
    printStats(me);
    sem_post(&doctor_mutex);
    return NULL;
}

int main()
{
    programStart = (unsigned long)time(NULL);
    sem_init(&seat_mutex, IS_MULTI_PROCESS, m);
    sem_init(&doctor_mutex, IS_MULTI_PROCESS, n);

    createThreads(users, totalUsers);
    joinThreads(users, totalUsers);

    sem_destroy(&doctor_mutex);
    return 0;
}
