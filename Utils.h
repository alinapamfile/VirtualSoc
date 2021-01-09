#include <stdlib.h>
#include <ctime>
#include <sqlite3.h>

using namespace std;

class Utils {
public:
    static void inputParse(char response[], char* argv[], int &argn);
    static char* getCurrentDate();
    static char* getCurrentTime();
    static int isFriend(sqlite3* db, char *username, char *friend_username, const char *type, char *errMessage);
private:
    Utils() {}
};
