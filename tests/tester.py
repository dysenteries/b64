#!/usr/bin/env python3

import subprocess, random, string, base64

def get_random_string(length: int) -> str:
    characters = string.ascii_letters + string.digits + string.punctuation
    randstr = ''.join(random.choice(characters) for i in range(length))
    return randstr

def base64_encode(string: str) -> str:
    a = base64.b64encode(string.encode("ascii"))
    string = a.decode("ascii")
    return string

def base64_decode(string: str) -> str:
    a = base64.b64decode(string.encode("ascii"))
    string = a.decode("ascii")
    return string

encoded = 0
decoded = 0

for i in range(500):
    randstr = get_random_string(random.randint(1, 100000))
    process = subprocess.run(['./test', f'{randstr}'], stdout = subprocess.PIPE,  universal_newlines = True)
    output = process.stdout.split('\n')
    encoded_string = base64_encode(randstr)
    decoded_string = base64_decode(encoded_string)
    if encoded_string == output[0]: encoded += 1
    if decoded_string == output[1]: decoded += 1

print(f"TESTS:\nencoded:{encoded}/500\ndecoded:{decoded}/500")
