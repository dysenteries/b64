// This main is only used to run the tests.

#include "b64.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    char *encoded_string;
    char *decoded_string;

    if (argc > 1) {
        if (!(encoded_string = b64_encode(argv[1])))
            return 84;
        printf("%s\n", encoded_string);
        if (!(decoded_string = b64_decode(encoded_string)))
            return 84;
        printf("%s\n", decoded_string);
        free(encoded_string);
        free(decoded_string);
    }
    return 0;
}
