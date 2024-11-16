import socket


# server configuration
HOST = '127.0.0.1'  # server IP address (localhost for now)
PORT = 8080         # server port

def connect_to_server():
    """Establishes a connection to the server and returns the socket object."""
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
        print("Connected to the Redis server.")
        return client_socket
    
    except ConnectionError:
        print("Failed to connect to the server.")
        return None

def send_command(client_socket, command):
    """Sends a command to the server and receives the response."""
    # append newline to command to signal end of command
    client_socket.sendall(command.encode('utf-8') + b'\n')
    
    # receive and decode the response from the server
    response = client_socket.recv(1024).decode('utf-8')
    return response

def main():
    client_socket = connect_to_server()
    if client_socket is None:
        return

    try:
        while True:
            # prompt for user input
            command = input("Enter command (or 'QUIT' to exit): ").strip()
            
            if command.upper() == "QUIT":
                print("Closing connection and exiting.")
                break
            
            # send user input
            response = send_command(client_socket, command)
            print(f"Server response: {response}\n")
    
    finally:
        client_socket.close()
        print("Disconnected from the server.")

if __name__ == "__main__":
    main()