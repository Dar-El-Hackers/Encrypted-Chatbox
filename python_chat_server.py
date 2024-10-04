import socket

def start_server():
    # Step 1: Create a socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Step 2: Bind the socket to an IP and PORT
    server_socket.bind(('0.0.0.0', 12345))  # 0.0.0.0 means it will listen on all interfaces, port 12345
    
    # Step 3: Start listening for incoming connections
    server_socket.listen(1)  # Allow one connection
    print("Server is listening for incoming connections...")
    
    # Step 4: Accept a connection
    conn, addr = server_socket.accept()
    print(f"Connection established with {addr}")

    # Step 5: Chat loop
    while True:
        # Step 6: Receive message from client
        client_message = conn.recv(1024).decode()  # Receiving a message
        if client_message.lower() == 'exit':
            print("Client has left the chat.")
            break
        print(f"Client: {client_message}")

        # Step 7: Send a message back to the client
        server_message = input("You: ")
        conn.send(server_message.encode())  # Sending a message
        if server_message.lower() == 'exit':
            print("You left the chat.")
            break

    # Step 8: Close the connection
    conn.close()
    server_socket.close()

# Call the function to start the server
start_server()

