#include <sys/types.h>
#include <sys/socket.h>


void print_usage_details (void);
int start_server (int argc, char **argv);


// Global Definitions
#define MAX_WAIT_QUEUE 10


// Global Variables
//      Look at making these not global
int server_port;
