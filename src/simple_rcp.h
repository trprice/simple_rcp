#include <sys/types.h>
#include <sys/socket.h>


int parse_args(int argc, char **argv);
void print_usage_details (void);
int parse_server_args (int argc, char **argv);
int parse_client_args (int argc, char **argv);
int create_socket (void);
int close_socket (int);
