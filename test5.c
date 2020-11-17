
#include "core.h"

int n = 5; // no of doctors
int m = 1; // buffer size

struct user users[] = {
    { "user1", 15 },
    { "user2", 15 },
    { "user3", 2 },
    { "user4", 4 },
};

int totalUsers = sizeof(users) / sizeof(struct user);

int main()
{
    run();
    return 0;
}
