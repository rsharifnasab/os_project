#include "core.h"

int main()
{
    const int m = 1;
    const int n = 1;

    struct user users[] = {
        newUser("user1", 1),
        newUser("user2", 1),
        newUser("user3", 2),
        newUser("user4", 3),
    };

    int usersCount = sizeof(users) / sizeof(struct user);
    run(m, n, users, usersCount);
    return 0;
}
