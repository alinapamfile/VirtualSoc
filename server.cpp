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
int counter = 0, connectedClients = 0, clients[50];
char* logged_users[50], *searched_users[50];

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
        if (send(clients[index], Meniu::meniu_user_on_friend_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        Utils::inputParse(response, argv, argn);

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
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "log_out") == 0) {
            pthread_exit(0);
        }
    }
}

void userOnUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_user_on_user, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        Utils::inputParse(response, argv, argn);

        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if(strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
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
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "log_out") == 0) {
            pthread_exit(0);
        }
    }
}

void adminOnAdminUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_admin_on_admin_user_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        Utils::inputParse(response, argv, argn);

        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
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
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "log_out") == 0) {
            pthread_exit(0);
        }
    }
}

void adminOnAdminFriendProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_admin_on_admin_friend_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        Utils::inputParse(response, argv, argn);

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
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "log_out") == 0) {
            pthread_exit(0);
        }
    }
}

void adminOnFriendProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_admin_on_friend_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        Utils::inputParse(response, argv, argn);

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
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "log_out") == 0) {
            pthread_exit(0);
        }
    }
}

void adminOnUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_admin_on_user_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        Utils::inputParse(response, argv, argn);

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
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "log_out") == 0) {
            pthread_exit(0);
        }
    }
}

void unauthUserOnUserProfile(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_unauth_user_on_profile, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }

        Utils::inputParse(response, argv, argn);

        if (strcmp(argv[0], "see_user_details") == 0) {
            Command::seeUserDetails(logged_users[index], searched_users[index], argn, result);
        } else if (strcmp(argv[0], "see_user_posts") == 0) {
            Command::seeUserPosts(logged_users[index], searched_users[index], argn, result);
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

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
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_unauth_user, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }

        Utils::inputParse(response, argv, argn);

        if (strcmp(argv[0], "search_user") == 0) {
            if (Command::searchUser(argv, argn, result)) {
                searched_users[index] = new char[50];
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
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "quit") == 0) {
            pthread_exit(0);
        }
    }
}

//functionalitatile din meniul principal al utilizatorului logat
void handleLoggedUser(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_auth_user, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        if (recv(clients[index], &response, SIZE, 0) == -1) {
            cout << "[server] Error at recv().\n";
            fflush(stdout);
            continue;
        }
        strcpy(copy, response);

        Utils::inputParse(response, argv, argn);

        if (strcmp(argv[0], "search_user") == 0) {
            if (Command::searchUser(argv, argn, result)) {
                searched_users[index] = new char[50];
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
                            continue;
                        } else {
                            adminOnFriendProfile(index);
                            continue;
                        }
                    } else {
                        userOnFriendProfile(index);
                        continue;
                    }
                } else if (code == 0) {
                    if (user1->isAdmin) {
                        if (user2->isAdmin) {
                            adminOnAdminUserProfile(index);
                            continue;
                        } else {
                            adminOnUserProfile(index);
                            continue;
                        }
                    } else {
                        userOnUserProfile(index);
                        continue;
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
            cout << "h";
            fflush(stdout);
        } else {
            cout << "Unknown command";
            fflush(stdout);
        }

        if (send(clients[index], result, SIZE, 0) == -1) {
            cout << "[server] Error at send().\n";
            fflush(stdout);
            continue;
        }

        //incheie comunicarea
        if (strcmp(argv[0], "log_out") == 0) {
            pthread_exit(0);
        }
    }
}

//functionalitatile din meniul de autentificare
void* authentication(void* arg) {
     char response[SIZE];
     int index = *((int *)arg), argn, code;
     char* argv[10], *result = new char[SIZE];

     //comunica cu clientul pana cand acesta se deconecteaza sau isi sterge contul
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

         Utils::inputParse(response, argv, argn);

         if (strcmp(argv[0], "sign_up") == 0) {
             if (Command::signUp(argv, argn, result)) {
                 logged_users[index] = new char[50];
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
             pthread_exit(0);
         } else {
             cout << "Unknown command";
             fflush(stdout);
         }

         if (send(clients[index], result, SIZE, 0) == -1) {
             cout << "[server] Error at send().\n";
             fflush(stdout);
             continue;
         }

         //incheie comunicarea
         if (strcmp(argv[0], "quit") == 0 || strcmp(argv[0], "log out") == 0 || strcmp(argv[0], "delete account") == 0) {
             pthread_exit(0);
         }
     }
}

int main() {
    struct sockaddr_in server{};
    struct sockaddr_in client{};
    int sd, child, option = 1, clientd;

    if (pthread_mutex_init(&Command::mutex, NULL) != 0) {
        cout << "[server] Error at pthread_mutex_init().\n";
        fflush(stdout);
        exit(1);
    }

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

        if (counter == CLIENTS_MAX_NR) {
            counter = 0;
        }

        ///TODO: decrementeaza cand se deconecteaza un client
        connectedClients++;
        clients[counter++] = clientd;
        //cream un nou thread
        if (pthread_create(&(tid[counter - 1]), NULL, authentication, &counter - 1) != 0) {
            cout << "[server] Error at pthread_create().\n";
            fflush(stdout);
            continue;
        }
    }
}