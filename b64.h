#include <string.h>
#include <stdlib.h>

#ifndef B64_H_
    #define B64_H_

//  b64_encode -> encode in base64 a string passed as parameter
//  return value -> encoded string (or NULL if error)
char *b64_encode(char *string_to_encode);

//  b64url_encode -> encode in base64 a string passed as parameter and replace ('+' and '/') by ('-' and '_')
//  return value -> encoded string (or NULL if error)
char *b64url_encode(char *string_to_encode);

//  b64_decode -> decode a base64 string passed as parameter
//  return value -> decoded string (or NULL if error)
char *b64_decode(char *string_to_decode);

//  b64_decode -> decode a base64url string passed as parameter
//  return value -> decoded string (or NULL if error)
char *b64url_decode(char *string_to_decode);

#endif
