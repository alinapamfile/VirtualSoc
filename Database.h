#include <stdlib.h>
#include <sqlite3.h>
#include "User.h"
#include "Post.h"

using namespace std;

class Database {
public:
    static sqlite3* db;
    static int userExists(char *username, char *errMessage);
    static bool addUser(char *argv[], int argn, char *errMessage);
    static User* getUser(char *username, char *errMessage);
    static bool getMatchingUsers(char *pattern, char *users[], int &count, char *errMessage);
    static bool updateUser(char *username, char *field, char *value, char *errMessage);
    static bool addPost(char *username, char *argv[], int argn, char *errMessage);
    static Post* getPost(char *id, char *errMessage);
    static bool deletePost(char *username, char *id, char *errMessage);
private:
    Database();
};
