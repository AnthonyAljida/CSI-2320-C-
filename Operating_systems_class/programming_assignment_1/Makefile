CC = gcc
CFLAGS	= -Wall
OBJFILE1 = main.o
OBJFILE2 = task.o

all: main task clean

main: $(OBJFILE1)
	$(CC) $(CFLAGS) -o main $(OBJFILE1) 

task: $(OBJFILE2)
	$(CC) $(CFLAGS) -o task $(OBJFILE2) 

$(OBJFILE1): main.c
	$(CC) $(CFLAGS) -c main.c -o $(OBJFILE1)

$(OBJFILE2): task.c
	$(CC) $(CFLAGS) -c task.c -o $(OBJFILE2)

clean:
	rm -f $(OBJFILE1) $(OBJFILE2) 