#include <stdlib.h>
#include <sqlite3.h>
#include "User.h"

using namespace std;

class Database {
public:
    static sqlite3* db;
    static int userExists(char *username, char *errMessage);
    static bool addUser(char *argv[], int argn, char *errMessage);
    static User* getUser(char *username, char *errMessage);
    static bool getMatchingUsers(char *pattern, char *users[], int &count, char *errMessage);
    static bool updateUser(char *username, char *field, char *value, char *errMessage);
private:
    Database();
};
