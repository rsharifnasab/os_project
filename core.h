#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "lib/clui.h"

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

extern int n; // no of doctors
extern int m; // buffer size

extern struct user users[];
extern int totalUsers;


unsigned long programStart;

bool colorPrint = false;

void printStats(struct user* u)
{
    if (u->doneCure == 0) {
        // left building
        if (colorPrint)
            change_color(COLOR_RED);
        printf("cure failed for user \"%s\" [arrival:%d] \n", u->name, u->arrivalTime);
        if (colorPrint)
            reset_color();
    } else {
        if (colorPrint)
            change_color(COLOR_GREEN);
        printf("cure done for user \"%s\" [arrival:%d] (cure from %d to %d)\n",
                u->name, u->arrivalTime, u->startCure, u->doneCure);
        if (colorPrint)
            reset_color();
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
        int* arrivalTime = (int*) calloc(1, sizeof(int));
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
    sem_post(&seatMutex);      // ba'd inke vared doctor shodi az jat pasho
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

void run()
{

    colorPrint = isatty(fileno(stdout));

    // initialize semaphores
    freeSeats = m;
    sem_init(&seatCountMutex, IS_MULTI_PROCESS, 1);
    sem_init(&seatMutex, IS_MULTI_PROCESS, m);
    sem_init(&doctorMutex, IS_MULTI_PROCESS, n);

    programStart = (unsigned long)time(NULL);

    // run threads
    createThreads(users, totalUsers);
    joinThreads(users, totalUsers);

    sem_destroy(&doctorMutex);
    sem_destroy(&seatMutex);
}

