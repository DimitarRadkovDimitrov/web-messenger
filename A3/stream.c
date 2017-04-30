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
    char* path = malloc(sizeof(char) * 50);
    char* str = malloc(sizeof(char) * 1000);
    char* prev = malloc(sizeof(char) * 256);

    int size = 0;
    strcpy(path, "messages/");
    strcat(path, st->streamname);
    strcat(path, "Stream\0");

    FILE* stream = fopen(path, "a");
    fprintf(stream, "Sender: %s\n", st->username);
    fprintf(stream, "Date: %s", st->date);
    fprintf(stream, "%s", st->text);
    fclose(stream);

    strcat(path, "Data\0");
    stream = fopen(path, "r+");

    while (fgets(str, 1000, stream) != NULL)
    {
        strcpy(prev, str);
    }

    prev[strlen(prev)-1] = '\0';
    size = sizeof(char) * strlen(st->username) + strlen(st->date) + strlen(st->text) + 15;
    size = size + atoi(prev);
    fprintf(stream, "%d\n", size);

    free(path);
    free(str);
    free(prev);
    fclose(stream);
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
    char* stPath = malloc(sizeof(char) * 50);
    char* usrPath = malloc(sizeof(char) * 50);
    char* dataPath = malloc(sizeof(char) * 50);
    FILE* fp = NULL;

    strcpy(stPath, "messages/");
    strcat(stPath, list);
    strcat(stPath, "Stream\0");
    fp = fopen(stPath, "a");
    fclose(fp);

    strcpy(usrPath, stPath);
    strcat(usrPath, "Users\0");
    fp = fopen(usrPath, "a");
    fprintf(fp, "%s\t0\n", username);
    fclose(fp);

    strcpy(dataPath, stPath);
    strcat(dataPath, "Data\0");
    fp = fopen(dataPath, "a");

    fclose(fp);
    free(stPath);
    free(usrPath);
    free(dataPath);
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
    char* buff = malloc(sizeof(char) * 256);
    char* userList = malloc(sizeof(char) * 256);
    char* path = malloc(sizeof(char) * 50);
    char* token;
    FILE* fp;

    strcpy(path, "messages/");
    strcat(path, list);
    strcat(path, "StreamUsers\0");

    fp = fopen(path, "r");

    while (fgets(buff, 256, fp) != NULL)
    {
        buff[strlen(buff)-1] = '\0';
        token = strtok(buff, "\t");

        if (strcmp(buff, username) != 0)
        {
            strcat(userList, buff);
            strcat(userList, "\t");
            token = strtok(NULL, "\n");
            strcat(userList, token);
            strcat(userList, "\n");
        }
    }
    strcat(userList, "\0");
    fclose(fp);

    fp = fopen(path, "w");
    fprintf(fp, "%s", userList);

    fclose(fp);
    free(path);
    free(buff);
    free(userList);
}
