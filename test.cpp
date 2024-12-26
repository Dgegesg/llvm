#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // For inet_addr
#include <errno.h> // For errno

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const char *hello = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    // Attach socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return EXIT_FAILURE;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Accept incoming connections
    while (true) {
        std::cout << "Waiting for connections..." << std::endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed: " << strerror(errno) << std::endl;
            close(server_fd);
            return EXIT_FAILURE;
        }

        // Read the request
        read(new_socket, buffer, BUFFER_SIZE);
        std::cout << "Request: " << buffer << std::endl;

        // Send response
        send(new_socket, hello, strlen(hello), 0);
        std::cout << "Response sent" << std::endl;

        // Close the socket
        close(new_socket);
    }

    // Close the server socket (this line will never be reached in this example)
    close(server_fd);
   
