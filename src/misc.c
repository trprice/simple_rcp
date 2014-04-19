#include "simple_rcp.h"
#include <stdio.h>

void print_usage_details (void)
{
    printf ("simple_rcp - Remote Copy Client and Server\n");
    printf ("Usage details:\n");
    printf ("To start a server: simple_rcp --start-server --port=XXXX\n");
    printf ("To start a client: simple_rcp DESIGN THESE PARAMETERS\n");
}
