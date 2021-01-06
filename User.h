#include <iostream>

using namespace std;

#define SIZE 2024

class User {
public:
    char* username = new char[SIZE];
    char* password = new char[SIZE];
    char* firstname = new char[SIZE];
    char* lastname = new char[SIZE];
    char* country = new char[SIZE];
    char* city = new char[SIZE];
    char* occupation = new char[SIZE];
    char* profileVisibility = new char[SIZE];
    int isAdmin;

    User() {}
};