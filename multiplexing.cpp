#include "main.h"

char buffer_[2049];
int count_of_connections = -1;

int multiplexing(int master_socket, int max_clients, fd_set readfds, int *client_socket, struct sockaddr_in address, int addrlen)
{
    int activity, valread, sd, max_sd, i, new_socket;
    sock_to_reqStruct StRs[MAXIMUM_CONNECTIONS];

    memset(&buffer_, 0, sizeof(buffer_));

	while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
 
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
		
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
			sd = client_socket[i];
            
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
            
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
				max_sd = sd;
        }
 
        //wait for an activity on one of the sockets , timeout is NULL , so wait...
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
   
        if ((activity < 0) && (errno!=EINTR)) 
        {
            errors_log("SELECT FAILED!");
        }
         
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                errors_log("ACCEPT FAILED!");
                exit(EXIT_FAILURE);
            }

            //send new connection greeting message
            //if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            //{
                //errors_log("SEND FAILED!");                                 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //}
             
            //actions_log("Welcome message sent!");
             
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
				if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
					connections_log(new_socket, address, i);
                    StRs[i].sock = new_socket;
                    StRs[i].reqStruct = i;     //pozhe razobratbsya s etoi heresy
                    count_of_connections++;

					break;
                }
            }
        }
         
        //its some IO operation 
        for (int j = 0; j < max_clients; j++) 
        {
            sd = client_socket[j];
             
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and read the incoming message
                if ((valread = read( sd , buffer_, 1024)) == 0)
                {
                    //Somebody disconnected , print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    disconnections_log(address);

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[j] = 0;
                }
                 
                //Echo back the message that came in
                else
                {   
                    parse_startline(new_socket);
                    //set the string terminating NULL byte on the end of the data read
                    //buffer_[valread] = '\0';
                    //send(sd , buffer_ , strlen(buffer_) , 0 );
                }
            }
        }
    }
     
    return 0;
}
