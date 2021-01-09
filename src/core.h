#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define CURE_TIME 2
#define IS_MULTI_PROCESS 0

struct user {
    char *name;
    int arrivalTime;
    int startCure;
    int doneCure;
    int curedby;
    pthread_t thread;
};

void* userThread(void* arg);
void run(int m, int n, struct user[], int usersCount);
struct user newUser(char* const name, int arrivalTime);
