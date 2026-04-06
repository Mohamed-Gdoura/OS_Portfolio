#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    string message;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Connect to localhost
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address" << endl;
        return -1;
    }

    // Connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection Failed" << endl;
        return -1;
    }

    cout << "Connected to server!" << endl;

    while (true) {
        // Send message
        cout << "Client: ";
        getline(cin, message);
        send(sock, message.c_str(), message.length(), 0);

        // Receive reply
        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        cout << "Server: " << buffer << endl;
    }

    return 0;
}