from http.server import BaseHTTPRequestHandler, HTTPServer

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # Set the response code
        self.send_response(200)
        
        # Set the response headers
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        
        # Write the response content (Hello, World!)
        self.wfile.write(b'Hello, World!')

# Define server address and port
server_address = ('', 8000)  # Serve on localhost, port 8000
httpd = HTTPServer(server_address, SimpleHTTPRequestHandler)

print("Server running on http://localhost:8000")
httpd.serve_forever()
