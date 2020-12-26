#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define SIZE 2024
#define PORT 1024

using namespace std;

class User {
public:
    string username;
    string password;
    string firstname, lastname;
    string country, city;
    string occupation;
    string profileVisibility;
    string isAdmin;

    User(const string &username, const string &password, const string &firstname, const string &lastname,
         const string &country, const string &city, const string &occupation, const string &profileVisibility,
         const string &isAdmin) : username(username), password(password), firstname(firstname), lastname(lastname),
                                  country(country), city(city), occupation(occupation),
                                  profileVisibility(profileVisibility), isAdmin(isAdmin) {}
};

class Post {
public:
    string username;
    string id;
    string content;
    string date;
    string time;
    string postVisibility;
};

User* currentUser = new User("alinapamfile", "ubh21kfg4", "Alina", "Pamfile", "Romania",
                             "Iasi", "student", "public", "yes");
string searchedUsername = "mariapopa";

int isAvailable(string a) { return 1; }

char* encryptPassword(string pass) {
    char *password = (char*)malloc(SIZE);
    strcpy(password, "ubh21kfg4");
    return password;
}

int userExists(string username) { return 0; }

void getUserMatches(string username, string matches[], int &nr) {
    matches[0] = "alinap";
    matches[1] = "alinapamfile";
    nr = 2;
}

User* getUser(string username) {
    User* user = new User("alinapamfile", "ubh21kfg4", "Alina", "Pamfile", "Romania",
                         "Iasi", "student", "public", "yes");
    return user;
}

int updateUser(string username, string field, string newValue) { return 1; }
int addFriendship(string username1, string username2, string type) { return 1; }
int deleteFriendship(string username1, string username2) { return 1; }
int deleteUser(string username) { return 1; }
int deletePost(int id) { return 1; }
void deleteMessages(string username) {}
int addMessage(string content, string username1, string username2) { return 1; }

Post* getPost(int id) {
    Post* post = new Post();
    post->id = "1";
    post->username = "alinapamfile";
    post->content = "Continutul postarii.";
    post->date = "13.09.2020";
    post->time = "15:42";
    post->postVisibility = "private";
    return post;
}

int isValidField(string field) { return 1; }
int isFriend(string user1, string user2) { return 1; }
int isCloseFriend(string user1, string user2) { return 1; }
int addUser(string info[8]) { return 1; }
int addPost(string content, string visibility) { return 1; }

 /* in 'info' se vor primi: username, password, firstname, lastname, country, city,
  *                        occupation, profileVisibility
  */
 int sign_up(string info[8]) {
     //verifica daca username-ul si parola nu sunt null
     if (info[0].empty() || info[1].empty()) {
         cout << "[server] Username or password null.\n";
         fflush(stdout);
         return -1;
     }

     //verifica daca username-ul nu este deja utilizat
     if (!isAvailable(info[0])) {
         cout << "[server] Username is already used.\n";
         fflush(stdout);
         return -1;
     }

     //encripteaza parola
     info[1] = encryptPassword(info[1]);

     //adauga user-ul in baza de date
     if (!addUser(info)) {
         cout << "[server] Signing up failed.\n";
         fflush(stdout);
         return -1;
     }

     cout << "[server] Successfully signed up!\n";
     fflush(stdout);

     return 1;
 }

 int log_in(const string& username, string password) {
     //verifica daca user-ul exista
     if (userExists(username)) {
         cout << "[server] Username doesn't exist.\n";
         fflush(stdout);
         return -1;
     }

     //extrage user-ul din baza de date
     User* user = getUser(username);

     //verifica daca parola introdusa este corecta
     if (user->password != encryptPassword(move(password))) {
         cout << "[server] Password incorrect.\n";
         fflush(stdout);
         return -1;
     }

     currentUser = user;
     cout << "[server] Successfully logged in!\n";
     fflush(stdout);

     return 1;
 }

 void edit_profile(const string& field, string newValue) {
     //verifica daca field-ul este valid (daca exista in baza de date)
     if (!isValidField(field)) {
         cout << "[server] Invalid field to edit.\n";
         fflush(stdout);
         return;
     }

     //user-ul nu isi poate edita username-ul
     if (field == "username") {
         cout << "[server] You can't edit your username.\n";
         fflush(stdout);
         return;
     }

     //daca user-ul vrea sa iti schimbe parola, aceasta va fi encriptata
     if (field == "password") {
         newValue = encryptPassword(newValue);
     }

     //updateaza user-ul in baza de date
     if (updateUser(currentUser->username, field, move(newValue))) {
         cout << "[server] Couldn't edit profile.\n";
         fflush(stdout);
         return;
     }

     //extrage user-ul modificat din baza de date
     currentUser = getUser(currentUser->username);

     cout << "[server] Profile has been edited successfully!\n";
     fflush(stdout);
 }

 int search_user(const string& input) {
     //verifica daca input-ul este un username existent
     if (userExists(input)) {
         searchedUsername = input;
         return 1;
     }

     //returneaza in 'matches' username-urile care se potrivesc cu textul cautat
     string matches[20];
     int nr = 0;
     getUserMatches(input, matches, nr);

     //daca nu se gaseste nicio potrivire
     if (nr == 0) {
         printf("[server] User %s does not exist. No matches found for '%s'.\n", input.c_str(), input.c_str());
         fflush(stdout);
         return -1;
     }

     printf("[server] User %s does not exist. Matches of '%s':\n", input.c_str(), input.c_str());
     fflush(stdout);

     //afiseaza potrivirile gasite
     for (int index = 0; index < nr; index++) {
         cout << matches[index] << endl;
         fflush(stdout);
     }

     return -1;
 }

 void user_details(string username) {
     //extrage user-ul din baza de date
     User* user = getUser(move(username));

     //daca user-ul exista
     if (user != nullptr) {
         //daca persoana are profil public
         if (user->profileVisibility == "public") {
             cout << "Username: " + user->username + "\n" + "First name: " + user->firstname + "\n" +
                     "Last name: " + user->lastname + "\n" + "Country: " + user->country + "\n" +
                     "City: " + user->city + "\n" + "Occupation: " + user->occupation + "\n";
             fflush(stdout);
         } else { //daca persoana are profil privat
             //daca user-ul este admin sau prieten cu persoana respectiva
             if (currentUser->isAdmin == "yes" || isFriend(user->username, currentUser->username) ||
                 isCloseFriend(user->username, currentUser->username)) {
                 cout << "Username: " + user->username + "\n" + "First name: " + user->firstname + "\n" +
                         "Last name: " + user->lastname + "\n" + "Country: " + user->country + "\n" +
                         "City: " + user->city + "\n" + "Occupation: " + user->occupation + "\n";
                 fflush(stdout);
             } else {
                 cout << "[server] Private account.";
                 fflush(stdout);
             }
         }
     }
 }

 void user_posts(string username) {
     //extrage user-ul din baza de date
     User* user = getUser(move(username));

     //daca user-ul exista
     if (user != nullptr) {
         //daca user-ul nu e autentificat
         if (currentUser == nullptr) {
             //afiseaza postarile publice
         } else { //daca user-ul este autentificat
             //daca este admin sau prieten apropiat
             if (currentUser->isAdmin == "yes" || isCloseFriend(user->username, currentUser->username)) {
                 //afiseaza toate postarile
             } else if (isFriend(user->username, currentUser->username)) {
                 //afiseaza postarile publice si cele dedicate prietenilor
             }
         }
     }

     cout << "[server] Posts have been printed!\n";
     fflush(stdout);
 }

 void share_post(const string& content, const string& visibility) {
     //verifica daca 'visibility' are un input valid
     if (visibility != "public" && visibility != "friends" && visibility != "close friends") {
         cout << "[server] You didn't enter a valid visibility of the post.\n";
         fflush(stdout);
         return;
     }

     //adauga postarea in baza de date
     if (addPost(content, visibility)) {
         cout << "[server] Post shared!\n";
         fflush(stdout);
     }
     else {
         cout << "[server] Couldn't share post.\n";
         fflush(stdout);
     }
 }

 void add_friend(const string& friendUsername, string friendshipType) {
     //adauga prietenia in baza de date
     if (addFriendship(currentUser->username, friendUsername, move(friendshipType))) {
         cout << "[server] Couldn't add friend.\n";
         fflush(stdout);
     } else {
         printf("%s is now your friend!\n", friendUsername.c_str());
         fflush(stdout);
     }
 }

 void remove_friend(const string& friendUsername) {
     //sterge prietenia din baza de date
     if (deleteFriendship(currentUser->username, friendUsername)) {
         printf("[server] User %s has been removed from your friends lists.\n", friendUsername.c_str());
         fflush(stdout);
     } else {
         printf("[server] User %s couldn't been removed from your friends lists.\n", friendUsername.c_str());
         fflush(stdout);
     }
 }

 void send_message(const string& content, string users[], int nr) {
     int code;

     //adauga mesajul in baza de date, destinatarul fiind, pe rand, fiecare persoana din 'users'
     //mesajul va fi sters din baza de date dupa ce destinatarul il vizualizeaza
     for (int index = 0; index < nr; index++) {
         code = addMessage(content, currentUser->username, users[index]);
         if (code == 0) {
             printf("Couldn't send message to %s.\n", users[index].c_str());
             fflush(stdout);
         } else {
             printf("Message sent to %s\n", users[index].c_str());
             fflush(stdout);
         }
     }
 }

 void make_admin() {
     //updateaza field-ul "isAdmin" din baza de date
     if (updateUser(searchedUsername, "isAdmin", "yes")) {
         printf("[server] User %s is now an admin.\n", searchedUsername.c_str());
         fflush(stdout);
     } else {
         printf("[server] Couldn't make %s an admin.\n", searchedUsername.c_str());
         fflush(stdout);
     }
 }

 void delete_user() {
     //sterge user-ul din baza de date
     if (deleteUser(searchedUsername)) {
         printf("[server] User %s has been deleted.\n", searchedUsername.c_str());
         fflush(stdout);
     } else {
         printf("[server] User %s couldn't been deleted.\n", searchedUsername.c_str());
         fflush(stdout);
     }
 }

 void delete_user_post(int postId) {
     //sterge postarea din baza de date
     if (deletePost(postId)) {
         cout << "[server] Post deleted.\n";
         fflush(stdout);
     } else {
         cout << "[server] Post couldn't been deleted.\n";
         fflush(stdout);
     }
 }

 void delete_post(int postId) {
     //extrage postarea din baza de date
     Post* post = getPost(postId);

     //verifica daca exista postarea
     if (post == nullptr) {
         printf("[server] You have no post with the id %d.\n", postId);
         fflush(stdout);
         return;
     }

     //daca postarea este a utilizatorului
     if (post->username == currentUser->username) {
         //sterge postarea din baza de date
         if (deletePost(postId)) {
             cout << "[server] Post deleted.\n";
             fflush(stdout);
         } else {
             cout << "[server] Couldn't delete post.\n";
             fflush(stdout);
         }
     } else {
         printf("[server] You have no post with the id %d.\n", postId);
         fflush(stdout);
     }
 }

 void print_unread_messages() {
     /*
      * extrage mesajele cu destinatarul 'currentUser->username' din baza de date
      * afiseaza mesajele: sender, date, time, content
      */

     //sterge mesajele vizualizate
     deleteMessages(currentUser->username);

     cout << "[server] Messages have been printed!\n";
     fflush(stdout);
 }

 void delete_account() {
     //sterge user-ul din baza de date
     if (deleteUser(currentUser->username)) {
         cout << "[server] Your account have been deleted.\n";
         fflush(stdout);
     } else {
         cout << "[server] Couldn't delete your account.\n";
         fflush(stdout);
     }
 }

int main() {
    struct sockaddr_in server{};
    struct sockaddr_in client{};
    int sd, child, option = 1;

    //Meniurile disponibile:

    char meniu_auth[SIZE] = "Enter one of the commands below:\n"
                         "1. sign up\n"
                         "2. log in\n"
                         "3. continue without authentication\n";

//    char meniu_unauth[SIZE] = "Because you are not authenticated, you can only access a few of the app's functionalities.\n"
//                                         "Enter one of the commands below:\n"
//                                         "1. search user\n"
//                                         "2. back\n";
//    char meniu_user[SIZE] = "Enter one of the commands below:\n"
//                            "1. search user\n"
//                            "2. share post\n"
//                            "3. delete post\n"
//                            "4. send a message\n"
//                            "5. see unread messages\n"
//                            "6. edit profile\n"
//                            "7. delete account\n"
//                            "8. log out";
//    char meniu_not_a_friend[SIZE] = "Enter one of the commands below:\n"
//                                    "1. see user details\n"
//                                    "2. see user posts\n"
//                                    "3. add as friend\n"
//                                    "4. add as close friend\n"
//                                    "5. send message\n"
//                                    "6. back\n";
//    char meniu_friend[SIZE] = "Enter one of the commands below:\n"
//                              "1. see user details\n"
//                              "2. see user posts\n"
//                              "3. remove from friends\n"
//                              "4. send message\n"
//                              "5. back\n";
//    char meniu_unauth_user[SIZE] = "Because you are not authenticated, you can only access a few of the app's functionalities.\n"
//                                   "Enter one of the commands below:\n"
//                                   "1. see user details\n"
//                                   "2. see user posts\n"
//                                   "3. back";
//    char meniu_admin_not_a_friend[SIZE] = "Enter one of the commands below:\n"
//                                          "1. see user details\n"
//                                          "2. see user posts\n"
//                                          "3. add as friend\n"
//                                          "4. add as close friend\n"
//                                          "5. send message\n"
//                                          "6. make admin\n"
//                                          "7. delete user\n"
//                                          "8. delete user post\n"
//                                          "9. back\n";
//    char meniu_admin_friend[SIZE] = "Enter one of the commands below:\n"
//                              "1. see user details\n"
//                              "2. see user posts\n"
//                              "3. remove from friends\n"
//                              "4. send message\n"
//                              "5. make admin\n"
//                              "6. delete user\n"
//                              "7. delete user post\n"
//                              "8. back\n";

    char response[SIZE], result[SIZE] = "Command executed successfully";

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
    if (listen(sd, 50) == -1) {
        cout << "[server] Error at listen().\n";
        fflush(stdout);
        exit(1);
    }

    //servirea concurenta a clientilor
    while (true) {
        int clientd;
        int length = sizeof (struct sockaddr);

        printf("[server] Waiting at port %d.\n", PORT);
        fflush(stdout);

        //acceptam un client
        clientd = accept(sd, (struct sockaddr*) &client, (socklen_t *) &length);

        if (clientd == -1) {
            cout << "[server] Error at accept().\n";
            fflush(stdout);
            continue;
        }

        //cream un proces-copil care va comunica cu clientul
        if ((child = fork()) == -1) {
            cout << "[server] Error at fork().\n";
            fflush(stdout);
            continue;
        }

        if (!child) {
            close(sd);
            //comunica cu clientul pana cand acesta se deconecteaza sau isi sterge contul
            while (true) {
                //trimite meniul disponibil clientului
                if (send(clientd, &meniu_auth, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                //primeste cereri de comenzi de la client
                if (recv(clientd, &response, SIZE, 0) == -1) {
                    cout << "[server] Error at recv().\n";
                    fflush(stdout);
                    continue;
                }

                //onoreaza comenzile

                //exemple de comenzi
                if (strcmp(response, "sign up") == 0) {
                    //cere clientului sa introduca toate informatiile necesare

                    //apelare sign_up() cu un exemplu
                    string info[] = {"alinapamfile", "parola123", "Alina", "Pamfile", "Romania", "Iasi", "Student", "public"};
                    if (sign_up(info)) {
                        currentUser = getUser("alinapamfile");
                    }
                } else if (strcmp(response, "log in") == 0) {
                    log_in("alinapamfile", "parola123");
                } else if (strcmp(response, "continue without authentication") == 0) {
                    cout << "You are using the app without being authenticated.\n";
                    fflush(stdout);
                }

                //exemple de apelari ale functiilor ce realizeaza comenzile:

//                 edit_profile("city", "Tecuci");
//                 search_user("mariapopa");
//                 user_details("alinapamfile");
//                 user_posts("alinapamfile");
//                 share_post("Continutul postarii.", "public");
//                 add_friend("mariapopa", "close friend");
//                 remove_friend("mariapopa");
//
//                 string users[] = { "mariapopa" };
//                 int nr = 1;
//                 send_message("Un mesaj important.", users, nr);
//
//                 make_admin();
//                 delete_user();
//                 delete_user_post(1);
//                 delete_post(1);
//                 print_unread_messages();
//                 delete_account();

                //trimite clientului raspunsul
                if (send(clientd, result, SIZE, 0) == -1) {
                    cout << "[server] Error at send().\n";
                    fflush(stdout);
                    continue;
                }

                //incheie comunicarea
                if (strcmp(response, "log out") == 0 || strcmp(response, "delete account") == 0){
                    exit(0);
                }
            }
        } else {
            close(clientd);
            continue;
        }
    }
}

