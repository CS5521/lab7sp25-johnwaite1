#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_CHILDREN 7

void usage()
{
    printf(2, "usage: schedtest loops tickets1 [tickets2 ... ticketsN]\n");
    printf(2, "       loops must be > 0\n");
    printf(2, "       each ticketsN must be >= 10\n");
    printf(2, "       up to %d children allowed\n", MAX_CHILDREN);
    exit();
}

void spin_forever()
{
    while (1)
    {
        // waste CPU cycles
    }
}

int main(int argc, char *argv[])
{
    int i;
    if (argc < 3 || argc > (2 + MAX_CHILDREN))
    {
        usage();
    }

    int loops = atoi(argv[1]);
    if (loops <= 0)
    {
        usage();
    }

    int pids[MAX_CHILDREN];
    int num_children = argc - 2;

    for (i = 0; i < num_children; i++)
    {
        int tickets = atoi(argv[i + 2]);
        if (tickets < 10)
        {
            usage();
        }

        int pid = fork();
        if (pid < 0)
        {
            printf(2, "fork failed\n");
            exit();
        }

        if (pid == 0)
        {
            // child
            settickets(tickets);
            spin_forever();
        }
        else
        {
            // parent
            pids[i] = pid;
        }
    }

    // parent runs ps() in a loop
    for (i = 0; i < loops; i++)
    {
        ps();
        sleep(3);
    }

    // kill and wait for children
    for (i = 0; i < num_children; i++)
    {
        kill(pids[i]);
    }

    for (i = 0; i < num_children; i++)
    {
        wait();
    }

    exit();
}
