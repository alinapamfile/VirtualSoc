#include "Utils.h"
#include <string.h>
#include <iostream>

#define SIZE 2024

//functie de parsare al input-ului. In argv se returneaza cuvintele din input-ul clientului
void Utils::inputParse(char response[], char* argv[], int &argn) {
    char *word;
    argn = 0;
    word = strtok(response, " ");
    while(word != NULL) {
        argv[argn++] = word;
        word = strtok(NULL, " ");
    }
}

//cripteaza parola (Caesar Cypher Algorithm)
char* Utils::encryptPassword(char *password) {
    char *encryptedPass = new char[SIZE];
    for (int i = 0; i < strlen(password); i++) {
        char ch = password[i] + 5 % 26;
        encryptedPass[i] = ch;
    }
    return encryptedPass;
}

//returneaza data curenta
char* Utils::getCurrentDate() {
    time_t rawtime;
    struct tm * timeinfo;
    char *currentdate = new char[50];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(currentdate,50,"%d-%m-%Y",timeinfo);
    return currentdate;
}

//returneaza ora curenta
char* Utils::getCurrentTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char *currenttime = new char[50];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(currenttime,50,"%H:%M",timeinfo);
    return currenttime;
}

/*
 * in 'type' se primeste conditia de selectare a inregistrarilor
 * se returneaza:
 * - 0 daca cei doi useri nu au relatia de prietenie cautata
 * - 1 daca cei doi useri au relatia de prietenie cautata
 * - -1 in caz de eroare
 */
int Utils::isFriend(sqlite3* db, char *username, char *friend_username, const char *type, char *errMessage) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    if (type)
        strcpy(stmt, "SELECT * FROM friends WHERE user=? AND friend=? AND type=?;");
    else
        strcpy(stmt, "SELECT * FROM friends WHERE user=? AND friend=?;");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return -1;
    }

    if (sqlite3_bind_text(result, 1, username, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return -1;
    }

    if (sqlite3_bind_text(result, 2, friend_username, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return -1;
    }

    if (type)
        if (sqlite3_bind_text(result, 3, type, -1, NULL) != SQLITE_OK) {
            cout << "[server] Error at sqlite3_bind_text()\n";
            fflush(stdout);
            strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
            return -1;
        }

    step = sqlite3_step(result);
    return step == SQLITE_ROW;
}
