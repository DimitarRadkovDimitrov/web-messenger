/********************************
Author: Dimitar Dimitrov 0922289
Date: February 20, 2017
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stream.h"

/**
 *checkStream
 *Looks for a file in the messages folder that
 *matches the one given
 *
 *IN: char*, char*
 *RETURN: 1 on found, 0 on not found
 */
int checkStream(char* streamname, char* ext)
{
    char path[50] = "";
    strcpy(path, "messages/");
    strcat(path, streamname);
    strcat(path, "Stream");
    strcat(path, ext);
    strcat(path, "\0");

    if (access(path, F_OK ) != -1 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 *checkUser
 *Looks for a user in the stream user file specified
 *by the stream name given
 *
 *IN: char*, char*
 *RETURN: 1 on found, 0 on not found
 */
int checkUser(char* username, char* streamname)
{
    FILE* userStream = NULL;
    char path[50] = "";
    char string[256] = "";
    char* token;

    strcpy(path, "messages/");
    strcat(path, streamname);
    strcat(path, "StreamUsers\0");

    userStream = fopen(path, "r");

    if (userStream == NULL)
    {
        return 0;
    }
    else
    {
        while (fgets(string, 256, userStream) != NULL)
        {
            string[strlen(string)-1] = '\0';
            token = strtok(string, "\t");

            if (strcmp(string, username) == 0)
            {
                fclose(userStream);
                return 1;
            }
        }
        fclose(userStream);
        return 0;
    }
}

/**
 *updateStream
 *Appends to a stream, and updates the data file for
 *said stream
 *
 *IN: Post*
 *RETURN: N/A
 */
void updateStream(Post* st)
{
    int postSize = strlen(st->streamname) + strlen(st->username) + strlen(st->date) + strlen(st->text);
    char post[postSize+200];
    char command[postSize+200];

    strcpy(post, "\0");
    strcat(post, "Stream: ");
    strcat(post, st->streamname);
    strcat(post, "\nSender: ");
    strcat(post, st->username);
    strcat(post, "\nDate: ");
    strcat(post, st->date);
    strcat(post, st->text);
    strcat(post, "\0");

    strcpy(command, "./db -post \"");
    strcat(command, st->username);
    strcat(command, "\" \"");
    strcat(command, st->streamname);
    strcat(command, "\" \"");
    strcat(command, post);
    strcat(command, "\"\0");

    system(command);
}

/**
 *addUser
 *Creates stream files if they do not already exist
 *then adds the user to the stream's user file
 *
 *IN: char*, char*
 *RETURN: N/A
 */
void addUser(char* username, char* list)
{
    char* command = malloc(sizeof(char) * 100);

    strcpy(command, "./db -add ");
    strcat(command, username);
    strcat(command, " ");
    strcat(command, list);
    strcat(command, "\0");

    system(command);
    free(command);
}

/**
 *removeUser
 *Removes user from user file if one exists
 *
 *IN: char*, char*
 *RETURN: N/A
 */
void removeUser(char* username, char* list)
{
    char* command = malloc(sizeof(char) * 100);

    strcpy(command, "./db -remove ");
    strcat(command, username);
    strcat(command, " ");
    strcat(command, list);
    strcat(command, "\0");

    system(command);
    free(command);
}
