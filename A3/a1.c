#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isType(char* token)
{
    if ((strcmp(token, "int") == 0) || (strcmp(token, "char") == 0) || (strcmp(token, "float") == 0) ||
        (strcmp(token, "long") == 0) || (strcmp(token, "short") == 0) || (strcmp(token, "double") == 0) ||
        (strcmp(token, "void") == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char** parseForScope(char** functions, char** classVars, char* className)
{
    int i = 0;
    int j = 0;
    int isLocalVar = 0;
    int isModified = 0;
    int fnStart = 0;

    while (strcmp(functions[i], "") != 0)
    {
        if ((isType(functions[i]) == 1) && (strcmp(functions[i+2], "(") == 0))
        {
            fnStart = i;
        }
        while (strcmp(classVars[j], "") != 0)
        {
            if ((isType(classVars[j]) == 1) && (strcmp(classVars[j+1], functions[i]) == 0))
            {
                if (strcmp(functions[i-1], classVars[j]) == 0)
                {
                    isLocalVar = 1;
                }
                else if (isLocalVar == 0)
                {
                    if (isModified == 0)
                    {
                        int k = fnStart;
                        while (strcmp(functions[k+1], ")") != 0)
                        {
                            k++;
                        }
                        if (strcmp(functions[k], "(") != 0)
                        {
                            strcat(functions[k], ", ");
                        }
                        strcat(functions[k], "struct ");
                        strcat(functions[k], className);
                        strcat(functions[k], "* struct");
                        strcat(functions[k], className);

                        isModified = 1;
                    }

                    char* ref = malloc(sizeof(char) * (strlen(className) + strlen(functions[i]) + 10));
                    strcpy(ref, "struct");
                    strcat(ref, className);
                    strcat(ref, "->");
                    strcat(ref, functions[i]);
                    strcat(ref, "\0");
                    strcpy(functions[i], ref);
                    free(ref);
                }
            }
            j++;
        }
        if (((strcmp(functions[i], ";") == 0) || (strcmp(functions[i], "}") == 0)) && (strcmp(functions[i+1], "}") == 0)) //at functions end, reset local variable
        {
            isLocalVar = 0;
            isModified = 0;
        }
        i++;
        j = 0;
    }
    return functions;
}

/*Formatting of output .c file*/
void printArray(FILE* cFile, char** array)
{
    int i = 0;

    while ((strcmp(array[i], "") != 0) && (array[i+1] != NULL))
    {
        if ((array[i][0] == '/') || ((array[i][0] == '#') && (array[i][strlen(array[i])-1] == '>')))
        {
            fprintf(cFile, "%s\n", array[i]);
        }
        else if ((array[i][0] == '"') && (array[i-1][0] == '#'))
        {
            fprintf(cFile, "%s\n", array[i]);
        }
        else if (((strcmp(array[i], "!") == 0) || (strcmp(array[i], ">") == 0) || (strcmp(array[i], "<") == 0))
                && (strcmp(array[i+1], "=") == 0))
        {
            fprintf(cFile, "%s", array[i]);
        }
        else if (((strcmp(array[i], "+") == 0) || (strcmp(array[i], "-") == 0) || (strcmp(array[i], "=") == 0)
                || (strcmp(array[i], "*") == 0)) && (strcmp(array[i+1], array[i]) == 0))
        {
            fprintf(cFile, "%s", array[i]);
        }
        else if ((strcmp(array[i], "-") == 0) && (array[i+1][0] == '>'))
        {
            fprintf(cFile, "%s", array[i]);
        }
        else if ((strcmp(array[i+1], ";") == 0) || (strcmp(array[i+1], "(") == 0) || (strcmp(array[i+1], ")") == 0) || (strcmp(array[i], "(") == 0))
        {
            fprintf(cFile, "%s", array[i]);
        }
        else if ((strcmp(array[i], ";") == 0) && (strcmp(array[i+1], "}") == 0))
        {
            fprintf(cFile, "%s\n", array[i]);
        }
        else if ((strcmp(array[i], "{") == 0) || (strcmp(array[i], ";") == 0))
        {
            fprintf(cFile, "%s\n\t", array[i]);
        }
        else
        {
            fprintf(cFile, "%s ", array[i]);
        }
        i++;
    }
}

/*Free memory for 2D Array*/
void destroy2DArray(char** array, int rows)
{
    if (array != NULL)
    {
        for (int i = 0; i < rows; i++)
        {
            if (array[i] != NULL)
            {
                free(array[i]);
            }
        }
        free(array);
    }
}

/*Allocate Memory For 2D Array*/
char** make2DArray(int rows, int cols)
{
    char** array = malloc(sizeof(char*) * rows);

    for (int i = 0; i < rows; i++)
    {
        array[i] = malloc(sizeof(char) * cols);
        strcpy(array[i], "");
    }
    return array;
}

/*At one array's contents to the end of another*/
char** appendToArray(char** array, char** toBeAdded, int* endIndex, int j)
{
    while (strcmp(toBeAdded[j], "") != 0)
    {
        strcpy(array[*endIndex], toBeAdded[j]);
        (*endIndex) = (*endIndex) + 1;
        j++;
    }
    return array;
}

/*Append parameter characters and className for method overloading*/
char* getFunctionName(char* className, char* funcName, char** paramList)
{
    char* type = malloc(sizeof(char) * 31);
    char* newName = malloc(sizeof(char) * 50);

    int i = 0;

    if (paramList != NULL)
    {
        while (strcmp(paramList[i], "") != 0)
        {
            if (isType(paramList[i]) == 1)
            {
                char str[2] = "";
                str[0] = paramList[i][0];
                str[1] = '\0';
                strcat(type, str);
            }
            i++;
        }
    }

    strcpy(newName, className);
    strcat(newName, funcName);
    strcat(newName, type);
    free(type);
    return newName;
}

/*Returns the tokenized parameters of a functions in a 2D array*/
char** getParamList(char** oldClass, int start)
{
    char** paramList = make2DArray(100, 100); //might need more cols

    int j = 0;
    int i = start;
    while (strcmp(oldClass[i], "") != 0)
    {
        if (strcmp(oldClass[i], "(") == 0)
        {
            i++;

            while (strcmp(oldClass[i], ")") != 0)
            {
                strcpy(paramList[j], oldClass[i]);
                j++;
                i++;
            }
            return paramList;
        }
        i++;
    }

    return NULL;
}

char** getFcnPtrs(char** functions, char** fcnNames, char* className)
{
    char** fcnPtrs = make2DArray(100, 256);
    int i = 0;
    int j = 0;
    int bracCount = 0;

    while (strcmp(functions[i], "") != 0)
    {
        if ((isType(functions[i]) == 1) && (functions[i+2][0] == '('))
        {
            strcpy(fcnPtrs[j], functions[i]);
            strcat(fcnPtrs[j], " (*");
            strcat(fcnPtrs[j], fcnNames[j]);
            strcat(fcnPtrs[j], ")()\0");
        }
        if (strcmp(functions[i], "{") == 0)
        {
            bracCount++;
        }
        if (strcmp(functions[i], "}") == 0)
        {
            bracCount--;
        }

        if ((bracCount == 0) && (strcmp(functions[i], "}") == 0))
        {
            j++;
        }
        i++;
    }

    return fcnPtrs;
}

/*Returns a formatted constructor call based on class name*/
char* makeConstructorCall(char* className, char* id)
{
    char* callConst = malloc(sizeof(char) * 50);

    strcpy(callConst, "constructor");
    strcat(callConst, className);
    strcat(callConst, "(&");
    strcat(callConst, id);
    strcat(callConst, ");");
    return callConst;
}

char** makeConstructor(char* className, char** fcnNames)
{
    char** constructor = make2DArray(500, 256); //might need more rows

    int i = 0;
    int j = 0;
    strcpy(constructor[i], "void");
    i++;
    strcpy(constructor[i], "constructor");
    strcat(constructor[i], className);
    i++;
    strcpy(constructor[i], "(");
    i++;
    strcpy(constructor[i], "struct");
    i++;
    strcpy(constructor[i], className);
    i++;
    strcpy(constructor[i], "*");
    i++;
    strcpy(constructor[i], "struct");
    strcat(constructor[i], className);
    i++;
    strcpy(constructor[i], ")");
    i++;
    strcpy(constructor[i], "{");
    i++;

    while (strcmp(fcnNames[j], "") != 0)
    {
        strcpy(constructor[i], "struct");
        strcat(constructor[i], className);
        strcat(constructor[i], "->");
        strcat(constructor[i], fcnNames[j]);
        i++;

        strcpy(constructor[i], "=");
        i++;

        strcpy(constructor[i], "&");
        strcat(constructor[i], fcnNames[j]);
        i++;

        strcpy(constructor[i], ";");
        i++;
        j++;
    }

    strcpy(constructor[i], "}");
    i++;

    return constructor;
}

char** parseClass(char** oldClass, int start, int end)
{
    char** newClass = make2DArray(1000, 256);
    char** classVars = make2DArray(500, 256);
    char** functions = make2DArray(1000, 256);
    char** fcnNames = make2DArray(100, 256);
    char** oldFcnNames = make2DArray(100, 256);
    char** fcnPtrs = NULL;
    char** paramList = NULL;
    char** constructor = NULL;
    char* funcName = NULL;

    int i = 0; //old class
    int j = 0; //instance vars
    int k = 0; //functions
    int l = 0; //new class
    int o = 0; //fcn names
    int p = 0; //old fcn names
    int bracCount = 0;

    strcpy(newClass[l], oldClass[start]);
    l++;

    strcpy(newClass[l], oldClass[start+1]);
    l++;

    strcpy(newClass[l], oldClass[start+2]);
    l++;

    i = start;
    while (i < end)
    {
        if (isType(oldClass[i]) == 1)
        {
            if (strcmp(oldClass[i+2], "(") != 0) //Instance variable(s)
            {
                while (strcmp(oldClass[i], ";") != 0)
                {
                    strcpy(classVars[j], oldClass[i]);
                    i++;
                    j++;
                }
                strcpy(classVars[j], oldClass[i]);
                j++;
            }
            else //otherwise function
            {
                int fnStart = i;
                bracCount = 0;

                while (i < end)
                {
                    strcpy(functions[k], oldClass[i]);
                    k++;
                    i++;

                    if (strcmp(oldClass[i], "{") == 0)
                    {
                        bracCount++;
                    }
                    if (strcmp(oldClass[i], "}") == 0)
                    {
                        bracCount--;
                    }
                    if ((bracCount == 0) && (strcmp(oldClass[i], "}") == 0))
                    {
                        break;
                    }

                }
                strcpy(functions[k], oldClass[i]);
                k++;

                paramList = getParamList(oldClass, fnStart); //Parse for function elements
                funcName = getFunctionName(oldClass[start+1], oldClass[fnStart+1], paramList);

                strcpy(oldFcnNames[p], oldClass[fnStart+1]);
                p++;

                strcpy(fcnNames[o], funcName);
                o++;

                if (funcName != NULL)
                {
                    free(funcName);
                }
                destroy2DArray(paramList, 100);
            }
        }
        i++;
    }

    functions = parseForScope(functions, classVars, oldClass[start+1]);
    fcnPtrs = getFcnPtrs(functions, fcnNames, oldClass[start+1]);
    constructor = makeConstructor(oldClass[start+1], fcnNames);
    newClass = appendToArray(newClass, classVars, &l, 0);


    int z = 0;
    while (strcmp(fcnPtrs[z], "") != 0)
    {
        strcpy(newClass[l], fcnPtrs[z]);
        l++;
        strcpy(newClass[l], ";");
        l++;
        z++;
    }

    strcpy(newClass[l], "}");
    l++;

    strcpy(newClass[l], ";"); //Marks the end of a class
    l++;

    newClass = appendToArray(newClass, functions, &l, 0);
    newClass = appendToArray(newClass, constructor, &l, 0); //Append elements of array in order

    int x = 0;

    while (strcmp(newClass[x], "") != 0)
    {
        int y = 0;

        if (strcmp(newClass[x], "class") == 0) //replace any 'class' in fcn with struct
        {
            strcpy(newClass[x], "struct");
        }
        while (strcmp(oldFcnNames[y], "") != 0) //Replace oldfcnnames with new function names
        {
            if (strcmp(newClass[x], oldFcnNames[y]) == 0)
            {
                strcpy(newClass[x], fcnNames[y]);
            }
            y++;
        }
        x++;
    }

    destroy2DArray(constructor, 500);
    destroy2DArray(classVars, 500);
    destroy2DArray(functions, 1000);
    destroy2DArray(fcnNames, 100);
    destroy2DArray(oldFcnNames, 100);
    destroy2DArray(fcnPtrs, 100);

    return newClass;
}

void parseTokens(char* fileName)
{
    FILE* tokenFile = fopen("tokens.txt", "r");
    fileName[strlen(fileName) - 1] = '\0';
    FILE* cFile = fopen(fileName, "w");

    int strSize = 509;
    char* string = malloc(sizeof(char) * strSize);

    char** master = make2DArray(10000, 256);
    char** class = NULL;
    char* prev = malloc(sizeof(char) * 256);
    char* prevPrev = malloc(sizeof(char) * 256);

    int i = 0;
    int start = 0;
    int end = 0;

    while (fgets(string, strSize, tokenFile) != NULL)
    {
        string[strlen(string) - 1] = '\0';

        if (strcmp(string, "class") == 0) //"class"
        {
            strcpy(string, "struct");
            strcpy(prevPrev, string);

            strcpy(master[i], prevPrev);
            i++;

            fgets(string, strSize, tokenFile); //"class_name"
            string[strlen(string) - 1] = '\0';

            strcpy(prev, string);
            strcpy(master[i], prev);
            i++;

            fgets(string, strSize, tokenFile); //"{" or "object_name"
            string[strlen(string) - 1] = '\0';

            strcpy(master[i], string);
            i++;

            if (strcmp(string, "{") == 0) //Parse for classes
            {
                start = i - 3;

                while (fgets(string, strSize, tokenFile) != NULL)
                {
                    string[strlen(string) - 1] = '\0';

                    if ((strcmp(prev, "}") == 0) && (strcmp(string, ";") == 0))
                    {
                        break;
                    }

                    strcpy(master[i], string);
                    i++;

                    strcpy(prev, string);
                }

                end = i;

                class = parseClass(master, start, end);
                master = appendToArray(master, class, &start, 0);
                i = start;
                destroy2DArray(class, 1000);
            }
            else
            {  //Make constructor calls for each 'object' created
                char** constCalls = make2DArray(100, 256);
                int j = 0;
                while (strcmp(string, ";") != 0)
                {
                    if (strcmp(string, ",") != 0)
                    {
                        char* callingCode = makeConstructorCall(prev, string);
                        callingCode[strlen(callingCode) - 1] = '\0';

                        strcpy(constCalls[j], callingCode);
                        j++;

                        strcpy(constCalls[j], ";");
                        j++;
                        free(callingCode);
                    }

                    fgets(string, strSize, tokenFile);
                    string[strlen(string) - 1] = '\0';

                    strcpy(master[i], string);
                    i++;
                }

                master = appendToArray(master, constCalls, &i, 0);
                destroy2DArray(constCalls, 100);
            }
        }
        else
        {
            strcpy(master[i], string);
            i++;
        }
    }

    printArray(cFile, master);
    free(prevPrev);
    free(prev);
    free(string);
    destroy2DArray(master, 10000);
    fclose(cFile);
    fclose(tokenFile);
}

/*Splits file based on whitespace, punctuation, and comments*/
void splitFile(FILE* fp)
{
    if (fp != NULL)
    {
        FILE* outFile = fopen("tokens.txt", "w");
        char prev = '\0';
        int curr = 0;
        int count = 0;

        while ((curr = fgetc(fp)) != EOF)
        {
            if (curr == '/')
            {
                while (curr != '\n')
                {
                    fprintf(outFile, "%c", curr);
                    curr = fgetc(fp);
                }
                fprintf(outFile, "\n");
            }
            else if (curr == '"')
            {
                if ((prev != ' ') && (prev != '\t') && (prev != '\n') && (prev != '\r'))
                {
                    fprintf(outFile, "\n");
                }

                count++;

                while (count % 2 != 0)
                {
                    fprintf(outFile, "%c", curr);
                    curr = fgetc(fp);
                    if ((curr == '"') && (prev != '\\'))
                    {
                        count++;
                    }
                    prev = curr;
                }

                fprintf(outFile, "%c", curr);
                count = 0;
            }
            else if ((curr == '+') || (curr == '-') || (curr == '*') || (curr == '/') || (curr == '%') || (curr == '='))
            {
                if ((prev != ' ') && (prev != '\t') && (prev != '\n') && (prev != '\r'))
                {
                    fprintf(outFile, "\n%c", curr);
                }
                else
                {
                    fprintf(outFile, "%c", curr);
                }
            }
            else if ((curr == ' ') || (curr == '\t') || (curr == '\n') || (curr == '\r')) //New lines for whitespace
            {
                if ((prev != ' ') && (prev != '\t') && (prev != '\n') && (prev != '\r'))
                {
                    fprintf(outFile, "\n");
                }
            }
            else if ((curr == ';') || (curr == ',') || (curr == '{') || (curr == '}') || (curr == '(') || (curr == ')')) //New Lines and character for punctuation
            {
                if ((prev != ' ') && (prev != '\t') && (prev != '\n') && (prev != '\r'))
                {
                    fprintf(outFile, "\n");
                }
                fprintf(outFile, "%c", curr);
            }
            else
            {
                if ((prev == ';') || (prev == ',') || (prev == '{') || (prev == '}') || (prev == '(') || (prev == ')'))
                {
                    fprintf(outFile, "\n");
                }
                if ((prev == '+') || (prev == '-') || (prev == '=') || (prev == '*') || (prev == '/') || (prev == '%'))
                {
                    fprintf(outFile, "\n");
                }

                fprintf(outFile, "%c", curr);
            }
            prev = curr;
        }
        fclose(outFile);
    }
    else
    {
        printf("File not found\n");
        exit(0);
    }
}

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    splitFile(fp);
    fclose(fp);
    parseTokens(argv[1]);
    return 0;
}
