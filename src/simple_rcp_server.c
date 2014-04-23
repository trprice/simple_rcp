#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "simple_rcp.h"


////////////////////////////////////////
// Forward declarations for the server.
////////////////////////////////////////
int parse_server_args (int argc, char **argv, int *server_port);
int setup_server_socket (int *socket_id, int *server_port);
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
    int server_port;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    if (parse_server_args (argc, argv, &server_port) == -1)
        return -1;

    if (setup_server_socket (&socket_id, &server_port) == -1)
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
            printf ("Accept returned a bad socket\n");
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
int parse_server_args (int argc, char **argv, int *server_port)
{
    int numPortChars, counter;
    char *szPort;
    int iPort;

    if (argc < 2)
    {
        print_usage_details();
        printf ("Starting a server\n");
        printf ("Had less than 2 arguments\n.");
        return -1;
    }
    else if (argc == 2)
    {
        *server_port = 1111;
        return 0;
    }

    if (strncmp (argv[2], "--port=", 7) == 0)
    {
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
    
        *server_port = iPort;
    
    
        // Clean up
        free (szPort);
    }
    else
        *server_port = 1111;

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
//          -1 - Something failed. The step that failed will print with printf
//
//  Notes:
//      A socket of type AF_INET is always used.
//          - What does this imply for the client?
//              - Always having to specify IP?
////////////////////////////////////////
int setup_server_socket(int *socket_id, int *server_port)
{
    struct sockaddr_in addrport;

    // Create the socket
    *socket_id = socket (AF_INET, SOCK_STREAM, 0);

    if (*socket_id == -1)
    {
        printf ("Socket creation failed\n");
        return -1;
    }


    // Bind the socket to a port
    addrport.sin_family = AF_INET;
    addrport.sin_port = htons (*server_port);
    addrport.sin_addr.s_addr = inet_addr ("127.0.0.1");

    if (bind (*socket_id, (struct sockaddr *) &addrport, sizeof (addrport)) == -1)
    {
        printf ("Bind failed\n");
        return -1;
    }


    // Set the server port to listen.
    if (listen (*socket_id, MAX_WAIT_QUEUE) == -1)
    {
        printf ("Listen failed\n");
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
    size_t file_name_buf_size = MAX_PATH_FILE_NAME_LEN;
    size_t buffer_size = PIPE_BUF + 1;
    char file_name_buf[file_name_buf_size], file_buffer[buffer_size];
    FILE *file_id;
    int counter;


    // Receive file name length
    if ((message_size = recv (data_socket_id, file_name_buf, file_name_buf_size, MSG_PEEK)) < 0)
    {
        printf ("recv of file name length failed\n");
        return -1;
    }

    counter = 0;

    while (isdigit(file_name_buf[counter]))
        counter++;


    strncpy (file_buffer, file_name_buf, counter);
    file_name_buf_size = atoi (file_buffer);

    printf ("file_name_buf_size = %d\n", (int) file_name_buf_size);
    printf ("counter = %d\n", counter);

    // Clear the buffer
    memset (file_name_buf, '\0', file_name_buf_size);
    
    // Remove the length from the queue
    if ((message_size = recv (data_socket_id, file_name_buf, counter, 0)) < 0)
    {
        printf ("Removal of length from queue failed\n");
        return -1;
    }

    printf ("file_name_buf:%s\n", file_name_buf);

    // Clear the buffer
    memset (file_name_buf, '\0', file_name_buf_size);
    
    // Receive file name
    //      Error here. The file name isn't as long as the max and we're
    //      reading EVERYTHING out of the buffer.
    //      - How can we know how many bytes to read?
    if ((message_size = recv (data_socket_id, file_name_buf, file_name_buf_size, 0)) < 0)
    {
        printf ("recv of file name failed\n");
        return -1;
    }

    printf ("file_name_buf:%s\n", file_name_buf);

    
    // Open file with the given file name.
    //     Note that this will create the file with the same ownership and
    //     permissions as the running process. 
    file_id = fopen (file_name_buf, "w+");
    

    // Receive file data & write it
    do {
        // Clear the buffer
        memset (file_buffer, '\0', buffer_size);

        if ((message_size = recv (data_socket_id, file_buffer, buffer_size, 0)) < 0)
        {
            printf ("recv of file data failed\n");
            fclose (file_id);
            return -1;
        }

        if ((written_size = fwrite (file_buffer, 1, message_size, file_id)) < message_size)
        {
            printf ("write to file failed\n");
            fclose (file_id);
            return -1;
        }
    } while (message_size > 0);

    fclose (file_id);

    return 0;
}
