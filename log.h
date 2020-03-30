#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <fstream>

class logs
{
    public:

        void main_log()
        {
            std::ofstream fstream;

            fstream.open("log.txt");
            if(!fstream.is_open())
            {
                perror("logs");    
            }

            fstream << "Server started!" << std::endl;
            fstream << "Listener on port: " << PORT << std::endl;
            fstream << "Waiting for connections..." << std::endl;
        }

        int connection_log(int new_socket, struct sockaddr_in address, int i)
        {
            std::ofstream fstream;

            fstream.open("log.txt", std::ios::app);
            if(!fstream.is_open())
            {
                perror("logs");    
            }

            fstream << "New connection: socket fd is " << new_socket << ", ip is: " <<  inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
            fstream << "Added to the list of sockets as: " << i << std::endl;
            return 1;
        }

        int disconnection_log(struct sockaddr_in address)
        {
            std::ofstream fstream;

            fstream.open("log.txt", std::ios::app);
            if(!fstream.is_open())
            {
                perror("logs");    
            }

            fstream << "Host disconnected, ip is: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
        }
}