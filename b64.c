#include "b64.h"

static const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static char *malloc_encoded_string(int div, int mod)
{
    char *encoded_string;

    if (!mod) { 
        if (!(encoded_string = malloc(1 + (div * 4) * sizeof(char)))) return NULL;
    encoded_string[div * 4] = 0;
    } else {
        if (!(encoded_string = malloc(1 + ((div + 1) * 4) * sizeof(char)))) return NULL;
        encoded_string[(div + 1) * 4] = 0;
    }
    return encoded_string;
}

static char *extract_bits(char *bytes, int bytes_nb)
{
    char *bits = malloc(1 + 24 * sizeof(char));

    if (!bits)
        return NULL;
    bits[bytes_nb * 8] = 0;
    for (int i = 0 ; i < bytes_nb; ++i)
        for (int r = 0; r < 8; ++r) { bits[(i * 8) + (7 - r)] = (bytes[i] & (1 << r)) != 0; bits[(i * 8) + (7 - r)] += '0'; }
    return bits;
}

static int bin_to_dec(char *binary_string)
{
    int bin = 0;
    int dec = 0;
    char a[6];

    for (int i = 0; i < 6; ++i)
        a[i] = binary_string[i];

    bin = atoi(a);
    for (int mod, base = 1; bin > 0; bin /= 10) { mod = bin % 10; dec += mod * base; base *= 2; }
    return dec;
}

char *b64_encode(char *string_to_encode)
{
    if (!string_to_encode) return NULL;
    int div = strlen(string_to_encode) / 3;
    int mod = strlen(string_to_encode) % 3;
    char *encoded_string = malloc_encoded_string(div, mod);
    char *bits;

    if (!encoded_string)
        return NULL;
    for (int i = 0; i < div; ++i) {
        if (!(bits = extract_bits(&string_to_encode[i * 3], 3)))
            return NULL;
        for (int r = 0; r < 4; ++r) { encoded_string[i * 4 + r] = base[bin_to_dec(&bits[r * 6])]; }
    }
    if (mod) {
        if (!(bits = extract_bits(&string_to_encode[div * 3], mod)))
            return NULL;
        if (mod == 1) {
            for (int i = 0; i < 4; ++i) bits[i + 8] = '0';
            for (int i = 0; i < 2; ++i) { encoded_string[div * 4 + i] = base[bin_to_dec(&bits[i * 6])]; encoded_string[(div * 4) + 2 + i] = '='; }
        }
        if (mod == 2) {
            for (int i = 0; i < 2; ++i) bits[i + 16] = '0';
            for (int i = 0; i < 3; ++i) encoded_string[div * 4 + i] = base[bin_to_dec(&bits[i * 6])];
            encoded_string[((div + 1) * 4) - 1] = '=';
        }
    }
    return encoded_string;
}
