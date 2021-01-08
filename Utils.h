#include <stdlib.h>
#include <ctime>

using namespace std;

class Utils {
public:
    static void inputParse(char response[], char* argv[], int &argn);
    static char* getCurrentDate();
    static char* getCurrentTime();
private:
    Utils() {}
};
