#include "core.h"

int n = 1; // no of doctors
int m = 2; // buffer size

struct user users[] = {
    { "User1", 1 },
    { "User2", 1 },
    { "User3", 1 },
    { "User4", 1 },
    { "User5", 1 },
    { "User6", 3 },
    { "User7", 3 },
    { "User8", 10 },
};

int totalUsers = sizeof(users) / sizeof(struct user);

int main()
{
    run();
    return 0;
}
