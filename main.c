#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
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

const int n = 2; // no of doctors
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
        printf("joining %d\n", i);
        pthread_join(users[i].thread, NULL);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

sem_t seat_mutex;
sem_t doctor_mutex;

bool takeASeat(struct user* me)
{
    int startLookingTime = getCurrentTime();
    sem_wait(&seat_mutex);
    int endLookingTime = getCurrentTime();
    if (startLookingTime != endLookingTime) { // hamun moghe ja khali nabude
        // there is no seat. leave building
        sem_post(&seat_mutex);
        printStats(me);
        return false;
    }
    return true;
}

void goToDrRoom(struct user* me){
    sem_post(&seat_mutex); // ba'd inke vared doctor shodi az jat pasho
    me->startCure = getCurrentTime();
}

void leaveDrRoom(struct user* me){
    me->doneCure = getCurrentTime();
    printStats(me);
    sem_post(&doctor_mutex);
}

void cure(){
    sleep(CURE_TIME);
}

void waitForDoctor(){
    sem_wait(&doctor_mutex);
}

void handleArrival(struct user* me){
    sleep(me->arrivalTime);
}

void* userThread(void* arg)
{
    int ind = *((int*)arg);
    struct user* me = &users[ind];
    handleArrival(me);

    if(!takeASeat(me)){
        return NULL;
    }

    waitForDoctor();
    goToDrRoom(me);
    cure(); //critical section
    leaveDrRoom(me);

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
    sem_destroy(&seat_mutex);
    return 0;
}
