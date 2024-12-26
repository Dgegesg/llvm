#!/bin/sh

# Define the port to listen on
PORT=8080

# Start listening for HTTP requests
while true; do
    # Use nc (netcat) to listen for incoming connections on the given port
    # and pipe the request to the C++ backend
    nc -lk $PORT | ./backend
done
