gcc main.c ../b64.c -o test -Wall -Wextra -Werror -pedantic -I..
chmod +x test
chmod +x tester.py
./tester.py
rm test