# build an executable named nolife from nolife.c
all: nolife.c
	gcc -g -Wall -o nolife nolife.c

clean:
	$(RM) nolife