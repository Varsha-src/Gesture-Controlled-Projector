
// Loading Libraries
#include<iostream>   
#include<stdio.h>    
#include<stdlib.h>   
#include<string.h>   
#include<unistd.h>   
#include<sys/types.h>   
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<netdb.h>    

#define IP "127.0.0.1"  

using namespace std;

int main() {
    int client, server;   // `client` for the listening socket, `server` for the connection
    int portNum;          // Port number for the server to listen on
    bool isExit = false;  // Flag to determine when to exit the connection
    int bufsize = 1024;   // Buffer size for sending/receiving data
    char buffer[bufsize]; // Buffer to store messages

    // Struct to hold the server's address information
    struct sockaddr_in server_addr;
    socklen_t size;  // To store size of server address structure

    // Create a TCP socket: AF_INET for IPv4, SOCK_STREAM for TCP
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        cout << "Error establishing connection." << endl;
        exit(1);  
    }
    cout << "Server Socket connection created." << endl;

    cout << "Enter port number: ";  
    cin >> portNum;

    // Set up the server address structure
    server_addr.sin_family = AF_INET;  // IPv4 address family
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  // Bind to any IP address
    server_addr.sin_port = htons(portNum);  // Set port number (converted to network byte order)

    // Bind the socket to the server address and port
    if (bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cout << "Error binding socket." << endl;
        exit(1);  // Exit if the bind fails
    }

    size = sizeof(server_addr);  // Set the size of the server address structure
    cout << "Looking for clients..." << endl;

    // Listen for incoming connections, max queue of 1 connection
    listen(client, 1);

    // Accept a connection from a client
    server = accept(client, (struct sockaddr*)&server_addr, &size);
    if (server < 0) {
        cout << "Error on accepting." << endl;
        exit(1);  // Exit if the accept fails
    }

    // Main loop for handling the client-server communication
    while (server > 0) {
        // Send initial message to the client
        strcpy(buffer, "Server connected..\n");
        send(server, buffer, bufsize, 0);
        cout << "Connected with client." << endl;
        cout << "Enter # to end the connection." << endl;

        cout << "Client: ";

        // Loop to handle receiving data from the client
        do {
            recv(server, buffer, bufsize, 0);  // Receive message from client
            cout << buffer;  // Display the message

            if (*buffer == '#') {  // If client sends '#', mark for exit
                *buffer = '*';     // Replace '#' with '*' to indicate termination
                isExit = true;     
            }
        } while (*buffer != '*');  // Loop until '*' is received

        // Server's turn to send a response to the client
        do {
            cout << "\nServer: ";

            // Loop for sending data
            do {
                cin >> buffer;  // Read input from server user
                send(server, buffer, bufsize, 0);  // Send data to client

                if (*buffer == '#') {  // If server sends '#', mark for exit
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';  // Replace '#' with '*'
                    isExit = true;
                }
            } while (*buffer != '*');  // Loop until '*' is sent

            // Now, server waits for the client's response again
            cout << "Client: ";

            do {
                recv(server, buffer, bufsize, 0);  // Receive client response
                cout << buffer << " ";  // Display client message

                if (*buffer == '#') {  // If client sends '#', mark for exit
                    *buffer = '*';  // Replace '#' with '*'
                    isExit = true;
                }
            } while (*buffer != '*');  // Loop until '*' is received

        } while (!isExit);  

        // After exiting the loop
        cout << "Connection terminated." << endl;
        isExit = false;  // Reset the exit flag for future connections

        exit(1);  // Terminate the program (can be replaced with a loop for more clients)
    }

    // Close the server socket after the connection ends
    close(client);
    return 0;
}
