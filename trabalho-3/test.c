/* localtime example */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// Declaração de variáveis
int buffer_size = 2000;

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

int main () {
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime );
    timeinfo = localtime(&rawtime);
    printf ("Current local time and date: %s", asctime (timeinfo));

    // char *buff = "this is a test string";
    // printf("%.*s \n", 4, buff + 10);

    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    printf("Current Time : %s\n", time_str);

    printf("%.*s \n", 8, time_str + strlen(time_str) - 13);

    write_log(125, "1");

    return 0;
}