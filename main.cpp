#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

int main()
{
    //init socket, main struct and text buffer
    struct sockaddr_in addr;
    
    int sock, listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[1024];
    int bytes_read;
    
    //checking: socket created successfully?
    if(listener == -1)
    {
        perror("ERROR: creating socket");
        exit(EXIT_FAILURE);
    }

    //clean basic struct before filling
    memset(&addr, 0, sizeof(addr));

    //filling basic struct
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1100);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //binding socket to port
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("ERROR: binding error");
        exit(EXIT_FAILURE);
    }

    //preparing socket for incloming connections (creating descriptor)
    listen(listener, 1);

    while(1)
    {
        //accepting connection request
        sock = accept(listener, NULL, NULL);

        //checking: accepting socket ok?
        if(sock < 0)
        {
            perror("ERROR: accepting socket");
            exit(EXIT_FAILURE);
        }

        //std::cout << "Server started successfully!" << std::endl;
        //std::cout << "Waiting for commands or messages..." << std::endl;

        while(1)
        {
            bytes_read = recv(sock, buffer, 1024, 0);
            std::cout << "Reading new bytes..." << std::endl;
            if(bytes_read <= 0) break;
            std::cout << "Sending anw back..." << std::endl;
            send(sock, buffer, bytes_read, 0);     
        }
    
        close(sock);
    }

    return 0;
}