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
        //printf("joining %d\n", i);
        pthread_join(users[i].thread, NULL);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int freeSeats;
sem_t seatCountMutex;
sem_t seatMutex;
sem_t doctorMutex;

bool takeASeat(struct user* me)
{
    bool success = false;
    sem_wait(&seatCountMutex); // lock seat_count
    if (freeSeats == 0) {
        // there is no seat. leave building
        printStats(me);

        sem_post(&seatCountMutex); // unlock seat_count
        return false;
    } else {
        freeSeats--;
        sem_wait(&seatMutex);

        sem_post(&seatCountMutex); // unlock seat_count
        return true;
    }

}

void goToDrRoom(struct user* me)
{
    sem_wait(&seatCountMutex); // lock seat_count
    freeSeats++;
    sem_post(&seatMutex); // ba'd inke vared doctor shodi az jat pasho
    sem_post(&seatCountMutex); // unlock seat_count
    me->startCure = getCurrentTime();
}

void leaveDrRoom(struct user* me)
{
    me->doneCure = getCurrentTime();
    printStats(me);
    sem_post(&doctorMutex);
}

void cure()
{
    sleep(CURE_TIME);
}

void waitForDoctor()
{
    sem_wait(&doctorMutex);
}

void handleArrival(struct user* me)
{
    sleep(me->arrivalTime);
}

void* userThread(void* arg)
{
    int ind = *((int*)arg);
    struct user* me = &users[ind];
    handleArrival(me);

    if (!takeASeat(me)) {
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

    freeSeats = m;
    sem_init(&seatCountMutex, IS_MULTI_PROCESS, 1);
    sem_init(&seatMutex, IS_MULTI_PROCESS, m);
    sem_init(&doctorMutex, IS_MULTI_PROCESS, n);

    createThreads(users, totalUsers);
    joinThreads(users, totalUsers);

    sem_destroy(&doctorMutex);
    sem_destroy(&seatMutex);
    return 0;
}
