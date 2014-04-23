// Headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "simple_rcp.h"


////////////////////////////////////////
// Forward declarations for the client.
////////////////////////////////////////
int parse_client_args (int argc, char **argv);


////////////////////////////////////////
//  Function: start_client
//
//  Purpose: Parse the arguments for starting a client.
//
//  Parameters:
//          int argc - argument count
//          char **argv - the actual arguments
//
//  Return Value:
//          0 - client started and listening on port
//          -1 - Bad arguments
////////////////////////////////////////
int start_client (int argc, char **argv)
{
    int returnValue;
    int socket_id, data_socket_id;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    if (parse_client_args (argc, argv) == -1)
        return -1;

    if (setup_client_socket (&socket_id) == -1)
        return -1;

    // Handle sending the file.

    if (close_socket() == -1)
        return -1;    

    return 0;
}


////////////////////////////////////////
//  Function: parse_client_args
//
//  Purpose: Parse the arguments for starting a client.
//
//  Parameters:
//          int argc - argument count
//          char **argv - the actual arguments
//
//  Return Value:
//          0 - Valid arguments
//          -1 - Bad arguments
////////////////////////////////////////
int parse_client_args (int argc, char **argv)
{
    if (argc < 3)
    {
        print_usage_details();
        printf ("Starting a client\n");
        printf ("Had less than 3 arguments\n.");
        return -1;
    }

    // Get the port if it was provided.
    if (strncmp (argv[2], "--port=", 7) != 0)
    {
    }
    else // Set the default port
    {
        
    }

    return 0;
}

