#include "simple_rcp.h"
#include <string.h>

// Forward Declarations
//      Here so that the program logic in main comes first and makes the flow
//      of the program easy to read.
int parse_args (int argc, char **argv);


////////////////////////////////////////
//  Function: main
//
//  Purpose: Entry point to the program.
//
//  Parameters:
//          int argc - argument count
//          char **argv - the actual arguments
//
//  Return Value:
//          0 - Normal Run
////////////////////////////////////////
int main (int argc, char **argv)
{
    int result;

    result = parse_args(argc, argv);

    if (result == 1)
        parse_server_args (argc, argv);
    //else if (result == 2)
    //    parse_client_args (argc, argv);
    else
        return 0;


    return 0;
}


////////////////////////////////////////
//  Function: parse_args
//
//  Purpose: Parse the top level arguments to determine if we're starting
//           a server or starting a client. Don't do any validation of the
//           arguments other than to determine whether we're starting a server.
//           We'll pass along the arguments to the appropriate argument
//           parsing routine.
//
//  Parameters:
//          int argc - argument count
//          char **argv - the actual arguments
//
//  Return Value:
//          1 - Start a server
//          2 - Start a client
//          -1 - Bad arguments
////////////////////////////////////////
int parse_args (int argc, char **argv)
{
    if (argc > 2)
    {
        if (strncmp (argv[2], "--start-server", 14) == 0)
            return 1;
        else
            return 2;
    }

    print_usage_details();
    return -1;
}
