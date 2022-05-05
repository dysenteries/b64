#include "b64.h"

static const unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const unsigned char padding = '=';

static char *reverse_bits(char *bits, int loop)
{
    int start;
    int end;

    for (int i = 0; i < loop; ++i) {
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
    for (int mod; bin;) {
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
                for (int bit = 0; bit < 8; ++bit)
                    twenty_four_bits[char_in_group * 8 + bit] = ((str[group * 3 + char_in_group] >> bit) & 1) + '0';
            twenty_four_bits = reverse_bits(twenty_four_bits, 3);
            for (int i = 0, dec = 0, six_bits_grouping = 0; i < 4; ++i, six_bits_grouping += 6, ++index) {
                if ((dec = six_bits_bin_to_dec(&twenty_four_bits[six_bits_grouping])) == -1)
                    return NULL;
                b64_encoded[index] = alphabet[dec];
            }
        }
        if (!less_than_three)
            free(twenty_four_bits);
    }
    if (less_than_three) {
        if (less_than_three == 2) {
            twenty_four_bits[18] = 0;
            for (int i = 0; i < 2; ++i)
                for (int bit = 0; bit < 8; ++bit)
                    twenty_four_bits[i * 8 + bit] = ((str[three_bytes_grouping * 3 + i] >> bit) & 1) + '0';
            twenty_four_bits = reverse_bits(twenty_four_bits, 2);
            for (int i = 0; i < 2; ++i)
                twenty_four_bits[16 + i] = '0';
            for (int i = 0, dec = 0, six_bits_grouping = 0; i < 3; ++i, six_bits_grouping += 6) {
                if ((dec = six_bits_bin_to_dec(&twenty_four_bits[six_bits_grouping])) == -1)
                    return NULL;
                b64_encoded[three_bytes_grouping * 4 + i] = alphabet[dec];
            }
        b64_encoded[three_bytes_grouping * 4 + 3] = padding;
        } else if (less_than_three == 1) {
            twenty_four_bits[12] = 0;
            for (int bit = 0; bit < 8; ++bit)
                twenty_four_bits[bit] = ((str[three_bytes_grouping * 3] >> bit) & 1) + '0';
            twenty_four_bits = reverse_bits(twenty_four_bits, 1);
            for (int i = 0; i < 4; ++i)
                twenty_four_bits[8 + i] = '0';
            for (int i = 0, dec = 0, six_bits_grouping = 0; i < 2; ++i, six_bits_grouping += 6) {
                if ((dec = six_bits_bin_to_dec(&twenty_four_bits[six_bits_grouping])) == -1)
                    return NULL;
                b64_encoded[three_bytes_grouping * 4 + i] = alphabet[dec];
            }
            for (int i = 2; i > 0; --i)
                b64_encoded[three_bytes_grouping * 4 + 4 - i] = padding;
        }
        free(twenty_four_bits);
    }
    return b64_encoded;
}

char *b64url_encode(char *str)
{
    char *b64 = b64_encode(str);

    if (!b64)
        NULL;
    for (int i = 0; b64[i]; ++i) {
        if (b64[i] == '+')
            b64[i] = '-';
        if (b64[i] == '/')
            b64[i] = '_';
    }
    return b64;
}
