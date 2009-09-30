CC = cc
CFILES = benchmark.c  file.c  protocol.c  main.c  parse.c update.c
CFLAGS = -lcurl -Wall -pedantic-errors
DEBUG = -g
TARGET = apt-spy

$(TARGET) : $(CFILES)
	$(CC) -o $(TARGET) $(CFLAGS) $(DEBUG) $(CFILES)

clean :
	rm -f $(TARGET) *.o
