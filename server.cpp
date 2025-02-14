#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

#include "Meniu.h"
#include "Command.h"
#include "Database.h"
#include "Utils.h"

#define SIZE 2024
#define PORT 1024
#define CLIENTS_MAX_NR 50

using namespace std;

pthread_t tid[CLIENTS_MAX_NR];
int counter = 0, clients[50];
char* logged_users[50], *searched_users[50];

void handleLoggedUser(int index);
void handleUnauthUser(int index);
void userOnFriendProfile(int index);
void userOnUserProfile(int index);
void adminOnAdminUserProfile(int index);
void adminOnAdminFriendProfile(int index);
void adminOnFriendProfile(int index);
void adminOnUserProfile(int index);

void userOnFriendProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_user_on_friend_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "remove_friend") == 0) {
            if (Command::removeFriend(logged_users[index], searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                userOnUserProfile(index);
            }
        } else if (strcmp(argv[0], "send_message") == 0) {
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, "\"");
            argv[argn] = new char[SIZE];
            strcpy(argv[argn++], searched_users[index]);
            Command::sendMessage(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "back") == 0) {
            strcpy(result, "\nGoing back on the main menu.\n\n");
            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            handleLoggedUser(index);
        } else {
           strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

void userOnUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_user_on_user, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if(strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "add_friend") == 0) {
            if (Command::addFriend(logged_users[index], searched_users[index], "friend", argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                userOnFriendProfile(index);
            }
        } else if (strcmp(argv[0], "add_close_friend") == 0) {
            if (Command::addFriend(logged_users[index], searched_users[index], "close_friend", argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                userOnFriendProfile(index);
            }
        } else if (strcmp(argv[0], "send_message") == 0) {
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, "\"");
            argv[argn] = new char[SIZE];
            strcpy(argv[argn++], searched_users[index]);
            Command::sendMessage(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "back") == 0) {
            strcpy(result, "\nGoing back on the main menu.\n\n");
            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            handleLoggedUser(index);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

void adminOnAdminUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_admin_on_admin_user_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "add_friend") == 0) {
            if (Command::addFriend(logged_users[index], searched_users[index], "friend", argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnAdminFriendProfile(index);
            }
        } else if (strcmp(argv[0], "add_close_friend") == 0) {
            if (Command::addFriend(logged_users[index], searched_users[index], "close_friend", argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnAdminFriendProfile(index);
            }
        } else if (strcmp(argv[0], "send_message") == 0) {
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, "\"");
            argv[argn] = new char[SIZE];
            strcpy(argv[argn++], searched_users[index]);
            Command::sendMessage(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_post") == 0) {
            Command::deletePost(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_user") == 0) {
            if (Command::deleteAccount(searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                pthread_exit(0);
            }
        } else if (strcmp(argv[0], "back") == 0) {
            strcpy(result, "\nGoing back on the main menu.\n\n");
            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            handleLoggedUser(index);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

void adminOnAdminFriendProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_admin_on_admin_friend_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "remove_friend") == 0) {
            if (Command::removeFriend(logged_users[index], searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnAdminUserProfile(index);
            }
        } else if (strcmp(argv[0], "send_message") == 0) {
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, "\"");
            argv[argn] = new char[SIZE];
            strcpy(argv[argn++], searched_users[index]);
            Command::sendMessage(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_post") == 0) {
            Command::deletePost(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_user") == 0) {
            if (Command::deleteAccount(searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                pthread_exit(0);
            }
        } else if (strcmp(argv[0], "back") == 0) {
            strcpy(result, "\nGoing back on the main menu.\n\n");
            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            handleLoggedUser(index);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

void adminOnFriendProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_admin_on_friend_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "remove_friend") == 0) {
            if (Command::removeFriend(logged_users[index], searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnUserProfile(index);
            }
        } else if (strcmp(argv[0], "send_message") == 0) {
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, "\"");
            argv[argn] = new char[SIZE];
            strcpy(argv[argn++], searched_users[index]);
            Command::sendMessage(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "make_admin") == 0) {
            if (Command::makeAdmin(searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnAdminFriendProfile(index);
            }
        } else if (strcmp(argv[0], "delete_post") == 0) {
            Command::deletePost(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_user") == 0) {
            if (Command::deleteAccount(searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                pthread_exit(0);
            }
        } else if (strcmp(argv[0], "back") == 0) {
            strcpy(result, "\nGoing back on the main menu.\n\n");
            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            handleLoggedUser(index);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

void adminOnUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_admin_on_user_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "add_friend") == 0) {
            if (Command::addFriend(logged_users[index], searched_users[index], "friend", argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnFriendProfile(index);
            }
        } else if (strcmp(argv[0], "add_close_friend") == 0) {
            if (Command::addFriend(logged_users[index], searched_users[index], "close_friend", argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnFriendProfile(index);
            }
        } else if (strcmp(argv[0], "send_message") == 0) {
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, "\"");
            argv[argn] = new char[SIZE];
            strcpy(argv[argn++], searched_users[index]);
            Command::sendMessage(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "make_admin") == 0) {
            if (Command::makeAdmin(searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                adminOnAdminUserProfile(index);
            }
        } else if (strcmp(argv[0], "delete_post") == 0) {
            Command::deletePost(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_user") == 0) {
            if (Command::deleteAccount(searched_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                pthread_exit(0);
            }
        } else if (strcmp(argv[0], "back") == 0) {
            strcpy(result, "\nGoing back on the main menu.\n\n");
            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            handleLoggedUser(index);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

void unauthUserOnUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_unauth_user_on_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "back") == 0) {
            strcpy(result, "\nGoing back on the main menu.\n\n");
            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            handleUnauthUser(index);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

//functionalitatile din meniul principal al utilizatorului nelogat
void handleUnauthUser(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_unauth_user, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "search_user") == 0) {
            if (Command::searchUser(argv, argn, result)) {
                searched_users[index] = new char[50];
                //retinem user-ul cautat de client
                strcpy(searched_users[index], argv[1]);

                strcpy(result, "\nYou are now on ");
                strcat(result, searched_users[index]);
                strcat(result, "'s profile.\n\n");

                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                unauthUserOnUserProfile(index);
            }
        } else if (strcmp(argv[0], "quit") == 0) {
            strcpy(result, "\nHope to see you back soon!\n\n");

            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            clients[index] = -1;
            pthread_exit(0);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

//functionalitatile din meniul principal al utilizatorului logat
void handleLoggedUser(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        //trimite meniul disponibil clientului
        if (send(clients[index], Meniu::meniu_auth_user, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //primeste cereri de comenzi de la client
        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        //parsam input-ul
        Utils::inputParse(response, argv, argn);

        //se executa comanda aleasa de client
        if (strcmp(argv[0], "search_user") == 0) {
            if (Command::searchUser(argv, argn, result)) {
                searched_users[index] = new char[50];
                //retinem user-ul cautat de client
                strcpy(searched_users[index], argv[1]);

                strcpy(result, "\nYou are now on ");
                strcat(result, searched_users[index]);
                strcat(result, "'s profile.\n\n");

                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                User *user1 = Database::getUser(logged_users[index], result);
                User *user2 = Database::getUser(searched_users[index], result);
                int code = Utils::isFriend(Database::db, logged_users[index], searched_users[index], NULL, result);

                if (code == 1) {
                    if (user1->isAdmin) {
                        if (user2->isAdmin) {
                            adminOnAdminFriendProfile(index);
                        } else {
                            adminOnFriendProfile(index);
                        }
                    } else {
                        userOnFriendProfile(index);
                    }
                } else if (code == 0) {
                    if (user1->isAdmin) {
                        if (user2->isAdmin) {
                            adminOnAdminUserProfile(index);
                        } else {
                            adminOnUserProfile(index);
                        }
                    } else {
                        userOnUserProfile(index);
                    }
                }
            }
        } else if (strcmp(argv[0], "share_post") == 0) {
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, " ");
            argv[argn++] = strtok(NULL, "\"");
            Command::sharePost(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_post") == 0) {
            Command::deletePost(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "send_message") == 0) {
            char *word;
            argn = 0;
            argv[argn++] = strtok(copy, " ");
            argv[argn++] = strtok(NULL, "\"");
            while((word = strtok(NULL, " ")) != NULL) {
                argv[argn++] = word;
            }
            Command::sendMessage(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "see_unread_messages") == 0) {
            Command::seeUnreadMessages(logged_users[index], argn, result);
        } else if (strcmp(argv[0], "edit_profile") == 0) {
            Command::editProfile(logged_users[index], argv, argn, result);
        } else if (strcmp(argv[0], "delete_account") == 0) {
            if (Command::deleteAccount(logged_users[index], argn, result)) {
                if (send(clients[index], result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                pthread_exit(0);
            }
        } else if (strcmp(argv[0], "log_out") == 0) {
            strcpy(result, "\nHope to see you back soon!\n\n");

            if (send(clients[index], result, SIZE, 0) == -1) {
                cout << "[server] Error at send().\n";
                fflush(stdout);
                continue;
            }

            clients[index] = -1;
            pthread_exit(0);
        } else {
            strcpy(result, "\nUnknown command\n\n");
        }

        //trimitem clientului raspunsul la comanda sa
        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }
    }
}

//functionalitatile din meniul de autentificare
void* authentication(void* arg) {
     char response[SIZE];
     int index = *((int *)arg), argn, code;
     char* argv[10], *result = new char[SIZE];

     while (true) {
         //trimite meniul disponibil clientului
         if (send(clients[index], Meniu::meniu_auth, SIZE, 0) == -1) {
             cout << "[server] Error at send().\n";
             fflush(stdout);
             continue;
         }

         //primeste cereri de comenzi de la client
         if (recv(clients[index], &response, SIZE, 0) == -1) {
             cout << "[server] Error at recv().\n";
             fflush(stdout);
             continue;
         }

         //parsam input-ul
         Utils::inputParse(response, argv, argn);

         //se executa comanda aleasa de client
         if (strcmp(argv[0], "sign_up") == 0) {
             if (Command::signUp(argv, argn, result)) {
                 logged_users[index] = new char[50];
                 //memoram username-ul clientului autentificat
                 strcpy(logged_users[index], argv[1]);

                 if (send(clients[index], result, SIZE, 0) == -1) {
                     cout << "[server] Error at send().\n";
                     fflush(stdout);
                     continue;
                 }

                 handleLoggedUser(index);
             }
         } else if (strcmp(argv[0], "log_in") == 0) {
             if (Command::logIn(argv, argn, result)) {
                 logged_users[index] = new char[50];
                 //memoram username-ul clientului autentificat
                 strcpy(logged_users[index], argv[1]);

                 if (send(clients[index], result, SIZE, 0) == -1) {
                     cout << "[server] Error at send().\n";
                     fflush(stdout);
                     continue;
                 }

                 handleLoggedUser(index);
             }
         } else if (strcmp(argv[0], "continue") == 0) {
             strcpy(result, "You are using the app without being authenticated.\n");

             if (send(clients[index], result, SIZE, 0) == -1) {
                 cout << "[server] Error at send().\n";
                 fflush(stdout);
                 continue;
             }

             logged_users[index] = NULL;
             handleUnauthUser(index);
         } else if (strcmp(argv[0], "quit") == 0) {
             strcpy(result, "\nHope to see you back soon!\n\n");

             if (send(clients[index], result, SIZE, 0) == -1) {
                 cout << "[server] Error at send().\n";
                 fflush(stdout);
                 continue;
             }

             clients[index] = -1;
             pthread_exit(0);
         } else {
             strcpy(result, "\nUnknown command\n\n");
         }

         //trimitem clientului raspunsul la comanda sa
         if (send(clients[index], result, SIZE, 0) == -1) {
             cout << "[server] Error at send().\n";
             fflush(stdout);
             continue;
         }
     }
}

int main() {
    struct sockaddr_in server{};
    struct sockaddr_in client{};
    int sd, child, option = 1, clientd;

    for (int i = 0; i < CLIENTS_MAX_NR; i++)
        clients[i] = -1;

    //deschidem baza de date
    if (sqlite3_open("db", &Database::db) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_open().\n";
        fflush(stdout);
        exit(1);
    }

    //crearea unui socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "[server] Error at socket().\n";
        fflush(stdout);
        exit(1);
    }

    //setam optiunile socket-ului
    if (setsockopt(sd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char*)&option, sizeof(option)) < 0)
    {
        cout << "Error at setsockopt().\n";
        fflush(stdout);
        exit(1);
    }

    //stabilirea familiei de socket-uri
    server.sin_family = AF_INET;
    //acceptam orice adresa
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    //utilizam un port utilizator
    server.sin_port = htons (PORT);

    //atasam socket-ul
    if (bind(sd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1) {
        cout << "[server] Error at bind().\n";
        fflush(stdout);
        exit(1);
    }

    //serverul asculta daca vin clienti sa se conecteze
    if (listen(sd, CLIENTS_MAX_NR) == -1) {
        cout << "[server] Error at listen().\n";
        fflush(stdout);
        exit(1);
    }

    //servirea concurenta a clientilor
    while (true) {
        int length = sizeof (struct sockaddr);

        //acceptam un client
        clientd = accept(sd, (struct sockaddr*) &client, (socklen_t *) &length);

        if (clientd == -1) {
            cout << "[server] Error at accept().\n";
            fflush(stdout);
            continue;
        }

        //cautam un index liber pentru a memora descriptorul clientului si thread id-ul corespunzator
        while(clients[counter] != -1) {
            if (counter == CLIENTS_MAX_NR) {
                counter = 0;
            } else {
                counter++;
            }
        }

        //retinem in tabloul clients descriptorul clientului curent
        clients[counter++] = clientd;

        int index = counter - 1;
        //cream un nou thread
        if (pthread_create(&(tid[counter - 1]), NULL, authentication, &index) != 0) {
            cout << "[server] Error at pthread_create().\n";
            fflush(stdout);
            continue;
        }
    }
}