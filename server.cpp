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

//User* currentUser = new User("alinapamfile", "ubh21kfg4", "Alina", "Pamfile", "Romania",
//                             "Iasi", "student", "public", "yes");
//
//string searchedUsername = "mariapopa";
//
//int isAvailable(string a) { return 1; }
//
//char* encryptPassword(string pass) {
//    char *password = (char*)malloc(SIZE);
//    strcpy(password, "ubh21kfg4");
//    return password;
//}
//
//int userExists(string username) { return 0; }
//
//void getUserMatches(string username, string matches[], int &nr) {
//    matches[0] = "alinap";
//    matches[1] = "alinapamfile";
//    nr = 2;
//}
//
//User* getUser(string username) {
//    User* user = new User("alinapamfile", "ubh21kfg4", "Alina", "Pamfile", "Romania",
//                         "Iasi", "student", "public", "yes");
//    return user;
//}
//
//int updateUser(string username, string field, string newValue) { return 1; }
//int addFriendship(string username1, string username2, string type) { return 1; }
//int deleteFriendship(string username1, string username2) { return 1; }
//int deleteUser(string username) { return 1; }
//int deletePost2 (int id) { return 1; }
//void deleteMessages(string username) {}
//int addMessage(string content, string username1, string username2) { return 1; }
//
//Post* getPost(int id) {
//    Post* post = new Post();
//    post->id = "1";
//    post->username = "alinapamfile";
//    post->content = "Continutul postarii.";
//    post->date = "13.09.2020";
//    post->time = "15:42";
//    post->postVisibility = "private";
//    return post;
//}
//
//int isValidField(string field) { return 1; }
//int isFriend(string user1, string user2) { return 1; }
//int isCloseFriend(string user1, string user2) { return 1; }
//int addUser(string info[8]) { return 1; }
//int addPost(string content, string visibility) { return 1; }

//     //verifica daca user-ul exista
//     if (userExists(username)) {
//         cout << "[server] Username doesn't exist.\n";
//         fflush(stdout);
//         return -1;
//     }
//
//     //extrage user-ul din baza de date
//     User* user = getUser(username);
//
//     //verifica daca parola introdusa este corecta
//     if (user->password != encryptPassword(move(password))) {
//         cout << "[server] Password incorrect.\n";
//         fflush(stdout);
//         return -1;
//     }
//
//     currentUser = user;

//
// void editProfile(const string& field, string newValue) {
//     //verifica daca field-ul este valid (daca exista in baza de date)
//     if (!isValidField(field)) {
//         cout << "[server] Invalid field to edit.\n";
//         fflush(stdout);
//         return;
//     }
//
//     //user-ul nu isi poate edita username-ul
//     if (field == "username") {
//         cout << "[server] You can't edit your username.\n";
//         fflush(stdout);
//         return;
//     }
//
//     //daca user-ul vrea sa iti schimbe parola, aceasta va fi encriptata
//     if (field == "password") {
//         newValue = encryptPassword(newValue);
//     }
//
//     //updateaza user-ul in baza de date
//     if (updateUser(currentUser->username, field, move(newValue))) {
//         cout << "[server] Couldn't edit profile.\n";
//         fflush(stdout);
//         return;
//     }
//
//     //extrage user-ul modificat din baza de date
//     currentUser = getUser(currentUser->username);
//
//     cout << "[server] Profile has been edited successfully!\n";
//     fflush(stdout);
// }
//
// int searchUser(const string& input) {
//     //verifica daca input-ul este un username existent
//     if (userExists(input)) {
//         searchedUsername = input;
//         return 1;
//     }
//
//     //returneaza in 'matches' username-urile care se potrivesc cu textul cautat
//     string matches[20];
//     int nr = 0;
//     getUserMatches(input, matches, nr);
//
//     //daca nu se gaseste nicio potrivire
//     if (nr == 0) {
//         printf("[server] User %s does not exist. No matches found for '%s'.\n", input.c_str(), input.c_str());
//         fflush(stdout);
//         return -1;
//     }
//
//     printf("[server] User %s does not exist. Matches of '%s':\n", input.c_str(), input.c_str());
//     fflush(stdout);
//
//     //afiseaza potrivirile gasite
//     for (int index = 0; index < nr; index++) {
//         cout << matches[index] << endl;
//         fflush(stdout);
//     }
//
//     return -1;
// }
//
// void userDetails(string username) {
//     //extrage user-ul din baza de date
//     User* user = getUser(move(username));
//
//     //daca user-ul exista
//     if (user != nullptr) {
//         //daca persoana are profil public
//         if (user->profileVisibility == "public") {
//             cout << "Username: " + user->username + "\n" + "First name: " + user->firstname + "\n" +
//                     "Last name: " + user->lastname + "\n" + "Country: " + user->country + "\n" +
//                     "City: " + user->city + "\n" + "Occupation: " + user->occupation + "\n";
//             fflush(stdout);
//         } else { //daca persoana are profil privat
//             //daca user-ul este admin sau prieten cu persoana respectiva
//             if (currentUser->isAdmin == "yes" || isFriend(user->username, currentUser->username) ||
//                 isCloseFriend(user->username, currentUser->username)) {
//                 cout << "Username: " + user->username + "\n" + "First name: " + user->firstname + "\n" +
//                         "Last name: " + user->lastname + "\n" + "Country: " + user->country + "\n" +
//                         "City: " + user->city + "\n" + "Occupation: " + user->occupation + "\n";
//                 fflush(stdout);
//             } else {
//                 cout << "[server] Private account.";
//                 fflush(stdout);
//             }
//         }
//     }
// }
//
// void userPosts(string username) {
//     //extrage user-ul din baza de date
//     User* user = getUser(move(username));
//
//     //daca user-ul exista
//     if (user != nullptr) {
//         //daca user-ul nu e autentificat
//         if (currentUser == nullptr) {
//             //afiseaza postarile publice
//         } else { //daca user-ul este autentificat
//             //daca este admin sau prieten apropiat
//             if (currentUser->isAdmin == "yes" || isCloseFriend(user->username, currentUser->username)) {
//                 //afiseaza toate postarile
//             } else if (isFriend(user->username, currentUser->username)) {
//                 //afiseaza postarile publice si cele dedicate prietenilor
//             }
//         }
//     }
//
//     cout << "[server] Posts have been printed!\n";
//     fflush(stdout);
// }
//
// void sharePost(const string& content, const string& visibility) {
//     //verifica daca 'visibility' are un input valid
//     if (visibility != "public" && visibility != "friends" && visibility != "close friends") {
//         cout << "[server] You didn't enter a valid visibility of the post.\n";
//         fflush(stdout);
//         return;
//     }
//
//     //adauga postarea in baza de date
//     if (addPost(content, visibility)) {
//         cout << "[server] Post shared!\n";
//         fflush(stdout);
//     }
//     else {
//         cout << "[server] Couldn't share post.\n";
//         fflush(stdout);
//     }
// }
//
// void addFriend(const string& friendUsername, string friendshipType) {
//     //adauga prietenia in baza de date
//     if (addFriendship(currentUser->username, friendUsername, move(friendshipType))) {
//         cout << "[server] Couldn't add friend.\n";
//         fflush(stdout);
//     } else {
//         printf("%s is now your friend!\n", friendUsername.c_str());
//         fflush(stdout);
//     }
// }
//
// void removeFriend(const string& friendUsername) {
//     //sterge prietenia din baza de date
//     if (deleteFriendship(currentUser->username, friendUsername)) {
//         printf("[server] User %s has been removed from your friends lists.\n", friendUsername.c_str());
//         fflush(stdout);
//     } else {
//         printf("[server] User %s couldn't been removed from your friends lists.\n", friendUsername.c_str());
//         fflush(stdout);
//     }
// }
//
// void sendMessage(const string& content, string *users, int nr) {
//     int code;
//
//     //adauga mesajul in baza de date, destinatarul fiind, pe rand, fiecare persoana din 'users'
//     //mesajul va fi sters din baza de date dupa ce destinatarul il vizualizeaza
//     for (int index = 0; index < nr; index++) {
//         code = addMessage(content, currentUser->username, users[index]);
//         if (code == 0) {
//             printf("Couldn't send message to %s.\n", users[index].c_str());
//             fflush(stdout);
//         } else {
//             printf("Message sent to %s\n", users[index].c_str());
//             fflush(stdout);
//         }
//     }
// }
//
// void makeAdmin() {
//     //updateaza field-ul "isAdmin" din baza de date
//     if (updateUser(searchedUsername, "isAdmin", "yes")) {
//         printf("[server] User %s is now an admin.\n", searchedUsername.c_str());
//         fflush(stdout);
//     } else {
//         printf("[server] Couldn't make %s an admin.\n", searchedUsername.c_str());
//         fflush(stdout);
//     }
// }
//
// void deleteUser() {
//     //sterge user-ul din baza de date
//     if (deleteUser(searchedUsername)) {
//         printf("[server] User %s has been deleted.\n", searchedUsername.c_str());
//         fflush(stdout);
//     } else {
//         printf("[server] User %s couldn't been deleted.\n", searchedUsername.c_str());
//         fflush(stdout);
//     }
// }
//
// void deleteUserPost(int postId) {
//     //sterge postarea din baza de date
//     if (deletePost2(postId)) {
//         cout << "[server] Post deleted.\n";
//         fflush(stdout);
//     } else {
//         cout << "[server] Post couldn't been deleted.\n";
//         fflush(stdout);
//     }
// }
//
// void deletePost(int postId) {
//     //extrage postarea din baza de date
//     Post* post = getPost(postId);
//
//     //verifica daca exista postarea
//     if (post == nullptr) {
//         printf("[server] You have no post with the id %d.\n", postId);
//         fflush(stdout);
//         return;
//     }
//
//     //daca postarea este a utilizatorului
//     if (post->username == currentUser->username) {
//         //sterge postarea din baza de date
//         if (deletePost2(postId)) {
//             cout << "[server] Post deleted.\n";
//             fflush(stdout);
//         } else {
//             cout << "[server] Couldn't delete post.\n";
//             fflush(stdout);
//         }
//     } else {
//         printf("[server] You have no post with the id %d.\n", postId);
//         fflush(stdout);
//     }
// }
//
// void printUnreadMessages() {
//     /*
//      * extrage mesajele cu destinatarul 'currentUser->username' din baza de date
//      * afiseaza mesajele: sender, date, time, content
//      */
//
//     //sterge mesajele vizualizate
//     deleteMessages(currentUser->username);
//
//     cout << "[server] Messages have been printed!\n";
//     fflush(stdout);
// }
//
// void deleteAccount() {
//     //sterge user-ul din baza de date
//     if (deleteUser(currentUser->username)) {
//         cout << "[server] Your account have been deleted.\n";
//         fflush(stdout);
//     } else {
//         cout << "[server] Couldn't delete your account.\n";
//         fflush(stdout);
//     }
// }

void handleUnauthUser(int index) {

}

void handleLoggedUser(int index) {
    char response[SIZE];
    int argn;
    char* argv[20], *result = new char[SIZE], *copy = new char[SIZE];

    while (true) {
        if (send(clients[index], Meniu::meniu_user, SIZE, 0) == -1) {
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
                ///TODO: go to the next meniu
                cout << "ok";
                fflush(stdout);
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
            cout << "g";
            fflush(stdout);
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
             cout << "You are using the app without being authenticated.\n";
             fflush(stdout);
             strcpy(logged_users[index], "");
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
            counter = 1;
        } else {
            counter++;
        }
        ///TODO: decrementeaza cand se deconecteaza un client
        connectedClients++;
        clients[counter] = clientd;
        //cream un nou thread
        if (pthread_create(&(tid[counter]), NULL, authentication, &counter) != 0) {
            cout << "[server] Error at pthread_create().\n";
            fflush(stdout);
            continue;
        }
    }
}