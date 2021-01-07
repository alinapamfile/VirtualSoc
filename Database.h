#include <stdlib.h>
#include <sqlite3.h>
#include "User.h"

using namespace std;

class Database {
public:
    static sqlite3* db;
    static bool userExists(char *username, char *response);
    static void addUser(char *argv[], int argn, char *response);
    static User* getUser(char *username, char *response);
private:
    Database();
};
