#include <sys/types.h>
#include <sys/socket.h>


void print_usage_details (void);
int start_server (int argc, char **argv);
int start_client (int argc, char **argv);


// Global Definitions
#define MAX_WAIT_QUEUE 10
#define MAX_PATH_FILE_NAME_LEN PATH_MAX + NAME_MAX + 1
