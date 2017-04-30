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
    char* stream = malloc(sizeof(char) * 256);
    char* string = malloc(sizeof(char) * 256);
    char* token = NULL;
    int i = 1;
    int toRemove = 0;

    if (argc < 2)
    {
        printf("Error: Please enter a username\n");
        exit(0);
    }
    while (i < argc)
    {
        if (strcmp(argv[i], "-r") == 0)
        {
            toRemove = 1;
        }
        else
        {
            strcat(string, argv[i]);
            strcat(string, " ");
        }
        i++;
    }
    string[strlen(string)-1] = '\0';

    printf("list streams: ");
    scanf("%s", stream);

    token = strtok(stream, ",");

    if (token != NULL)
    {
        if (toRemove == 1)
        {
            if (checkUser(string, token) == 1)
            {
                removeUser(string, token);
            }
            else
            {
                printf("Error: User does not exist in %sStream\n", token);
            }
        }
        else
        {
            if (checkUser(string, token) == 0)
            {
                addUser(string, token);
            }
        }
    }

    while (token != NULL)
    {
        token = strtok(NULL, ",");

        if (token != NULL)
        {
            if (toRemove == 1)
            {
                if (checkUser(string, token) == 1)
                {
                    removeUser(string, token);
                }
                else
                {
                    printf("Error: User does not exist in %sStream\n", token);
                }
            }
            else
            {
                if (checkUser(string, token) == 0)
                {
                    addUser(string, token);
                }
            }
        }
    }

    free(string);
    free(stream);
    return 0;
}
