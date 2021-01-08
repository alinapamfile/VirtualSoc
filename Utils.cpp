#include "Utils.h"
#include <string.h>

void Utils::inputParse(char response[], char* argv[], int &argn) {
    char *word;
    argn = 0;
    word = strtok(response, " ");
    while(word != NULL) {
        argv[argn++] = word;
        word = strtok(NULL, " ");
    }
}

char* Utils::getCurrentDate() {
    time_t rawtime;
    struct tm * timeinfo;
    char *currentdate = new char[50];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(currentdate,50,"%d-%m-%Y",timeinfo);
    return currentdate;
}

char* Utils::getCurrentTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char *currenttime = new char[50];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(currenttime,50,"%H:%M",timeinfo);
    return currenttime;
}