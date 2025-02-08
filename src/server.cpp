#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 5

std::vector<int> clients;

void handle_client(int client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;
        
        std::cout << "Client: " << buffer << std::endl;
        
        // Broadcast message to all clients
        for (int client : clients) {
            if (client != client_socket) {
                send(client, buffer, bytesReceived, 0);
            }
        }
    }
    
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket < 0) continue;

        clients.push_back(client_socket);
        std::thread clientThread(handle_client, client_socket);
        clientThread.detach();
    }

    close(server_socket);
    return 0;
}
