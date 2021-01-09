#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#define SIZE 2024

using namespace std;

int main(int argc, char *argv[]) {
    struct sockaddr_in server{};
    int sd, port;
    char meniu[SIZE], input[SIZE], response[SIZE];

    //verifica daca exista toate argumentele in linia de comanda
    if (argc != 3) {
        printf("Syntax: %s <server_address> <port>\n", argv[0]);
        exit(1);
    }

    //stabileste portul
    port = atoi(argv[2]);

    //creaza un socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[client] Error at socket().\n");
        exit(1);
    }

    //familia socket-ului
    server.sin_family = AF_INET;
    //adresa IP a serverului
    server.sin_addr.s_addr = inet_addr(argv[1]);
    //portul de conectare
    server.sin_port = htons(port);

    //conectare la server
    if (connect(sd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1) {
        perror("[client] Error at connect().\n");
        exit(1);
    }

    //comunica cu serverul pana la introducerea comenzii "log out" sau "delete account"
    while (true) {
        //primeste meniul curent de la server
        if (recv(sd, meniu, SIZE, 0) == -1) {
            perror("[client] Error at recv().\n");
            exit(1);
        }

        cout << meniu << endl;
        //citeste comanda utilizatorului
        cin.getline(input, SIZE);

        //trimite comanda la server
        if (send(sd, input, SIZE, 0) == -1) {
            perror("[client] Error at send().\n");
            exit(1);
        }

        //primeste raspunsul din partea serverului
        if (recv(sd, response, SIZE, 0) == -1) {
            perror("[client] Error at recv().\n");
            exit(1);
        }

        cout << response << endl;
        fflush(stdout);

        if (strcmp(input, "quit") == 0 || strcmp(input, "log_out") == 0 || strcmp(input, "delete_account") == 0)
            exit(0);
    }
}