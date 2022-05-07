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

static int bin_to_dec(char *binary_string, int mode)
{
    int bin = 0;
    int dec = 0;
    char decode[8];
    char encode[6];

    if (mode) { for (int i = 0; i < 8; ++i) decode[i] = binary_string[i]; bin = atoi(decode); }
    if (!mode) { for (int i = 0; i < 6; ++i) encode[i] = binary_string[i]; bin = atoi(encode); }

    for (int mod, base = 1; bin > 0; bin /= 10) { mod = bin % 10; dec += mod * base; base *= 2; }
    return dec;
}

static char *malloc_decoded_string(int div, int mod)
{
    char *decoded_string;

    if (!mod) {
        if (!(decoded_string = malloc(1 + (div * 3) * sizeof(char)))) return NULL;
        decoded_string[div * 3] = 0;
    } else {
        if (mod == 2) { if (!(decoded_string = malloc(2 + (div * 3) * sizeof(char)))) return NULL; decoded_string[(div * 3) + 1] = 0; }
        if (mod == 3) { if (!(decoded_string = malloc(3 + (div * 3) * sizeof(char)))) return NULL; decoded_string[(div * 3) + 2] = 0; }
    }
    return decoded_string;
}

static int length_without_padding(char *str)
{
    int length = 0;

    for (int i = 0; str[i]; ++i)
        if (str[i] != '=')
            ++length;
    return length;
}

static int dec_to_bin(int dec)
{
    int bin = 0;

    for (int mod, base = 1; dec > 0; dec /= 2) { mod = dec % 2; bin += mod * base; base *= 10; }
    return bin;
}

static char *get_bits(char *bytes, int bytes_nb)
{
    char *bits = malloc(1 + 24 * sizeof(char));

    if (!bits)
        return NULL;
    bits[bytes_nb * 6] = 0;
    for (int i = 0; i < bytes_nb * 6; ++i) bits[i] = '0';
    for (int i = 0, bin; i < bytes_nb; ++i) {
        for (int r = 0; base[r]; ++r) if (base[r] == bytes[i]) bin = dec_to_bin(r);
        for (int r = 5; bin > 0; --r) { bits[i * 6 + r] = bin % 10 + '0'; bin /= 10; }
    }
    return bits;
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
        for (int r = 0; r < 4; ++r) { encoded_string[i * 4 + r] = base[bin_to_dec(&bits[r * 6], 0)]; }
        free(bits);
    }
    if (mod) {
        if (!(bits = extract_bits(&string_to_encode[div * 3], mod)))
            return NULL;
        if (mod == 1) {
            for (int i = 0; i < 4; ++i) bits[i + 8] = '0';
            for (int i = 0; i < 2; ++i) { encoded_string[div * 4 + i] = base[bin_to_dec(&bits[i * 6], 0)]; encoded_string[(div * 4) + 2 + i] = '='; }
        }
        if (mod == 2) {
            for (int i = 0; i < 2; ++i) bits[i + 16] = '0';
            for (int i = 0; i < 3; ++i) encoded_string[div * 4 + i] = base[bin_to_dec(&bits[i * 6], 0)];
            encoded_string[((div + 1) * 4) - 1] = '=';
        }
        free(bits);
    }
    return encoded_string;
}

char *b64_decode(char *string_to_decode)
{
    if (!string_to_decode) return NULL;
    int div = length_without_padding(string_to_decode) / 4;
    int mod = length_without_padding(string_to_decode) % 4;
    char *decoded_string = malloc_decoded_string(div, mod);
    char *bits;

    for (int i = 0; i < div; ++i) {
        if (!(bits = get_bits(&string_to_decode[i * 4], 4)))
            return NULL;
        for (int r = 0; r < 3; ++r) decoded_string[i * 3 + r] = bin_to_dec(&bits[r * 8], 1);
        free(bits);
    }
    if (mod) {
        if (!(bits = get_bits(&string_to_decode[div * 4], mod)))
            return NULL;
        if (mod == 2) decoded_string[div * 3] = bin_to_dec(bits, 1);
        if (mod == 3) for (int i = 0; i < 2; ++i) decoded_string[div * 3 + i] = bin_to_dec(&bits[i * 8], 2);
        free(bits);
    }
    return decoded_string;
}

char *b64url_encode(char *string_to_encode)
{
    if (!string_to_encode)
        return NULL;

    char *encoded_string = b64_encode(string_to_encode);

    for (int i = 0; encoded_string[i]; ++i) {
        if (encoded_string[i] == '+') encoded_string[i] = '-';
        if (encoded_string[i] == '/') encoded_string[i] = '_';
    }
    return encoded_string;
}

char *b64url_decode(char *string_to_decode)
{
    if (!string_to_decode)
        return NULL;

    char *decoded_string;

    for (int i = 0; string_to_decode[i]; ++i) {
        if (string_to_decode[i] == '+') string_to_decode[i] = '-';
        if (string_to_decode[i] == '/') string_to_decode[i] = '_';
    }
    char *decoded_string = b64_decode(string_to_decode);
    return decoded_string;
}
