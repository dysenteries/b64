#include "b64.h"

static const unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const unsigned char padding = '=';

static char *reverse_bits(char *bits)
{
    int start;
    int end;

    for (int i = 0; i < 3; ++i) {
        start = i * 8;
        end = start + 7;
        for (int r = 0, tmp; r < 4; ++r, ++start, --end) {
            tmp = bits[start];
            bits[start] = bits[end];
            bits[end] = tmp;
        }
    }
    return bits;
}

static int six_bits_bin_to_dec(char *bits)
{
    int bin;
    int dec = 0;
    int base = 1;
    char *six_bits_bin = strndup(bits, 6);

    if (!six_bits_bin)
        return -1;
    bin = atoi(six_bits_bin);
    free(six_bits_bin);
    for (int i = 0, mod; bin; i++) {
        mod = bin % 10;
        dec += mod * base;
        bin /= 10;
        base *= 2;
    }
    return dec;
}

char *b64_encode(char *str)
{
    if (!str)
        return NULL;

    int string_len = strlen(str);
    int three_bytes_grouping = string_len / 3;
    int less_than_three = string_len % 3;
    char *b64_encoded;
    char *twenty_four_bits = malloc(1 + 24 * sizeof(char));
    twenty_four_bits[24] = 0;

    if (!less_than_three) {
        b64_encoded = malloc((three_bytes_grouping * 4 + 1) * sizeof(char));
        b64_encoded[three_bytes_grouping * 4] = 0;
    } else {
        b64_encoded = malloc(((three_bytes_grouping + 1) * 4 + 1) * sizeof(char));
        b64_encoded[(three_bytes_grouping + 1) * 4] = 0;
    }
    if (!b64_encoded)
        return NULL;
    if (three_bytes_grouping) {
        for (int group = 0, index = 0; group < three_bytes_grouping; ++group) {
            for (int char_in_group = 0; char_in_group < 3; ++char_in_group)
                for (int i = 0; i < 8; ++i)
                    twenty_four_bits[char_in_group * 8 + i] = ((str[group * 3 + char_in_group] >> i) & 1) + '0';
            twenty_four_bits = reverse_bits(twenty_four_bits);
            for (int i = 0, dec = 0, six_bits_grouping = 0; i < 4; ++i, six_bits_grouping += 6, ++index) {
                if ((dec = six_bits_bin_to_dec(&twenty_four_bits[six_bits_grouping])) == -1)
                    return NULL;
                b64_encoded[index] = alphabet[dec];
            }
        }
        free(twenty_four_bits);
    }
    if (less_than_three)
        return NULL;
    return b64_encoded;
}
