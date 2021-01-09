#include "core.h"

char** doctors;
struct user* users;
int n;

unsigned long long programStart;

bool colorPrint = false;

sem_t printMutex;

void change_print_color(int r, int g, int b)
{
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void reset_print_color()
{
    printf("[0m");
}

void printStats(struct user* u)
{
    sem_wait(&printMutex);
    if (u->doneCure == 0) {
        // left building
        if (colorPrint)
            change_print_color(255, 0, 0);
        printf("cure failed for user \"%s\" [arrival:%d] \n", u->name, u->arrivalTime);
        if (colorPrint)
            reset_print_color();
    } else {
        if (colorPrint)
            change_print_color(0, 255, 0);
        printf("cure done for user \"%s\" by %d [arrival:%d] (cure from %d to %d)\n",
            u->name, u->curedby, u->arrivalTime, u->startCure, u->doneCure);
        if (colorPrint)
            reset_print_color();
    }
    sem_post(&printMutex);
}

int getCurrentTime()
{
    long long l = ((unsigned long long)time(NULL)) - programStart;
    return l;
}

void* userThread(void* arg);

void createThreads(struct user* users, int totalUsers)
{
    for (int i = 0; i < totalUsers; ++i) {
        int* arrivalTime = (int*)calloc(1, sizeof(int));
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

void initialize_doctors(char*** doctors, int doctorsCount)
{
    n = doctorsCount;
    *doctors = (char**)calloc(doctorsCount, sizeof(char*));
}

struct user newUser(char* const name, int arrivalTime)
{
    struct user u = {
        .name = name,
        .arrivalTime = arrivalTime,
        .startCure = 0,
        .doneCure = 0,
        .curedby = 0,
    };
    return u;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int freeSeats;
sem_t seatCountMutex;
sem_t seatMutex;
sem_t doctorMutex;
sem_t doctorArrMutex;

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

int getFreeDoctor(struct user* me)
{
    int ans = -1;
    sem_wait(&doctorArrMutex);
    for (int i = 0; i < n; ++i) {
        if (doctors[i] == NULL) { // ok we can go to this doctor
            ans = i;
            goto ret;
        }
    }

ret:
    doctors[ans] = me->name;
    sem_post(&doctorArrMutex);
    return ans;
}

void goToDrRoom(struct user* me)
{
    sem_wait(&seatCountMutex); // lock seat_count
    freeSeats++;
    sem_post(&seatMutex);      // ba'd inke vared doctor shodi az jat pasho
    sem_post(&seatCountMutex); // unlock seat_count

    me->curedby = getFreeDoctor(me);
    me->startCure = getCurrentTime();
}

void leaveDrRoom(struct user* me)
{
    me->doneCure = getCurrentTime();
    printStats(me);

    sem_wait(&doctorArrMutex);
    doctors[me->curedby] = NULL;
    sem_post(&doctorArrMutex);

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
    struct user* me = users +ind;
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

void run(int m, int n, struct user usersArr[], int usersCount)
{

    colorPrint = isatty(fileno(stdout));

    // initialize
    users = usersArr;
    initialize_doctors(&doctors, n);
    freeSeats = m;

    sem_init(&seatCountMutex, IS_MULTI_PROCESS, 1);
    sem_init(&seatMutex, IS_MULTI_PROCESS, m);
    sem_init(&doctorMutex, IS_MULTI_PROCESS, n);
    sem_init(&doctorArrMutex, IS_MULTI_PROCESS, 1);
    sem_init(&printMutex, IS_MULTI_PROCESS, 1);

    programStart = (unsigned long)time(NULL);

    // run threads
    createThreads(users, usersCount);
    joinThreads(users, usersCount);

    // cleanup
    sem_destroy(&seatCountMutex);
    sem_destroy(&seatMutex);
    sem_destroy(&doctorMutex);
    sem_destroy(&doctorArrMutex);
    sem_destroy(&printMutex);
    free(doctors);
}
