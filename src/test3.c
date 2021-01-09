
#include "core.h"

int n = 1; // no of doctors
int m = 5; // buffer size

struct user users[] = {
    { "user1", 1 },
    { "user2", 1 },
    { "user3", 1 },
    { "user4", 1 },
};

int totalUsers = sizeof(users) / sizeof(struct user);

int main()
{
    run();
    return 0;
}
