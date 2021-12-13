CC = g++
CFLAGS = -I.
OBJ = bf.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

bf: bf.o
	$(CC) -o $@ $^ $(CFLAGS)