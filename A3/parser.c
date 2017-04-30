/********************************
Author: Dimitar Dimitrov 0922289
Date: March 17, 2017
*********************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    split(fp);
    fclose(fp);
    return 0;
}

/**
 *split
 *splits tags up into tags and attributes
 *
 *IN: FILE*
 *RETURN: N/A
 */
void split(FILE* fp)
{
    printf("<html>\n");
    printf("<body>\n");
    if (fp != NULL)
    {
        int chr;
        char tag;

        while ((chr = fgetc(fp)) != EOF)
        {
            if (chr == '.')
            {
                int i = 0;
                char string[1000] = "";
                char** elementList = NULL;

                char* token = NULL;

                chr = fgetc(fp);
                tag = chr;
                chr = fgetc(fp);

                while ((chr = fgetc(fp)) != ')')
                {
                    if (chr == '"')
                    {
                        int count = 0;
                        count++;

                        while (count % 2 != 0)
                        {
                            string[i] = chr;
                            i++;
                            chr = fgetc(fp);

                            if (chr == '"')
                            {
                                count++;
                            }
                        }
                        string[i] = chr;
                        i++;
                        count = 0;
                    }
                    else if ((chr == ' ') || (chr == '\t') || (chr == '\r') || (chr == '\n'))
                    {
                        continue;
                    }
                    else
                    {
                        string[i] = chr;
                        i++;
                    }
                }

                string[i] = chr;
                i = 0;
                string[strlen(string)-1] = '\0';

                elementList = make2DArray(200, 256);
                token = strtok(string, ",");

                while (token != NULL)
                {
                    strcpy(elementList[i], token);
                    i++;

                    token = strtok(NULL, ",");
                }
                parseTag(tag, elementList);
                destroy2DArray(elementList, 200);
            }
        }
        printf("</body>\n");
        printf("</html>\n");
    }
    else
    {
        printf("File not found\n");
        exit(0);
    }
}

/**
 *parseTag
 *Prints html code to stdout depending on attributes given
 *
 *IN: char, char**
 *RETURN: N/A
 */
void parseTag(char tag, char** elementList)
{
    switch(tag)
    {
        case 'b': //Button Form
            {
                char name[256] = "ButtonName\0";
                char link[256] = "index.php\0";
                char** extra = make2DArray(50, 256);
                int j = 0;

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, "\n");

                    if (second != NULL)
                    {
                        if (strcmp("name", first) == 0)
                        {
                            strcpy(name, second);
                        }
                        else if (strcmp("link", first) == 0)
                        {
                            strcpy(link, second);
                        }
                        else
                        {
                            strcpy(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], "\0");
                            j++;
                        }
                    }
                    else
                    {
                        strcpy(extra[j], first);
                        strcat(extra[j], "\0");
                        j++;
                    }
                }

                printf("<form method=\"POST\" action=%s>\n", link);
                printf("<input type=\"Submit\" name=%s value=%s", name, name);
                for (int i = 0; (strcmp(extra[i], "") != 0); i++)
                {
                    printf(" %s", extra[i]);
                }
                printf(">\n</form>\n");
                destroy2DArray(extra, 50);
            }
            break;
        case 'd': //Horizontal Line
            printf("<hr>\n");
            break;
        case 'e': //Executable tag
            {
                char fileName[256] = "";
                char* second = strtok(elementList[0], "=");
                second = strtok(NULL, "\n");

                strcpy(fileName, "php exec.php ");

                if (second != NULL)
                {
                    strcat(fileName, second);
                    strcat(fileName, "\0");
                }

                FILE* file = fopen("./exec.php", "w");
                fprintf(file, "<?php\n");
                fprintf(file, "if (file_exists(\"./\" . $argv[1]))\n{\n");
                fprintf(file, "$handle = popen(\"./\" . $argv[1] . \" 2>&1\", \"r\");\n");
                fprintf(file, "pclose($handle);\n}\n");
                fprintf(file, "else if (file_exists(\"./bin/\" . $argv[1]))\n{\n");
                fprintf(file, "$handle = popen(\"./bin/\" . $argv[1] . \" 2>&1\", \"r\");\n");
                fprintf(file, "pclose($handle);\n}\n");
                fprintf(file, "else if (file_exists(\"/bin/\" . $argv[1]))\n{\n");
                fprintf(file, "$handle = popen(\"/bin/\" . $argv[1] . \" 2>&1\", \"r\");\n");
                fprintf(file, "pclose($handle);\n}\n");
                fprintf(file, "?>\n");
                fclose(file);

                system(fileName);
            }
            break;
        case 'h': //Heading
            {
                char text[256] = "HEADING\0";
                char size[2] = "3\0";
                char** extra = make2DArray(50, 256);
                int j = 0;

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, "\n");

                    if (second != NULL)
                    {
                        if (strcmp("text", first) == 0)
                        {
                            second++;
                            strcpy(text, second);
                            text[strlen(text)-1] = '\0';
                        }
                        else if (strcmp("size", first) == 0)
                        {
                            strcpy(size, second);
                            strcat(size, "\0");
                        }
                        else
                        {
                            strcpy(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], "\0");
                            j++;
                        }
                    }
                    else
                    {
                        strcpy(extra[j], first);
                        strcat(extra[j], "\0");
                        j++;
                    }
                }

                printf("<h%s", size);
                for (int i = 0; (strcmp(extra[i], "") != 0); i++)
                {
                    printf(" %s", extra[i]);
                }
                printf(">%s</h%s>\n", text, size);
                destroy2DArray(extra, 50);
            }
            break;
        case 'i': //Text input form
            {
                char fileName[256] = "";
                char buttonText[256] = "\"Submit\"\0";
                int j = 0;
                int k = 0;
                char** extra = make2DArray(50, 256);
                char** misc = make2DArray(50, 256);

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, "\n");

                    if (second != NULL)
                    {
                        if (strcmp("bText", first) == 0)
                        {
                            strcpy(buttonText, second);
                            strcat(buttonText, "\0");
                        }
                        else if (strcmp("action", first) == 0)
                        {
                            strcpy(fileName, second);
                            strcat(fileName, "\0");
                        }
                        else if (strcmp("text", first) == 0)
                        {
                            second++;
                            strcpy(extra[j], second);
                            extra[j][strlen(extra[j])-1] = '\0';
                            strcpy(extra[j], " <input type=\"text\" ");
                        }
                        else if (strcmp("name", first) == 0)
                        {
                            strcat(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], " \0");
                        }
                        else if (strcmp("value", first) == 0)
                        {
                            strcat(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], " \0");
                            j++;
                        }
                        else
                        {
                            strcpy(misc[k], first);
                            strcat(misc[k], "=");
                            strcat(misc[k], second);
                            strcat(misc[k], "\0");
                            k++;
                        }
                    }
                    else
                    {
                        strcpy(misc[k], first);
                        strcat(misc[k], "\0");
                        k++;
                    }
                }

                printf("<form method=\"%s\" action=%s>\n", "POST", fileName);

                for (int i = 0; (strcmp(extra[i], "") != 0); i++)
                {
                    printf("%s", extra[i]);
                    for (int i = 0; (strcmp(misc[i], "") != 0); i++)
                    {
                        printf(" %s", misc[i]);
                    }
                    printf(">\n");
                }

                printf("<input type=\"Submit\" name=%s value=%s>\n", buttonText, buttonText);
                printf("</form>\n");
                destroy2DArray(extra, 50);
                destroy2DArray(misc, 50);
            }
            break;
        case 'l': //link
            {
                char text[256] = "link\0";
                char URL[256] = "\"index.php\"\0";

                char** extra = make2DArray(50, 256);
                int j = 0;

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, "\n");

                    if (second != NULL)
                    {
                        if (strcmp("link", first) == 0)
                        {
                            strcpy(URL, second);
                            strcat(URL, "\0");
                        }
                        else if (strcmp("text", first) == 0)
                        {
                            second++;
                            strcpy(text, second);
                            text[strlen(text)-1] = '\0';
                        }
                        else
                        {
                            strcpy(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], "\0");
                            j++;
                        }
                    }
                    else
                    {
                        strcpy(extra[j], first);
                        strcat(extra[j], "\0");
                        j++;
                    }
                }

                printf("<a href=%s", URL);

                for (int i = 0; (strcmp(extra[i], "") != 0); i++)
                {
                    printf(" %s", extra[i]);
                }

                printf(">%s</a>\n", text);
                destroy2DArray(extra, 50);
            }
            break;
        case 'p': //Picture
            {
                char src[256] = "";
                char width[5] = "100\0";
                char height[5] = "100\0";

                char** extra = make2DArray(50, 256);
                int j = 0;

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, "\n");

                    if (second != NULL)
                    {
                        if (strcmp("image", first) == 0)
                        {
                            strcpy(src, second);
                            strcat(src, "\0");
                        }
                        else if (strcmp("size", first) == 0)
                        {
                            char* token = strtok(second, "x");
                            strcpy(width, token);
                            token = strtok(NULL, "\n");
                            strcpy(height, token);
                        }
                        else
                        {
                            strcpy(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], "\0");
                            j++;
                        }
                    }
                    else
                    {
                        strcpy(extra[j], first);
                        strcat(extra[j], "\0");
                        j++;
                    }
                }

                printf("<img src=%s width=\"%s\" height=\"%s\"", src, width, height);

                for (int i = 0; (strcmp(extra[i], "") != 0); i++)
                {
                    printf(" %s", extra[i]);
                }
                printf(">\n");

                destroy2DArray(extra, 50);
            }
            break;
        case 'r': //Radio button form
            {
                char fileName[256] = "";
                char name[256] = "";

                char** valueList = make2DArray(10, 256);
                char** extra = make2DArray(50, 256);
                int j = 0;
                int k = 0;

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, "\n");

                    if (second != NULL)
                    {
                        if (strcmp("action", first) == 0)
                        {
                            strcpy(fileName, second);
                            strcat(fileName, "\0");
                        }
                        else if (strcmp("name", first) == 0)
                        {
                            strcpy(name, second);
                            strcat(name, "\0");
                        }
                        else if (strcmp("value", first) == 0)
                        {
                            second++;
                            strcpy(valueList[k], second);
                            valueList[k][strlen(valueList[k])-1] = '\0';
                            k++;
                        }
                        else
                        {
                            strcpy(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], "\0");
                            j++;
                        }
                    }
                    else
                    {
                        strcpy(extra[j], first);
                        strcat(extra[j], "\0");
                        j++;
                    }
                }

                printf("<form action=%s method=\"POST\"", fileName);

                for (int i = 0; (strcmp(extra[i], "") != 0); i++)
                {
                    printf(" %s", extra[i]);
                }

                printf(">\n");
                printf("<input type=\"radio\" name=%s value=\"%s\" checked>%s<br>\n", name, valueList[0], valueList[0]);

                for (int i = 1; (strcmp(valueList[i], "") != 0); i++)
                {
                    printf("<input type=\"radio\" name=%s value=\"%s\">%s<br>\n", name, valueList[i], valueList[i]);
                }

                printf("<input type=\"Submit\" value=%s>\n", "Submit");
                printf("</form>\n");

                destroy2DArray(valueList, 10);
                destroy2DArray(extra, 50);
            }
            break;
        case 't': //Text
            {
                char text[10000] = "Default Text\0";
                char* first = strtok(elementList[0], "=");
                char* second = strtok(NULL, "\n");

                if (second != NULL)
                {
                    if (strcmp("file", first) == 0)
                    {
                        second++;
                        second[strlen(second)-1] = '\0';

                        FILE* fp = fopen(second, "r");

                        if (fp != NULL)
                        {
                            while (fgets(text, 10000, fp) != NULL)
                            {
                                printf("%s", text);
                            }
                            printf("\n");
                            fclose(fp);
                        }
                    }
                    else if (strcmp("text", first) == 0)
                    {
                        second++;
                        second[strlen(second)-1] = '\0';
                        printf("%s\n", second);
                    }
                }
            }
            break;
        case 'a':
            {
                char name[256] = "textName\0";
                char rows[10] = "\"200\"\0";
                char cols[10] = "\"200\"\0";

                char** extra = make2DArray(50, 256);
                int j = 0;

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, ",");

                    if (second != NULL)
                    {
                        if (strcmp("name", first) == 0)
                        {
                            strcpy(name, second);
                            strcat(name, "\0");
                        }
                        else if (strcmp("rows", first) == 0)
                        {
                            strcpy(rows, second);
                            strcat(rows, "\0");
                        }
                        else if (strcmp("cols", first) == 0)
                        {
                            strcpy(cols, second);
                            strcat(cols, "\0");
                        }
                        else
                        {
                            strcpy(extra[j], first);
                            strcat(extra[j], "=");
                            strcat(extra[j], second);
                            strcat(extra[j], "\0");
                            j++;
                        }
                    }
                    else
                    {
                        strcpy(extra[j], first);
                        strcat(extra[j], "\0");
                        j++;
                    }
                }

                printf("<textarea name=%s", name);
                for (int i = 0; (strcmp(extra[i], "") != 0); i = i+2)
                {
                    printf(" %s", extra[i]);
                }
                printf(" rows=%s cols=%s>\n", rows, cols);
                destroy2DArray(extra, 50);
            }
            break;

        case 'n': //Custom Tag for general forms with multiple types of input
            {
                char link[256] = "";

                char** extra = make2DArray(500, 500);
                char** garbage = make2DArray(500, 256);
                int j = 0;
                int k = 0;

                for (int i = 0; (strcmp(elementList[i], "") != 0); i++)
                {
                    char* first = strtok(elementList[i], "=");
                    char* second = strtok(NULL, "\n");

                    if (second != NULL)
                    {
                        if (strcmp("link", first) == 0)
                        {
                            strcpy(link, second);
                            strcat(link, "\0");
                        }
                        else if (strcmp("label", first) == 0)
                        {
                            second++;
                            second[strlen(second)-1] = '\0';
                            strcpy(extra[j], second);
                        }
                        else if (strcmp("input", first) == 0)
                        {
                            strcat(extra[j], " <input type=");
                            strcat(extra[j], second);
                        }
                        else if (strcmp("name", first) == 0)
                        {
                            strcat(extra[j], " name=");
                            strcat(extra[j], second);
                        }
                        else if (strcmp("value", first) == 0)
                        {
                            strcat(extra[j], " value=");
                            strcat(extra[j], second);
                            j++;
                        }
                        else
                        {
                            strcpy(garbage[k], first);
                            strcat(garbage[k], "=");
                            strcat(garbage[k], second);
                            strcat(garbage[k], "\0");
                            k++;
                        }
                    }
                    else
                    {
                        strcpy(garbage[k], first);
                        strcat(garbage[k], "\0");
                        k++;
                    }
                }

                printf("<form method=\"%s\" action=%s>\n", "POST", link);

                for (int i = 0; (strcmp(extra[i], "") != 0); i++)
                {
                    printf("%s", extra[i]);

                    for (int j = 0; (strcmp(garbage[j], "") != 0); j++)
                    {
                        printf(" %s", garbage[j]);
                    }
                    printf(">\n");
                }

                printf("</form>\n");
                destroy2DArray(extra, 500);
                destroy2DArray(garbage, 500);
            }
            break;
        case 'S':
            //streamList.py
            break;
        case 'V':
            //view.py
            break;
        case 'P':
            //post.c
            break;
        case 'A':
            //addauthor.c
            break;
    }
}

/**
 *make2DArray
 *Allocates heap memory for 2 dimensional array
 *filling all string with empty strings
 *
 *IN: int, int
 *RETURN: char**
 */
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

/**
 *destroy2DArray
 *Deallocates/frees heap memory for 2D array
 *
 *IN: char**, int
 *RETURN: N/A
 */
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
