
#include "core.h"

int n = 2; // no of doctors
int m = 2; // buffer size

struct user users[] = {
    { "user0", 0 },
    { "user1", 0 },
    { "user2", 1 },
    { "user3", 5 },
    { "user4", 9 },
};

int totalUsers = sizeof(users) / sizeof(struct user);

int main()
{
    run();
    return 0;
}
