#include <pthread.h>
#include <stdlib.h>

using namespace std;

class Command {
public:
    static pthread_mutex_t mutex;
    static void signUp(char* argv[], int argn, char* response);
    static void logIn(char* argv[], int argn, char* response);
private:
    Command() {}
};
