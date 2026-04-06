#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    string message;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        return -1;
    }

    cout << "Server waiting for connection..." << endl;

    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept");
        return -1;
    }

    cout << "Client connected!" << endl;

    while (true) {
        // Receive message
        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);
        cout << "Client: " << buffer << endl;

        // Send reply
        cout << "Server: ";
        getline(cin, message);
        send(new_socket, message.c_str(), message.length(), 0);
    }

    return 0;
}