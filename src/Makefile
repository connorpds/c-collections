CC = clang 
CFLAGS =  -g3 -O0 -Wall -std=c99 -Wno-incompatible-pointer-types -Wno-pointer-sign -Wno-format #-fsanitize=address
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer

#sources 
SOURCES := $(wildcard *.c) $(wildcard utils/*.c) 
HEADERS := $(wildcard *.h) $(wildcard utils/*.h)

#executable
EXEC = vector_test


#object files to produce and/or link 
OBJECTS = $(SOURCES:.c=.o)

#pattern rule for sources and headers 
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


$(EXEC):	$(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS)

#default target 
all : $(EXEC)


clean:	
	rm -rf $(EXEC) *.o  

.PHONY: all clean
