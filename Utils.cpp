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