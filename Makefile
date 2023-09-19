CC = gcc
CFLAGS = -Wall
LIBS = 

# Rule for RainbowGameOfLife_OpenMP.c
RainbowGameOfLife_OpenMP: RainbowGameOfLife_OpenMP.c
	$(CC) $(CFLAGS) -fopenmp -o $@ $< $(LIBS)

# Rule for RainbowGameOfLife_PThreads.c
RainbowGameOfLife_PThreads: RainbowGameOfLife_PThreads.c
	$(CC) $(CFLAGS) -lpthread -o $@ $< $(LIBS)

# Rule for RainbowGameOfLife_Serial.c
RainbowGameOfLife_Serial: RainbowGameOfLife_Serial.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

all: RainbowGameOfLife_OpenMP RainbowGameOfLife_PThreads RainbowGameOfLife_Serial

clean:
	rm -f RainbowGameOfLife_OpenMP RainbowGameOfLife_PThreads RainbowGameOfLife_Serial
