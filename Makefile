GENFLAGS := -Wall -Wextra -g3 -pedantic -std=c99 -Wvla -Wfloat-equal -Werror -fsanitize=undefined 

# -O2 means optimise comiled code for execution speed
OPTIM := $(GENFLAGS) 

# In the labs you'll proably use clang, but make sure to try gcc too
CC := clang

all: ms

ms: ms.c drv.c ms.h
		$(CC) ms.c drv.c $(OPTIM) -o ms

clean:
		rm -f ms

run: all
		./ms
