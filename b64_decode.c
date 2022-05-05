#include "b64.h"

static const unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const unsigned char padding = '=';

static int dec_to_bin(int dec)
{
    int bin;
    int base = 1;

    for (int mod; dec;) {
        mod = bin % 2;
        bin += mod * base;
        dec /= 2;
        base *= 10;
    }
    return bin;
}

static int return_alphabet_index(char current_char)
{
    for (int i = 0; alphabet[i]; ++i) {
        if (current_char == alphabet[i])
            return dec_to_bin(i);
    }
    return -1;
}


static char *fill_buffer(char *buffer, int bin)
{
    int copy = bin;

    for (int i = 5; i >= 0; --i) {
        if (copy == 0)
            buffer[i] = '0';
        buffer[i] = copy % 10 + '0';
        copy /= 10;
    }
    return buffer;
}

char *b64_decode(char *str)
{
    if (!str)
        return NULL;

    int string_len = 0;

    for (int i = 0; str[i]; ++i)
        if (str[i] != padding)
            ++string_len;

    int four_bytes_grouping = string_len / 4;
    int less_than_four = string_len % 4;
    char *b64_decoded;
    char *buffer = malloc(1 + 6 * sizeof(char));
    char *twenty_four_bits = malloc(1 + 24 * sizeof(char));
    buffer[6] = 0;
    twenty_four_bits[24] = 0;

    if (!less_than_four) {
        b64_decoded = malloc((four_bytes_grouping * 3 + 1) * sizeof(char));
        b64_decoded[four_bytes_grouping * 3] = 0;
    } else {
        if (less_than_four == 3) {
            b64_decoded = malloc((four_bytes_grouping * 4) * sizeof(char));
            b64_decoded[four_bytes_grouping * 4 - 1] = 0;
        } else if (less_than_four == 2) {
            b64_decoded = malloc((four_bytes_grouping * 3 + 2) * sizeof(char));
            b64_decoded[four_bytes_grouping * 3 + 1] = 0;
        } else
            return NULL;
    }
    return NULL;
}