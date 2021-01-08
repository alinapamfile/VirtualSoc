#include <pthread.h>
#include <stdlib.h>

using namespace std;

class Command {
public:
    static pthread_mutex_t mutex;
    static bool signUp(char* argv[], int argn, char* response);
    static bool logIn(char* argv[], int argn, char* response);
    static void editProfile(char *username, char* argv[], int argn, char* response);
    static bool searchUser(char *argv[], int argn, char *response);
    static void sharePost(char *username, char *argv[], int argn, char *response);
    static void deletePost(char *username, char *argv[], int argn, char *response);
    static void sendMessage(char *username, char *argv[], int argn, char *response);
    static void seeUnreadMessages(char *username, int argn, char *response);
private:
    Command() {}
};
