#include "Database.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

sqlite3* Database::db;

bool Database::userExists(char *username, char *response) {
    char* stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    strcpy(stmt, "SELECT * FROM users WHERE username=?;");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(response, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    if (sqlite3_bind_text(result, 1, username, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(response, "\nCommand couldn't been executed.\n\n");
        return false;
    }
    step = sqlite3_step(result);
    sqlite3_finalize(result);
    //exista un user cu acest username
    if (step == SQLITE_ROW) {
        strcpy(response, "\nUsername already exists.\n\n");
        return false;
    } else {
        strcpy(response, "\nSuccessfully signed up!\n\n");
        return true;
    }
}

void Database::addUser(char *argv[], int argn, char *response) {
    char* stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    strcpy(stmt, "INSERT INTO users (username, password, firstname, lastname, profileVisibility, isAdmin) VALUES (?, ?, ?, ?, ?, 0);");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(response, "\nCommand couldn't been executed.\n\n");
        return;
    }

    for (int index = 1; index < argn; index++) {
        if (sqlite3_bind_text(result, index, argv[index], -1, NULL) != SQLITE_OK) {
            cout << "[server] Error at sqlite3_bind_text()\n";
            fflush(stdout);
            strcpy(response, "\nCommand couldn't been executed.\n\n");
            return;
        }
    }

    step = sqlite3_step(result);
    if (step != SQLITE_DONE) {
        strcpy(response, "\nCommand couldn't been executed.\n\n");
    } else {
        strcpy(response, "\nSuccessfully signed up!\n\n");
    }
    sqlite3_finalize(result);
}

User* Database::getUser(char *username, char *response) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    //verificam daca exista deja un user cu acelasi username
    strcpy(stmt, "SELECT * FROM users WHERE username=?;");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(response, "\nCommand couldn't been executed.\n\n");
        return NULL;
    }

    if (sqlite3_bind_text(result, 1, username, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(response, "\nCommand couldn't been executed.\n\n");
        return NULL;
    }
    step = sqlite3_step(result);

    User *user = new User();
    if (step == SQLITE_ROW) {
        strcpy(user->username, static_cast<char const *> (sqlite3_column_text16(result, 0)));
        strcpy(user->password, static_cast<char const *> (sqlite3_column_text16(result, 1)));
        strcpy(user->firstname, static_cast<char const *> (sqlite3_column_text16(result, 2)));
        strcpy(user->lastname, static_cast<char const *> (sqlite3_column_text16(result, 3)));
        strcpy(user->country, static_cast<char const *> (sqlite3_column_text16(result, 4)));
        strcpy(user->city, static_cast<char const *> (sqlite3_column_text16(result, 5)));
        strcpy(user->occupation, static_cast<char const *> (sqlite3_column_text16(result, 6)));
        strcpy(user->profileVisibility, static_cast<char const *> (sqlite3_column_text16(result, 7)));
        user->isAdmin = sqlite3_column_int(result, 8);
        return user;
    } else {
        strcpy(response, "\nUser doesn't exist.\n\n");
        return NULL;
    }
    sqlite3_finalize(result);
}