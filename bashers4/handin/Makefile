
.IGNORE:
.SILENT:

all: bank threadperf

bank: bank.c
	gcc -Wall -lpthread -g bank.c -o bank

threadperf: threadperf.c 
	gcc -Wall -lpthread -g threadperf.c -o threadperf
