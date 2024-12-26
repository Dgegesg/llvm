import socket

def start_server(host='127.0.0.1', port=8080):
    # Create a socket object using IPv4 and TCP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Bind the socket to the specified host and port
    server_socket.bind((host, port))
    
    # Enable the server to accept connections (max 1 pending connection)
    server_socket.listen(1)
    print(f"Server started at http://{host}:{port}")
    
    while True:
        # Accept an incoming connection from a client
        client_socket, client_address = server_socket.accept()
        print(f"Connection from {client_address}")
        
        # Receive the HTTP request from the client (buffer size 1024)
        request = client_socket.recv(1024).decode()
        
        # Simple response, just sends "Hello, World!" as a plain text response
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!"
        
        # Send the HTTP response to the client
        client_socket.sendall(response.encode())
        
        # Close the connection with the client
        client_socket.close()

if __name__ == "__main__":
    start_server()
