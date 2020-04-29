#include "main.h"

int MAXIMUM_CONNECTIONS;
int _PORT_;
std::string FILES_PATH;

int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , client_socket[10] , max_clients = 10, i;
	int max_sd;
    struct sockaddr_in address;

    main_log();
    load_cfg_file();

    std::cout << "MAX_CON : " << MAXIMUM_CONNECTIONS << "\n_PORT_ : " << _PORT_ << "\nF_PATH : " << FILES_PATH << std::endl; 
     
    //set of socket descriptors
     
    //a test message
    //    /*const*/ char *message = /*std::string(*/ "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\n <html><head><h1>HELLO, FROM SERVER, MY FRIEND!</h1></head><body><h2>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p></body></html>"/*).c_str()*/;
 
    //initialise all client_socket[] to 0 (as not checked)
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
     
    //create a master_socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        errors_log("SOCKET FAILED!");
        exit(EXIT_FAILURE);
    }
 
    //set master_socket to allow multiple connections
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        errors_log("SETSOCKOPT FAILED!");
        exit(EXIT_FAILURE);
    }
 
    //filling basic struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( _PORT_ );
     
    //bind the socket to localhost port
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        errors_log("BIND FAILED!");
        exit(EXIT_FAILURE);
    }

	
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        errors_log("LISTEN FAILED!");
        exit(EXIT_FAILURE);
    }
     
    //accept the incoming connection
    addrlen = sizeof(address);

    //start waiting for an activity
    multiplexing(master_socket, max_clients, client_socket, address, addrlen);

    return 0;
}