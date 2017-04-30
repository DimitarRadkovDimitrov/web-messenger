/********************************
Author: Dimitar Dimitrov 0922289
Date: February 20, 2017
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"

/**
 *main
 *Calls addUser and Remove user and handles incorrect input
 *
 *IN: int, char*
 *RETURN: N/A
 */
int main(int argc, char* argv[])
{
    int toRemove = 0;

    if (argc < 2)
    {
        printf("Error: Please enter a username\n");
        exit(0);
    }

    if (strcmp(argv[1], "-r") == 0)
    {
        toRemove = 1;
    }

    if ((toRemove == 1) && (argv[2]) && (argv[3]))
    {
        if (checkUser(argv[2], argv[3]) == 1)
        {
            removeUser(argv[2], argv[3]);
        }
        else
        {
            printf("Error: User does not exist in %sStream\n", argv[3]);
        }
    }
    else if ((argv[1]) && (argv[2]))
    {
        if (checkUser(argv[1], argv[2]) == 0)
        {
            addUser(argv[1], argv[2]);
        }
    }

    return 0;
}
