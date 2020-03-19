#include "main.h"

int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , client_socket[30] , max_clients = 30, i;
	int max_sd;
    struct sockaddr_in address;
     
    char buffer[2049];  //data buffer of 2K
     
    //set of socket descriptors
    fd_set readfds;
     
    //a test message
    /*const*/ char *message = /*std::string(*/ "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\n <html><head><h1>HELLO, FROM SERVER, MY FRIEND!</h1></head><body><h2>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p></body></html>"/*).c_str()*/;
 
    //initialise all client_socket[] to 0 (as not checked)
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
     
    //create a master_socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    //set master_socket to allow multiple connections
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
 
    //filling basic struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
     
    //bind the socket to localhost port
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	printf("Listener on port %d \n", PORT);
	
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
     
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    //start waiting for an activity
    multiplexing(master_socket, max_clients, readfds, client_socket, address, addrlen, message, buffer);

    return 0;
}