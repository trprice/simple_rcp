#include <sys/types.h>
#include <sys/socket.h>


void print_usage_details (void);
int start_server (int argc, char **argv);
int start_client (int argc, char **argv);
int create_socket (void);
int close_socket (int);
