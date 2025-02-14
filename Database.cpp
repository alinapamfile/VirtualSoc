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

    //pregateste comanda sql
    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return -1;
    }

    //ataseaza parametrii necesari comenzii
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
    sqlite3_finalize(result);
    if (step != SQLITE_DONE) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}

bool Database::deleteUser(char *username, char *errMessage) {
    string stmts[4];

    stmts[0] = (string)"DELETE FROM users WHERE username='" + (string)username + (string)"';";
    stmts[1] = (string)"DELETE FROM posts WHERE username='" + (string)username + (string)"';";
    stmts[2] = (string)"DELETE FROM friends WHERE user='" + (string)username + (string)"' OR friend='"
                    + (string)username + (string)"';";
    stmts[3] = (string)"DELETE FROM messages WHERE sender='" + (string)username + (string)"' OR receiver='"
                   + (string)username + (string)"';";

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

    for (int i = 0; i < 4; i++) {
        if (sqlite3_exec(db, stmts[i].c_str(), NULL, NULL, NULL) != SQLITE_OK) {
            strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
            return false;
        }
    }

    sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);

    return true;
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
        if (sqlite3_column_type(result, 4) != SQLITE_NULL)
            strcpy(user->country, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 4))));
        if (sqlite3_column_type(result, 4) != SQLITE_NULL)
            strcpy(user->city, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 5))));
        if (sqlite3_column_type(result, 4) != SQLITE_NULL)
            strcpy(user->occupation, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 6))));
        strcpy(user->profileVisibility, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 7))));
        user->isAdmin = sqlite3_column_int(result, 8);
        sqlite3_finalize(result);
        return user;
    } else {
        sqlite3_finalize(result);
        strcpy(errMessage, "\nUser doesn't exist.\n\n");
        return NULL;
    }
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

    sqlite3_finalize(result);
    return true;
}

bool Database::updateUser(char *username, char *field, char *value, char *errMessage) {
    string stmt  = (string)"UPDATE users SET " + (string)field + (string)"='" + (string)value
                    + (string)"' WHERE username='" + (string)username + (string)"';";

    if (sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}

bool Database::updateUser(char *username, const char *field, int value, char *errMessage) {
    string stmt  = (string)"UPDATE users SET " + (string)field + (string)"='" + to_string(value)
                   + (string)"' WHERE username='" + (string)username + (string)"';";

    if (sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}

bool Database::addPost(char *username, char *argv[], int argn, char *errMessage) {
    string stmt = (string)"INSERT INTO posts VALUES (null, '" + (string)username + "'";

    for (int i = 1; i < argn; i++) {
        stmt += (string)", '" + (string)argv[i] + "'";
    }
    stmt += (string)");";

    if (sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}

Post* Database::getPost(char *id, char *errMessage) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;
    int step;

    strcpy(stmt, "SELECT * FROM posts WHERE id=?");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return NULL;
    }

    if (sqlite3_bind_text(result, 1, id, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return NULL;
    }

    step = sqlite3_step(result);

    Post *post = new Post();
    if (step == SQLITE_ROW) {
        post->id = sqlite3_column_int(result, 0);
        strcpy(post->username, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 1))));
        strcpy(post->postVisibility, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 2))));
        strcpy(post->content, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 3))));
        strcpy(post->date, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 4))));
        strcpy(post->time, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 5))));
        sqlite3_finalize(result);
        return post;
    } else {
        sqlite3_finalize(result);
        strcpy(errMessage, "\nPost doesn't exist.\n\n");
        return NULL;
    }
}

bool Database::deletePost(char *id, char *errMessage) {
    string stmt = (string)"DELETE FROM posts WHERE id=" + (string)id + ";";

    if (sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}

bool Database::addMessage(char *argv[], int argn, char *errMessage) {
    string stmt = (string)"INSERT INTO messages VALUES (";
    for (int i = 0; i < argn; i++) {
        if (i != 0)
            stmt += (string)", ";
        stmt += (string)"'" + (string)argv[i] + (string)"'";
    }

    stmt += (string)");";

    if (sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}

bool Database::getMessages(char *receiver, Message *messages[], int &count, char *errMessage) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;
    count = 0;

    strcpy(stmt, "SELECT * FROM messages WHERE receiver=?;");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    if (sqlite3_bind_text(result, 1, receiver, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    while (sqlite3_step(result) != SQLITE_DONE) {
        messages[count] = new Message();
        strcpy(messages[count]->sender, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 0))));
        strcpy(messages[count]->receiver, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 1))));
        strcpy(messages[count]->content, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 2))));
        strcpy(messages[count]->date, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 3))));
        strcpy(messages[count]->time, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 4))));
        count++;
    }

    sqlite3_finalize(result);

    if (count != 0)
        deleteUnreadMessages(receiver);
    return true;
}

void Database::deleteUnreadMessages(char *receiver) {
    string stmt = (string)"DELETE FROM messages WHERE receiver='" + (string)receiver + (string)"';";

    sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL);
}

bool Database::getUserPosts(char *user, char *visibility, Post *posts[], int &count, char *errMessage) {
    char *stmt = new char[SIZE];
    sqlite3_stmt *result;
    count = 0;

    if (visibility) {
        strcpy(stmt, "SELECT * FROM posts WHERE username=? ");
        strcat(stmt, visibility);
        strcat(stmt, ";");
    }
    else
        strcpy(stmt, "SELECT * FROM posts WHERE username=?;");

    if (sqlite3_prepare_v2(db, stmt, -1, &result, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_prepare_v2().\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    if (sqlite3_bind_text(result, 1, user, -1, NULL) != SQLITE_OK) {
        cout << "[server] Error at sqlite3_bind_text()\n";
        fflush(stdout);
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    }

    while (sqlite3_step(result) != SQLITE_DONE) {
        posts[count] = new Post();
        posts[count]->id = sqlite3_column_int(result, 0);
        strcpy(posts[count]->username, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 1))));
        strcpy(posts[count]->postVisibility, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 2))));
        strcpy(posts[count]->content, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 3))));
        strcpy(posts[count]->date, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 4))));
        strcpy(posts[count]->time, reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(result, 5))));
        count++;
    }

    sqlite3_finalize(result);
    return true;
}

bool Database::addFriend(char *argv[], int argn, char *errMessage) {
    string stmt = (string)"INSERT INTO friends VALUES (";

    for (int i = 0; i < argn; i++) {
        if (i != 0)
            stmt += (string)", ";
        stmt += (string)"'" + (string)argv[i] + (string)"'";
    }
    stmt += (string)");";

    if (sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}

bool Database::deleteFriend(char *user, char *friend_username, char *errMessage) {
    string stmt = (string)"DELETE FROM friends WHERE user='" + (string)user + (string)"' AND friend='"
                    + (string)friend_username + (string)"';";

    if (sqlite3_exec(db, stmt.c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        strcpy(errMessage, "\nCommand couldn't been executed.\n\n");
        return false;
    } else {
        return true;
    }
}
