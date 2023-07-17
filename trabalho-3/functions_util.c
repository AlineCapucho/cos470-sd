#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// Code from: https://stackoverflow.com/a/34957656
int split(const char *txt, char delim, char ***tokens)
{
    int *tklen, *t, count = 1;
    char **arr, *p = (char *) txt;

    while (*p != '\0') {
        if (*p++ == delim) {
            count += 1;
        }
    }
    t = tklen = calloc (count, sizeof (int));
    for (p = (char *) txt; *p != '\0'; p++) {
        *p == delim ? *t++ : (*t)++;
    }
    *tokens = arr = malloc (count * sizeof (char *));
    t = tklen;
    p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
    while (*txt != '\0')
    {
        if (*txt == delim)
        {
            p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
            txt++;
        }
        else *p++ = *txt++;
    }
    free(tklen);
    return count;
}

void print_message_tokens(char** message_tokens) {
    printf("Imprimindo message tokens.\n");
    for (int i = 0; i < 3; ++i) {
        printf("%s\n", message_tokens[i]);
    }
}

void print_header(int** header) {
    printf("Imprimindo header.\n");
    printf("Client_socket: %d.\n", header[0]);
    printf("Pid: %d.\n", header[1]);
    printf("Message_type: %d.\n", header[2]);
    // for (int i = 0; i < 3; ++i) {
    //     printf("%d\n", header[i]);
    // }
}
