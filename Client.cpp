
// Loading Libraries

#include<stdio.h>   
#include<iostream>  
#include<stdlib.h>  
#include<string.h>  
#include<unistd.h>  
#include<sys/types.h>   
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<netdb.h>   
#include<arpa/inet.h>   
#define IP "127.0.0.1"  

using namespace std;

int main(){
    int client, server;
    int portNum;   // Variable to store port number
    bool isExit = false;   // To check when to exit the connection
    int bufsize = 1024;    // Buffer size for sending/receiving data
    char buffer[bufsize];  // Buffer to store data to be sent/received

    // sockaddr_in is used to specify the address and port number for the server
    struct sockaddr_in server_addr;

    // Create a TCP socket (AF_INET - IPv4, SOCK_STREAM - TCP)
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        cout << "Error creating socket." << endl;  
        exit(1);  
    }

    cout << "Client Socket created." << endl;
    cout << "Enter port number: ";  // Prompt user to enter port number
    cin >> portNum;

    // Set up server address parameters
    server_addr.sin_family = AF_INET;   // IPv4 family
    server_addr.sin_addr.s_addr = inet_addr(IP);  // Set the IP address (localhost in this case)
    server_addr.sin_port = htons(portNum);  // Set the port number

    // Connect to the server
    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl; 

    // Receive initial message from the server
    recv(client, buffer, bufsize, 0);
    cout << buffer;
    cout << "Connection confirmed." << endl;
    cout << "Enter # to end the connection." << endl;

    // Main loop for sending and receiving data
    do {
        cout << "Client: ";

        // Sending loop for the client
        do {
            cin >> buffer;  // Input message to be sent
            send(client, buffer, bufsize, 0);  // Send message to server

            if (*buffer == '#') {  // If the user enters '#', exit the connection
                send(client, buffer, bufsize, 0);
                *buffer = '*';  // Replace '#' with '*' to stop further sending
                isExit = true;
            }
        } while (*buffer != 42);  // ASCII 42 is '*' - loop until '*' is sent

        cout << "Server: ";

        // Receiving loop for the server response
        do {
            recv(client, buffer, bufsize, 0);  // Receive message from server
            cout << buffer << " ";  // Output the message

            if (*buffer == '#') {  // If server sends '#', close the connection
                *buffer = '*';  // Mark it for exit
                isExit = true;
            }
        } while (*buffer != 42);  // Loop until '*' is received from the server

        cout << endl;
    } while (!isExit);  

    cout << "Connection terminated." << endl;

    // Close the socket connection
    close(client);

    return 0;
}
