#include <stdlib.h>

using namespace std;

#define SIZE 2024

class Message {
    char *sender = new char[SIZE];
    char *receiver = new char[SIZE];
    char *content = new char[SIZE];
    char *date = new char[SIZE];
    char *time = new char[SIZE];
};