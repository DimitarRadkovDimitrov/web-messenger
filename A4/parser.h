#ifndef __PARSER_H__
#define __PARSER_H__

void split(FILE* fp);
void parseTag(char tag, char** elementList);
char** make2DArray(int rows, int cols);
void destroy2DArray(char** array, int rows);
#endif
