#include "main.h"

//

std::ofstream fstream;

void open_file(std::ofstream& fstream)
{
    fstream.open("log.txt");
    if(!fstream.is_open())
    {
        perror("logs");    
    }
}

void main_log()
{
    open_file(fstream);

    std::time_t t = std::time(nullptr);

    fstream << std::put_time(std::localtime(&t), "%c %Z") <<": Server started!" << std::endl;
    fstream << "\t\t\tListener on port: " << _PORT_ << std::endl;
    fstream << "\t\t\tWaiting for connections..." << "\n\n\n" << std::endl;
}

void connections_log(int new_socket, struct sockaddr_in address, int i)
{
    std::time_t t = std::time(nullptr);
    
    fstream << std::put_time(std::localtime(&t), "%c %Z") << ": New connection: socket fd is " << new_socket << ", ip is: " <<  inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
    fstream << "\t\t\tAdded to the list of sockets as: " << i << "\n\n " << std::endl;
}

void disconnections_log(struct sockaddr_in address)
{
    std::time_t t = std::time(nullptr);
    
    fstream << std::put_time(std::localtime(&t), "%c %Z") << ": Host disconnected, ip is: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
}

void actions_log(std::string action)
{
    std::time_t t = std::time(nullptr);
    
    fstream << "\n\n" << std::put_time(std::localtime(&t), "%c %Z") << ": " << action << "\n\n" << std::endl;
}

void errors_log(std::string error)
{
    std::time_t t = std::time(nullptr);
    
    fstream << "\n\n\n" << std::put_time(std::localtime(&t), "%c %Z") << ": " << error << "\n\n" << "Server stopped!" << std::endl; 
}