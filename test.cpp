#include <iostream>
#include <string>

int main() {
    // Read HTTP request from stdin (where nc will pipe it)
    std::string request;
    std::getline(std::cin, request);

    // Log the request (you can modify this to process the request)
    std::cerr << "Received Request: " << request << std::endl;

    // Send HTTP response (Basic HTML page)
    std::cout << "HTTP/1.1 200 OK\r\n";
    std::cout << "Content-Type: text/html\r\n\r\n";
    std::cout << "<html><body><h1>Hello, World!</h1></body></html>\r\n";

    return 0;
}
