// Headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <linux/limits.h>
#include "simple_rcp.h"


////////////////////////////////////////
// Forward declarations for the client.
////////////////////////////////////////
#include "simple_rcp.h"


////////////////////////////////////////
// Forward declarations for the client.
////////////////////////////////////////
int parse_client_args (int argc, char **argv, int *client_port, char *from_file_name, char *host, char *to_file_name);


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
    int client_port;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    char from_file_name[MAX_PATH_FILE_NAME_LEN];
    char to_file_name[MAX_PATH_FILE_NAME_LEN];
    char host[16]; // 12 for digits + 3 for dots + 1 null

    if (parse_client_args (argc, argv, &client_port, from_file_name, host, to_file_name) == -1)
        return -1;


    //if (setup_client_socket (&socket_id) == -1)
    //    return -1;

    // Handle sending the file.

    //if (close_socket() == -1)
    //    return -1;    

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
int parse_client_args (int argc, char **argv, int *client_port,
                       char *from_file_name, char *host, char *to_file_name)
{
    int numPortChars, counter;
    int iPort;
    int host_length;
    char *szPort;
    char scratch[16 + MAX_PATH_FILE_NAME_LEN]; // 123.567.901.345:<file name> 
    char *token;
    short argv_counter = 1;

    if (argc < 3)
    {
        print_usage_details();
        printf ("Starting a client\n");
        printf ("Had less than 3 arguments\n.");
        return -1;
    }

    // Get the port if it was provided.
    if (strncmp (argv[argv_counter], "--port=", 7) == 0)
    {
        // For clarity take away 7 chars for "--port="
        // and add 1 for the null character.
        numPortChars = strlen (argv[argv_counter]) - 7 + 1;
        szPort = malloc (numPortChars * sizeof (char));
    
    
        strncpy (szPort, argv[argv_counter] + 7, numPortChars);
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
    
        *client_port = iPort;
   
        // Indicate that we should use the next argv bucket.
        argv_counter++; 
    
        // Clean up
        free (szPort);
    }
    else
        *client_port = 1111;

    
    // From file name
    //      Do I need to do any validation of this?
    strncpy (from_file_name, argv[argv_counter], MAX_PATH_FILE_NAME_LEN);
    argv_counter++;


    // To file name
    strncpy (scratch, argv[argv_counter], 16 + MAX_PATH_FILE_NAME_LEN);

    // Try for an IP address
    //      Using strchr here first because strtok did not return NULL when
    //      searching for ":". Figure out why later.
    token = strchr (scratch, ':');

    if (token != NULL)
    {
        token = strtok (scratch, ":");

        // Copy the IP address
        strncpy (host, token, 16);
        host_length = strlen(host) + 1;

        // Get the file name
        strncpy (to_file_name, scratch+host_length, MAX_PATH_FILE_NAME_LEN);
    }
    else
    {
        memset (host, '\0', 16);

        strncpy (to_file_name, scratch, MAX_PATH_FILE_NAME_LEN);
    }
        
    return 0;
}

