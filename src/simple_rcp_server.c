#include <string.h>
#include <stdlib.h>
#include "simple_rcp.h"

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
    int numPortChars;
    char *szPort;

    if (argc < 3)
        return -1;

    if (strncmp (argv[2], "--port=", 7) != 0)
        return -1;

    // For clarity take away 7 chars for "--port="
    // and add 1 for the null character.
    numPortChars = strlen (argv[2]) - 7 + 1;
    szPort = malloc (numPortChars * sizeof (char));


    strncpy (szPort, argv[2] + 7, numPortChars);
    szPort[numPortChars] = '\0';


    // Clean up
    free (szPort);

    return 0;
}
