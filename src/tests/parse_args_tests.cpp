#include "parse_args_tests.h"

TEST_F (ParseArgsTest, NoArgs) {
    int numArgs = 1;
    char **args;
    int returnValue = -9999;

    args = (char**) malloc (1 * sizeof (char *));

    args[0] = (char*) malloc (11 * sizeof (char));
    sprintf (args[0], "simple_rcp");

    returnValue = parse_args (numArgs, args);

    EXPECT_EQ (-1, returnValue);

    free (args[0]);
    free (args);
}
