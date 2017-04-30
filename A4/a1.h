#ifndef __A1_H__
#define __A1_H__

char** parseForScope(char** functions, char** classVars, char* className);

char** getFcnPtrs(char** functions, char** fcnNames, char* className);

int isPunc(char* token);

int isType(char* token);

void printArray(char** array);

void destroy2DArray(char** array, int rows);

char** make2DArray(int rows, int cols);

char** appendToArray(char** array, char** toBeAdded, int* endIndex, int j);

char* getFunctionName(char* className, char* funcName, char** paramList);

char** getParamList(char** oldClass, int start);

char* makeConstructorCall(char* className, char* id);

char** makeConstructor(char* className, char** fcnNames);

char** parseClass(char** oldClass, int start, int end);

void parseTokens();

void splitFile(FILE* fp);

#endif
