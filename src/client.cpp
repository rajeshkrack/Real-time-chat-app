#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

void receive_messages(int client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        std::cout << "Received: " << buffer << std::endl;
    }
}

int main() {
    int client_socket;
    sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    std::thread receiveThread(receive_messages, client_socket);
    receiveThread.detach();

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        send(client_socket, message.c_str(), message.length(), 0);
    }

    close(client_socket);
    return 0;
}
