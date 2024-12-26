#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

// Function to perform the HTTP GET request over a socket
void http_get(const std::string &host, const std::string &path) {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Get host information
    struct hostent *server = gethostbyname(host.c_str());
    if (!server) {
        std::cerr << "Error resolving host" << std::endl;
        close(sockfd);
        return;
    }

    // Set up the server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(80); // HTTP port

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        close(sockfd);
        return;
    }

    // Prepare the HTTP GET request
    std::ostringstream request;
    request << "GET " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    request << "Connection: close\r\n";
    request << "\r\n"; // End of headers

    // Send the HTTP GET request
    if (send(sockfd, request.str().c_str(), request.str().length(), 0) < 0) {
        std::cerr << "Error sending request" << std::endl;
        close(sockfd);
        return;
    }

    // Receive the HTTP response
    char buffer[4096];
    int n;
    while ((n = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0'; // Null-terminate the received data
        std::cout << buffer;
    }

    if (n < 0) {
        std::cerr << "Error receiving response" << std::endl;
    }

    // Close the socket
    close(sockfd);
}

int main() {
    // Example: Get the homepage of example.com
    http_get("example.com", "/");
    return 0;
}
