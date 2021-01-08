#include <iostream>

using namespace std;

#define SIZE 2024

class Post {
public:
    int id;
    char *username = new char[SIZE];
    char *postVisibility = new char[SIZE];
    char *content = new char[SIZE];
    char *date = new char[SIZE];
    char *time = new char[SIZE];

    Post() {}
};