# Simple Chat Server with Winsock in C

This project implements a basic multi-client chat server using Winsock in C. The server allows clients to send and receive messages in real-time.

## Features:
- Server listens for incoming client connections.
- Clients send their messages, including their names, to the server.
- Server broadcasts received messages to all connected clients.
- Basic structure for multi-threaded handling of clients using Winsock.

## How it works:
1. **Server Initialization**: The server initializes Winsock and sets up a listening socket.
2. **Client Connection**: When a client connects, the server accepts the connection and creates a new thread to handle the communication for that client.
3. **Message Broadcasting**: Each time a client sends a message, the server broadcasts the message (including the client’s name) to all other connected clients.
4. **Client Disconnection**: When a client disconnects, the server removes the client from the list and stops handling the client's communication.

## Limitations:
- **No Client Authentication**: There is no validation of client names or any authentication system. Clients can use the same name.
- **No Private Messaging**: All messages are broadcasted to all clients. There's no option to send private messages.
- **No User Management**: Clients can’t modify their names during the session. The server doesn't store user data, making it difficult to manage long-term.
- **Limited Error Handling**: The project has minimal error handling. If a client fails to send or receive messages, the server may not handle it gracefully.
- **No Data Persistence**: Once a client disconnects, all session data (like their name and messages) is lost.

## Improvements for Future Versions:
- **User Authentication**: Implement authentication to ensure each client has a unique name.
- **Private Messaging**: Allow users to send messages to specific individuals rather than broadcasting to everyone.
- **Message History**: Store past messages so new clients can see previous conversations.
- **More Robust Error Handling**: Implement error handling for edge cases like connection timeouts or disconnections.
- **Security Features**: Add encryption or validation to secure messages between clients and the server.

## Requirements:
- Windows operating system (for using Winsock)
- MinGW (or a similar compiler that supports Winsock)
- Basic understanding of socket programming in C

## Instructions to Run:
1. Compile the server and client code using a C compiler that supports Winsock (e.g., MinGW).
2. Start the server by running the compiled `server.exe`.
3. Run multiple instances of the client to simulate multiple users.
4. The clients can send messages, and the server will broadcast them to other clients.

## License:
This project is licensed under the MIT License.
