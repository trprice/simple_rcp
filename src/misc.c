#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "simple_rcp.h"

void print_usage_details (void)
{
    printf ("simple_rcp - Remote Copy Client and Server\n");
    printf ("Usage details:\n");
    printf ("To start a server: simple_rcp --start-server [--port=XXXX]\n");
    printf ("To start a client: simple_rcp [--port=XXXX] SOURCE [host:]DEST \n");
}


////////////////////////////////////////
//  Function: close_socket
//
//  Purpose: Create the socket for the server.
//
//  Parameters:
//
//  Return Value:
//          0 - Socket closed successfully
//          -1 - Error when closing socket.
////////////////////////////////////////
int close_socket(int socket_id)
{
    if (close (socket_id) == -1)
    {
        perror ("Close failed\n");
        return -1;
    }

    return 0;
}
