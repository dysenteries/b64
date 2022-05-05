gcc -c b64_encode.c b64_decode.c -I.
ar -crs libb64.a b64_encode.o b64_decode.o