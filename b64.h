#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef B64_H_
    #define B64_H_

/////////////////////////////////////////////////////////
//  b64_encode -> encode char * passed as parameter    //
//  return value -> encoded char * (or NULL if error)  //
/////////////////////////////////////////////////////////
char *b64_encode(char *string_to_encode);

/////////////////////////////////////////////////////////
//  b64_decode -> decode string passed as parameter    //
//  return value -> decoded string (or NULL if error)  //
/////////////////////////////////////////////////////////
char *b64_decode(char *string_to_decode);

#endif
