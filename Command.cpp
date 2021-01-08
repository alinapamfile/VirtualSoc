#include "Command.h"
#include "Database.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>

using namespace std;

pthread_mutex_t Command::mutex;

bool Command::signUp(char* argv[], int argn, char* response) {
    if (argn != 6) {
        strcpy(response, "\nYou didn't enter the parameters needed.\n\n");
        return false;
    } else {
        //verificam daca exista deja un user cu acelasi username
        int code = Database::userExists(argv[1], response);

        if (code == -1) {
            return false;
        } else if (code == 1) {
            strcpy(response, "\nUser already exists.\n\n");
            return false;
        }

        //criptam parola

        //adaugam user-ul in baza de date
        if (Database::addUser(argv, argn, response)) {
            strcpy(response, "\nSuccessfully signed up!\n\n");
            return true;
        } else {
            return false;
        }
    }
}

bool Command::logIn(char* argv[], int argn, char* response) {
    if (argn != 3) {
        strcpy(response, "\nYou didn't enter the parameters needed.\n\n");
        return false;
    } else {
        User *user = Database::getUser(argv[1], response);

        if (user) {
            if (strcmp(user->password, argv[2]) != 0) {
                strcpy(response, "\nIncorrect password.\n\n");
                return false;
            } else {
                strcpy(response, "\nSuccessfully logged in!\n\n");
                return true;
            }
        } else {
            return false;
        }
    }
}

void Command::editProfile(char* username, char* argv[], int argn, char* response) {
    if (argn != 3) {
        strcpy(response, "\nYou didn't enter the parameters needed.\n\n");
    } else {
        if (Database::updateUser(username, argv[1], argv[2], response)) {
            strcpy(response, "\nProfile edited successfully!\n\n");
        }
    }
}

bool Command::searchUser(char *argv[], int argn, char *response) {
    if (argn != 2) {
        strcpy(response, "\nYou didn't enter the parameters needed.\n\n");
        return false;
    } else {
        int code = Database::userExists(argv[1], response);

        if (code == -1) {
            return false;
        } else if (code == 1) {
            return true;
        } else {
            strcpy(response, "\nUser doesn't exist.\n");
            char *users[50];
            int count = 0;
            char *pattern = new char[50];

            strcpy(pattern, "%");
            strcat(pattern, argv[1]);
            strcat(pattern, "%");
            if (!Database::getMatchingUsers(pattern, users, count, response)) {
                return false;
            }

            if (count != 0) {
                strcat(response, "Users you could be looking for:\n");
                for (int i = 0; i < count; i++) {
                    strcat(response, users[i]);
                    strcat(response, "\n");
                }
                strcat(response, "\n");
                return false;
            } else {
                strcat(response, "\n");
                return false;
            }
        }
    }
}

void Command::sharePost(char *username, char *argv[], int argn, char *response) {
    if (argn != 3) {
        strcpy(response, "\nYou didn't enter the parameters needed.\n\n");
    } else {
        time_t rawtime;
        struct tm * timeinfo;

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        argv[argn] = new char[50];
        argv[argn + 1] = new char[50];

        strftime(argv[argn++],50,"%d-%m-%Y",timeinfo);
        strftime(argv[argn++],50,"%H:%M",timeinfo);

        if (Database::addPost(username, argv, argn, response)) {
            strcpy(response, "\nPost shared successfully!\n\n");
        }
    }
}

void Command::deletePost(char *username, char *argv[], int argn, char *response) {
    if (argn != 2) {
        strcpy(response, "\nYou didn't enter the parameters needed.\n\n");
    } else {
        User *user = Database::getUser(username, response);
        Post *post = Database::getPost(argv[1], response);

        if (user->isAdmin) {
            if (post) {
                if (Database::deletePost(username, argv[1], response))
                    strcpy(response, "\nPost deleted successfully!\n\n");
            }
        } else {
            if (post) {
                if (strcmp(post->username, username) == 0) {
                    if (Database::deletePost(username, argv[1], response)) {
                        strcpy(response, "\nPost deleted successfully!\n\n");
                    }
                } else {
                    strcpy(response, "\nYou don't have a post with this id.\n\n");
                }
            }
        }
    }
}