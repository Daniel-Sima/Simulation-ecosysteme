CC = gcc
#PROGRAMS = test_ecosys ecosys ecosys_param
PROGRAMS = test_ecosys ecosys

CFLAGS = -Wall -Wextra -pedantic -g

.PHONY: all clean
.SUFFIX:

all: $(PROGRAMS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

test_ecosys: ecosys.o main_tests.o
	$(CC) $(CFLAGS) -o $@ $^

ecosys: ecosys.o main_ecosys.o
	$(CC) $(CFLAGS) -o $@ $^

clean: 
	rm -rf *.o $(PROGRAMS)  
