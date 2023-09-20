CC = gcc
CFLAGS = 

# Detect the operating system
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

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

# Rule for timing based on OS
ifeq ($(DETECTED_OS),Windows)
time: RainbowGameOfLife_OpenMP RainbowGameOfLife_PThreads RainbowGameOfLife_Serial
	@echo "Timing RainbowGameOfLife_OpenMP on Windows..."
	@powershell Measure-Command {.\OpenMP.exe}
	@echo "\nTiming RainbowGameOfLife_PThreads on Windows..."
	@powershell Measure-Command {.\PThreads.exe}
	@echo "\nTiming RainbowGameOfLife_Serial on Windows..."
	@powershell Measure-Command {.\Serial.exe}
else
time: RainbowGameOfLife_OpenMP RainbowGameOfLife_PThreads RainbowGameOfLife_Serial
	@echo "Timing RainbowGameOfLife_OpenMP..."
	@time ./RainbowGameOfLife_OpenMP
	@echo "\nTiming RainbowGameOfLife_PThreads..."
	@time ./RainbowGameOfLife_PThreads
	@echo "\nTiming RainbowGameOfLife_Serial..."
	@time ./RainbowGameOfLife_Serial
endif

# Rule for clean based on OS
ifeq ($(DETECTED_OS),Windows)
clean:
	@powershell Remove-Item -Force RainbowGameOfLife_OpenMP.exe, RainbowGameOfLife_PThreads.exe, RainbowGameOfLife_Serial.exe
else
clean:
	rm -f RainbowGameOfLife_OpenMP RainbowGameOfLife_PThreads RainbowGameOfLife_Serial
endif





