/* localtime example */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 2000

// Declaração de variáveis
int buffer_size = 1024;

int message_type_size = 8;

void write_log(int pid, char* message) {
    char filename[] = "log.txt";
    FILE* ptr;
    ptr = fopen(filename, "a");

    if (ptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char pid_str[buffer_size];
    snprintf(pid_str, buffer_size, "%d", pid); // copy int pid to char pid_str

    char message_type[message_type_size];
    if (message == "1") {
        strcpy(message_type, "REQUEST");
    } else if (message == "2") {
        strcpy(message_type, "GRANT");
    } else if (message == "3") {
        strcpy(message_type, "RELEASE");
    }

    time_t current_time = time(NULL);
    char * current_time_str = ctime(&current_time);
    current_time_str[strlen(current_time_str)-1] = '\0';
    printf("Current Time : %s\n", current_time_str);

    fprintf(ptr, "Hora: %.*s; ", 8, current_time_str + strlen(current_time_str) - 13);
    fprintf(ptr, "Mensagem: %s; ", message_type);
    fprintf(ptr, "Processo: %s\n", pid_str);
    fclose(ptr);
}

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
    free (tklen);
    return count;
}

int main () {
    // time_t rawtime;
    // struct tm * timeinfo;

    // time(&rawtime );
    // timeinfo = localtime(&rawtime);
    // printf ("Current local time and date: %s", asctime (timeinfo));

    // time_t mytime = time(NULL);
    // char * time_str = ctime(&mytime);
    // time_str[strlen(time_str)-1] = '\0';
    // printf("Current Time : %s\n", time_str);

    // printf("%.*s \n", 8, time_str + strlen(time_str) - 13);

    // write_log(125, "1");

    // char phrase[20] = "Hello World";

    // char *token = strtok(phrase, " ");
    // printf("%s\n", token); //printing the token

    // char **tokens;
    // int count, i;
    // const char *str = "JAN,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC";

    // count = split(str, ',', &tokens);
    // for (i = 0; i < count; i++) {
    //     printf ("%s\n", tokens[i]);
    //     printf ("%s\n", tokens[i]);
    // }

    // for (i = 0; i < count; i++) {
    //     printf ("%s\n", tokens[i]);
    // }

    // /* freeing tokens */
    // for (i = 0; i < count; i++) {
    //     free (tokens[i]);
    // }
    // free (tokens);

    // return 0;

    // char file_frame[100] = "shots";
    // char fout[100];
    // sprintf((char*)fout,"%s%0*d", file_frame, 10 - strlen(file_frame), 0);

    // char message[10] = "2|";
    // strcat(message, "251");
    // sprintf((char*)message,"%s%0*d", message, 10 - strlen(message), 0);

    // printf("%s\n", file_frame);
    // printf("%s\n", fout);
    // printf("%s\n", message);

    // int requestCode = 1;
    // int pid = 14;
    // char separator[] = "|";
    // char* messageBuffer = malloc(BUFFER_SIZE);
    // memset(messageBuffer, '0', BUFFER_SIZE);

    // char requestCode_str[10];
    // snprintf(requestCode_str, 10, "%d", requestCode); // copy int x to char y
    // printf("Point 0.\n");
    // char pid_str[10];
    // snprintf(pid_str, 10, "%d", pid); // copy int x to char y
    // printf("Point 1.\n");
    // char* message = malloc(10);
    // printf("Point 2.\n");
    // strcat(message, requestCode_str);
    // printf("Point 3.\n");
    // strcat(message, separator);
    // printf("Point 4.\n");
    // strcat(message, pid_str);
    // strcat(message, separator);
    // printf("Point 5.\n");
    // sprintf((char*)message,"%s%0*d", message, 10 - strlen(message), 0);
    // printf("Point 6.\n");

    // printf("%s\n", message);

    // char num[] = "1";
    // int n = atoi(num);
    // printf("%d\n", n);

    int pid = 125421;
    char pid_str[10];
    snprintf(pid_str, 10, "%d", pid); // copy int x to char y
    printf("%s\n", pid_str);
}