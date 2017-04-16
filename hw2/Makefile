all:
	gcc -o getuid -Wall -g getuid.c
	gcc -o inject1.so -shared -fPIC inject1.c -ldl

test:
	MONITOR_OUTPUT=stderr LD_PRELOAD=./inject1.so ls -al
