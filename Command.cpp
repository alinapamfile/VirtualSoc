#include "Command.h"
#include "Database.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

pthread_mutex_t Command::mutex;

void Command::signUp(char* argv[], int argn, char* response) {
    if (argn != 6) {
        strcpy(response, "\nYou didn't enter enough parameters.\n\n");
    } else {

        //verificam daca exista deja un user cu acelasi username
        if (!Database::userExists(argv[1], response)) {
            return;
        }

        //criptam parola

        //adaugam user-ul in baza de date
        Database::addUser(argv, argn, response);
    }
}

void Command::logIn(char* argv[], int argn, char* response) {
    if (argn != 3) {
        strcpy(response, "\nYou didn't enter enough parameters.\n\n");
    } else {
        User *user = Database::getUser(argv[1], response);

        if (user) {
            if (strcmp(user->password, argv[2]) != 0) {
                strcpy(response, "\nIncorrect password.\n\n");
                return;
            } else {
                strcpy(response, "\nSuccessfully logged in!\n\n");
            }
        }
    }
}