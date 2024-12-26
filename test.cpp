#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

// Function to handle client requests
void handle_client(int client_socket) {
    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
    // Send the HTTP response
    send(client_socket, response, strlen(response), 0);
    // Close the client socket
    close(client_socket);
}

int main() {
    // Step 1: Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    // Step 2: Define the address structure
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    address.sin_port = htons(PORT);

    // Step 3: Bind the socket to the address
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed!" << std::endl;
        close(server_fd);
        return -1;
    }

    // Step 4: Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed!" << std::endl;
        close(server_fd);
        return -1;
    }
    std::cout << "Server is listening on 127.0.0.1:" << PORT << std::endl;

    // Step 5: Accept incoming client connections and handle them
    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Accept failed!" << std::endl;
            close(server_fd);
            return -1;
        }
        
        std::cout << "Connection established with client." << std::endl;

        // Handle the client (respond with "Hello, World!")
        handle_client(client_socket);
    }

    // Close the server socket (this line will never be reached in this simple example)
    close(server_fd);
    return 0;
}
