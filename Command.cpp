#include "Command.h"
#include "Database.h"
#include "Utils.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define SIZE 2024

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
        argv[argn++] = Utils::getCurrentDate();
        argv[argn++] = Utils::getCurrentTime();

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
                if (Database::deletePost(argv[1], response))
                    strcpy(response, "\nPost deleted successfully!\n\n");
            }
        } else {
            if (post) {
                if (strcmp(post->username, username) == 0) {
                    if (Database::deletePost(argv[1], response)) {
                        strcpy(response, "\nPost deleted successfully!\n\n");
                    }
                } else {
                    strcpy(response, "\nYou don't have a post with this id.\n\n");
                }
            }
        }
    }
}

void Command::sendMessage(char *username, char *argv[], int argn, char *response) {
    if (argn < 3) {
        strcpy(response, "\nYou didn't enter the parameters needed.\n\n");
    } else {
        User *user;
        char *args[5], *res = new char[SIZE];
        for (int i = 0; i < 5; i++)
            args[i] = new char[SIZE];

        for (int i = 2; i < argn; i++) {
            user = Database::getUser(argv[i], res);
            if (user) {
                strcpy(args[0], username);
                strcpy(args[1], argv[i]);
                strcpy(args[2], argv[1]);
                strcpy(args[3], Utils::getCurrentDate());
                strcpy(args[4], Utils::getCurrentTime());
                if (Database::addMessage(args, 5, res))
                    strcat(response, "\nMessage sent.\n\n");
                else
                    strcat(response, res);
            } else {
                strcat(response, res);
            }
        }
    }
}

void Command::seeUnreadMessages(char *username, int argn, char *response) {
    if (argn != 1) {
        strcpy(response, "\nYou entered too many parameters.\n\n");
    } else {
        Message *messages[50];
        int count;
        if (Database::getMessages(username, messages, count, response)) {
            if (count != 0) {
                strcpy(response, "\nYour messages:\n\n");
                for (int i = 0; i < count; i++) {
                    strcat(response, "Date: "); strcat(response, messages[i]->date);
                    strcat(response, "\nTime: "); strcat(response, messages[i]->time);
                    strcat(response, "\nFrom: "); strcat(response, messages[i]->sender);
                    strcat(response, "\nMessage: "); strcat(response, messages[i]->content);
                    strcat(response, "\n\n");
                }
            } else {
                strcpy(response, "\nYou don't have any new messages.\n\n");
            }
        }
    }
}

void Command::seeUserDetails(char *username, char *searchedUser, int argn, char *response) {
    if (argn != 1) {
        strcpy(response, "\nYou entered too many parameters.\n\n");
    } else {
        User *user1 = Database::getUser(username, response);
        User *user2 = Database::getUser(searchedUser, response);
        int is_friend = Utils::isFriend(Database::db, username, searchedUser, "friend", response);
        int is_close_friend = Utils::isFriend(Database::db, username, searchedUser, "close_friend", response);

        if (strcmp(user2->profileVisibility, "public") == 0 || (user1 != NULL && user1->isAdmin) || is_friend == 1 || is_close_friend == 1) {
            strcpy(response, "Username: "); strcat(response, user2->username);
            strcat(response, "\nFirstname: "); strcat(response, user2->firstname);
            strcat(response, "\nLastname: "); strcat(response, user2->lastname);
            strcat(response, "\nCountry: "); strcat(response, user2->country);
            strcat(response, "\nCity: "); strcat(response, user2->city);
            strcat(response, "\nOccupation: "); strcat(response, user2->occupation);
            strcat(response, "\n\n");
        } else {
            strcpy(response, "Username: "); strcat(response, user2->username);
            strcat(response, "\nFirstname: "); strcat(response, user2->firstname);
            strcat(response, "\nLastname: "); strcat(response, user2->lastname);
            strcat(response, "\n\n");
        }
    }
}

void Command::seeUserPosts(char *username, char *searchedUser, int argn, char *response) {
    if (argn != 1) {
        strcpy(response, "\nYou entered too many parameters.\n\n");
    } else {
        User *user1 = Database::getUser(username, response);
        User *user2 = Database::getUser(searchedUser, response);
        int is_friend = Utils::isFriend(Database::db, username, searchedUser, "friend", response);
        int is_close_friend = Utils::isFriend(Database::db, username, searchedUser, "close_friend", response);

        char *visibility = new char[SIZE];
        if (user1 == NULL || (!is_friend && !is_close_friend && !user1->isAdmin)) {
            strcpy(visibility, "AND postVisibility='public'");
        } else if (user1->isAdmin || is_close_friend) {
            visibility = NULL;
        } else {
            strcpy(visibility, "AND (postVisibility='public' OR postVisibility='friend')");
        }

        Post *posts[50];
        int count;

        if (Database::getUserPosts(searchedUser, visibility, posts, count, response)) {
            if (count != 0) {
                strcpy(response, "\n");
                strcat(response, searchedUser);
                strcat(response, "'s posts:\n\n");
                for (int i = 0; i < count; i++) {
                    strcat(response, "Id: "); strcat(response, to_string(posts[i]->id).c_str());
                    strcat(response, "\nDate: "); strcat(response, posts[i]->date);
                    strcat(response, "\nTime: "); strcat(response, posts[i]->time);
                    strcat(response, "\nContent: "); strcat(response, posts[i]->content);
                    strcat(response, "\n\n");
                }
            } else {
                strcpy(response, "\nUser doesn't have any posts.\n\n");
            }
        }
    }
}