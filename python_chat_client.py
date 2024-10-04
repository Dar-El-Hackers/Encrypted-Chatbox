import socket

def start_client():
    # Step 1: Create a socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Step 2: Connect to the server
    server_ip = input("Enter the server's IP address: ")  # Get the server IP address
    server_port = 12345
    client_socket.connect((server_ip, server_port))
    print("Connected to the server.")

    # Step 3: Chat loop
    while True:
        # Step 4: Send a message to the server
        message = input("You: ")
        client_socket.send(message.encode())  # Sending a message
        if message.lower() == 'exit':
            print("You left the chat.")
            break

        # Step 5: Receive a message from the server
        server_message = client_socket.recv(1024).decode()  # Receiving a message
        if server_message.lower() == 'exit':
            print("Server has left the chat.")
            break
        print(f"Server: {server_message}")

    # Step 6: Close the connection
    client_socket.close()

# Call the function to start the client
start_client()
