#ifndef __STREAM_H__
#define __STREAM_H__

typedef struct userPost {
    char *username;
    char *streamname;
    char *date;
    char *text;
} Post;

int checkStream(char* streamname, char* ext);
int checkUser(char* username, char* streamname);
void updateStream(Post* st);
void addUser(char* username, char* list);
void removeUser(char* username, char* list);

#endif
