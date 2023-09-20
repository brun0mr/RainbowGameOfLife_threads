CC = gcc
CFLAGS = 

# Rule for RainbowGameOfLife_OpenMP.c
RainbowGameOfLife_OpenMP: RainbowGameOfLife_OPenMP.c
	$(CC) $(CFLAGS) -fopenmp -o OpenMP $< 

# Rule for RainbowGameOfLife_PThreads.c
RainbowGameOfLife_PThreads: RainbowGameOfLife_PThreads.c
	$(CC) $(CFLAGS) -lpthread -o PThreads $< 

# Rule for RainbowGameOfLife_Serial.c
RainbowGameOfLife_Serial: RainbowGameOfLife_Serial.c
	$(CC) $(CFLAGS) -o Serial $< 

all: RainbowGameOfLife_OpenMP RainbowGameOfLife_PThreads RainbowGameOfLife_Serial


time: RainbowGameOfLife_OpenMP RainbowGameOfLife_PThreads RainbowGameOfLife_Serial
	@echo "Timing RainbowGameOfLife_OpenMP..."
	@time ./OpenMP
	@echo "\nTiming RainbowGameOfLife_PThreads..."
	@time ./PThreads
	@echo "\nTiming RainbowGameOfLife_Serial..."
	@time ./Serial

clean:
	rm -f Serial PThreads OpenMP
