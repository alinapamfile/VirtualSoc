#include "Database.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

sqlite3* Database::db;

int Database::userExists(char *username, char *errMessage) {
    char* stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    strcpy(stmt, "SELECT * FROM users WHERE username=?;");

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
    step = sqlite3_step(result);
    sqlite3_finalize(result);

    //exista un user cu acest username
    if (step == SQLITE_ROW) {
        return 1;
    } else {
        return 0;
    }
}

bool Database::addUser(char *argv[], int argn, char *errMessage) {
    char* stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    strcpy(stmt, "INSERT INTO users (username, password, firstname, lastname, profileVisibility, isAdmin) VALUES (?, ?, ?, ?, ?, 0);");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    for (int index = 1; index < argn; index++) {
        if (sqlite3_bind_text(result, index, argv[index], -1, NULL) != SQLITE_OK) {
            cout << "[server] Error at sqlite3_bind_text()\n";
            fflush(stdout);
            strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
            return false;
        }
    }

    step = sqlite3_step(result);
    if (step != SQLITE_DONE) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
    sqlite3_finalize(result);
}

User* Database::getUser(char *username, char *errMessage) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    //verificam daca exista deja un user cu acelasi username
    strcpy(stmt, "SELECT * FROM users WHERE username=?;");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return NULL;
    }

    if (sqlite3_bind_text(result, 1, username, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return NULL;
    }
    step = sqlite3_step(result);

    User *user = new User();
    if (step == SQLITE_ROW) {
        strcpy(user->username, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 0))));
        strcpy(user->password, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 1))));
        strcpy(user->firstname, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 2))));
        strcpy(user->lastname, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 3))));
        strcpy(user->country, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 4))));
        strcpy(user->city, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 5))));
        strcpy(user->occupation, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 6))));
        strcpy(user->profileVisibility, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 7))));
        user->isAdmin = sqlite3_column_int(result, 8);
        return user;
    } else {
        strcpy(errMessage, "\nUser doesn't exist.\n\n");
        return NULL;
    }
    sqlite3_finalize(result);
}

bool Database::getMatchingUsers(char *pattern, char *users[], int &count, char *errMessage) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;

    //verificam daca exista deja un user cu acelasi username
    strcpy(stmt, "SELECT username FROM users WHERE username LIKE ?;");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    if (sqlite3_bind_text(result, 1, pattern, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    while (sqlite3_step(result) != SQLITE_DONE) {
        users[count] = new char[50];
        strcpy(users[count++], reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 0))));
    }

    return true;
}

bool Database::updateUser(char *username, char *field, char *value, char *errMessage) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    //pregatim comanda sql
    strcpy(stmt, "UPDATE users SET ");
    strcat(stmt, field);
    strcat(stmt, "='");
    strcat(stmt, value);
    strcat(stmt, "' WHERE username='");
    strcat(stmt, username);
    strcat(stmt, "';");

    if (sqlite3_exec(db, stmt, NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}