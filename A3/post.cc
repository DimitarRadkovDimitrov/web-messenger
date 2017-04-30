/********************************
Author: Dimitar Dimitrov 0922289
Date: March 17, 2017
*********************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"stream.h"

class PostEntry
{
    void readInput(struct PostEntry* entry, char* userName, Post** post)
    {
        char* stream = malloc(sizeof(char) * 256);
        char* buffer = malloc(sizeof(char) * 1000);
        char* text = malloc(sizeof(char) * 1000);
        printf("stream: ");
        scanf("%s", stream);

        if (checkStream(stream, "") == 0)
        {
            printf("Error: Stream does not exist.\n");
            exit(0);
        }

        if (checkUser(userName, stream) == 0)
        {
            printf("Error: User has no authorization\n");
            exit(0);
        }

        printf("enter text: ");

        fgets(buffer, 1000, stdin);
        buffer[strlen(buffer)-1] = '\0';
        strcpy(text, buffer);

        while (fgets(buffer, 1000, stdin) != NULL)
        {
            buffer[strlen(buffer)-1] = '\0';

            printf("- ");
            if (strcmp(buffer, "^D") == 0)
            {
                break;
            }
            else
            {
                strcat(text, buffer);
                strcat(text, "\n");
            }
        }
        strcat(text, "\0");
        printf("\n");

        entry->PostEntryformatEntryccc(entry, userName, text, stream, &(*post));
        free(text);
        free(buffer);
        free(stream);
        free(userName);
    }

    void formatEntry(struct PostEntry* entry, char* userName, char* text, char* stream, Post** post)
    {
        (*post) = malloc(sizeof(Post));

        if ((*post) == NULL)
        {
            printf("Error: Malloc Failed\n");
        }
        else
        {
            (*post)->username = malloc(sizeof(char) * strlen(userName) + 1);
            (*post)->streamname = malloc(sizeof(char) * strlen(stream) + 1);
            (*post)->text = malloc(sizeof(char) * strlen(text) + 5);

            strcpy((*post)->username, userName);
            strcpy((*post)->streamname, stream);
            strcpy((*post)->text, text);
            strcat((*post)->text, "\n\0");

            entry->PostEntrygetTimeDatec(&((*post)->date));
            entry->PostEntrysubmitPost(&(*post));
        }
    }

    void getTimeDate(char** dateString)
    {
        time_t currTime = time(NULL);
        (*dateString) = (ctime(&currTime));
    }

    void submitPost(Post** post)
    {
        updateStream(*post);
    }

    void destroyPost(Post* post)
    {
        if (post != NULL)
        {
            free(post->username);
            free(post->streamname);
            free(post->text);
            free(post);
        }
    }
};

int main(int argc, char* argv[])
{
    class PostEntry entry;
    Post* post = NULL;

    if (checkStream(argv[2], "") == 0)
    {
        printf("Error: Stream does not exist.\n");
        exit(0);
    }

    if (checkUser(argv[1], argv[2]) == 0)
    {
        printf("Error: User has no authorization\n");
        exit(0);
    }

    entry.PostEntryformatEntryccc(&entry, argv[1], argv[3], argv[2], &post);
    entry.PostEntrydestroyPost(post);
    return 0;
}
