#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simple_rcp.h"


////////////////////////////////////////
// Forward declarations for the server.
////////////////////////////////////////
int parse_server_args (int argc, char **argv);
int setup_server_socket (int *socket_id);
int close_socket ();
int receive_file (int data_socket_id);


////////////////////////////////////////
//  Function: start_server
//
//  Purpose: Parse the arguments for starting a server.
//
//  Parameters:
//          int argc - argument count
//          char **argv - the actual arguments
//
//  Return Value:
//          0 - Server started and listening on port
//          -1 - Bad arguments
////////////////////////////////////////
int start_server (int argc, char **argv)
{
    int returnValue;
    int socket_id, data_socket_id;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    if (parse_server_args (argc, argv) == -1)
        return -1;

    if (setup_server_socket (&socket_id) == -1)
        return -1;

    // Call accept() and let it block until we have a connection.
    while (1)
    {
        client_addr_len = sizeof (client_addr);
        data_socket_id = accept (socket_id,
                                 (struct sockaddr*) &client_addr,
                                 &client_addr_len);

        if (data_socket_id != -1)
        {
            // Call a handling function to receive data and write a file?
            receive_file (data_socket_id);
            
            // Close this data socket.
            close_socket (data_socket_id);
        }
        else
        {
            perror ("Accept returned a bad socket\n");
            return -1;
        }
    }
    
    if (close_socket() == -1)
        return -1;    

    return 0;
}


////////////////////////////////////////
//  Function: parse_server_args
//
//  Purpose: Parse the arguments for starting a server.
//
//  Parameters:
//          int argc - argument count
//          char **argv - the actual arguments
//
//  Return Value:
//          0 - Valid arguments
//          -1 - Bad arguments
////////////////////////////////////////
int parse_server_args (int argc, char **argv)
{
    int numPortChars, counter;
    char *szPort;
    int iPort;

    if (argc < 3)
    {
        print_usage_details();
        printf ("Starting a server\n");
        printf ("Had less than 3 arguments\n.");
        return -1;
    }

    if (strncmp (argv[2], "--port=", 7) != 0)
    {
        print_usage_details();
        return -1;
    }

    // For clarity take away 7 chars for "--port="
    // and add 1 for the null character.
    numPortChars = strlen (argv[2]) - 7 + 1;
    szPort = malloc (numPortChars * sizeof (char));


    strncpy (szPort, argv[2] + 7, numPortChars);
    szPort[numPortChars] = '\0';

    counter = 0;

    while (szPort[counter] != '\0')
    {
        if (!isdigit (szPort[counter]))
        {
            print_usage_details();
            printf ("Found a character in the port number\n");
            return -1;
        }

        counter++;
    }

    iPort = atoi (szPort);
    
    if (iPort > 65535)
    {
        print_usage_details();
        printf ("Port number was greater than 65535\n");
        return -1;
    }

    server_port = iPort;


    // Clean up
    free (szPort);

    return 0;
}


////////////////////////////////////////
//  Function: setup_server_socket
//
//  Purpose: Create the socket for the server.
//
//  Parameters:
//      int *socket_id - Receives the socket id.
//
//  Return Value:
//          0 - Socket opened and bound
//          -1 - Something failed. The step that failed will print with perror
//
//  Notes:
//      A socket of type AF_INET is always used.
//          - What does this imply for the client?
//              - Always having to specify IP?
////////////////////////////////////////
int setup_server_socket(int *socket_id)
{
    struct sockaddr_in addrport;

    // Create the socket
    *socket_id = socket (AF_INET, SOCK_STREAM, 0);

    if (*socket_id == -1)
    {
        perror ("Socket creation failed\n");
        return -1;
    }


    // Bind the socket to a port
    addrport.sin_family = AF_INET;
    addrport.sin_port = htons (server_port);
    addrport.sin_addr.s_addr = htonl (INADDR_ANY);

    if (bind (*socket_id, (struct sockaddr *) &addrport, sizeof (addrport)) == -1)
    {
        perror ("Bind failed\n");
        return -1;
    }


    // Set the server port to listen.
    if (listen (*socket_id, MAX_WAIT_QUEUE) == -1)
    {
        perror ("Listen failed\n");
        return -1;
    }


    return 0;
}


////////////////////////////////////////
//  Function: receive_file
//
//  Purpose: Communicate through the data socket to receive the incoming file.
//
//  Parameters:
//      int data_socket_id - The socket to communicate through.
//
//  Return Value:
//          0 - Success
//          -1 - Error
////////////////////////////////////////
int receive_file (int data_socket_id)
{
    ssize_t message_size, written_size;
    size_t file_name_buf_size = PATH_MAX + NAME_MAX + 1;
    size_t buffer_size = PIPE_BUF + 1;
    char file_name_buf[file_name_buf_size], file_buffer[buffer_size];
    int file_id;


    // Receive file name
    if ((message_size = recv (data_socket_id, file_name_buf, file_name_buf_size, 0)) < 0)
    {
        perror ("recv of file name failed\n");
        return -1;
    }

    
    // Open file with the given file name.
    //     Note that this will create the file with the same ownership and
    //     permissions as the running process. 
    file_id = open (file_name_buf, O_CREAT|O_RDWR|O_TRUNC);
    

    // Receive file data & write it
    do {
        if ((message_size = recv (data_socket_id, file_buffer, buffer_size, 0)) < 0)
        {
            perror ("recv of file data failed\n");
            close (file_id);
            return -1;
        }

        if ((written_size = write (file_id, file_buffer, message_size)) < 0)
        {
            perror ("write to file failed\n");
            close (file_id);
            return -1;
        }
    } while (message_size > 0);

    close (file_id);

    return 0;
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
