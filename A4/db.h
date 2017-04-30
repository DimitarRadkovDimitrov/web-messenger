#ifndef __DIMITROV_DB_H__
#define __DIMITROV_DB_H__

void initStreams(MYSQL* mysql, char* username, char* stream, char* query);
void removeUser(MYSQL* mysql, char* username, char* stream, char* query);
void post(MYSQL* mysql, char* username, char* stream, char* message, char* query);
void findStreams(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* username, char* query);
void view(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* username, char* stream, char* index, char* flag, char* query);
void dropAll(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query);
void clearAll(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query);
void showStreams(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query);
void showUsers(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query);
void showPosts(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query);
#endif
