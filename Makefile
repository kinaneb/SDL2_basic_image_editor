

CC = gcc
CFLAGS = -Wall -lSDL2 -lSDL2_image -lreadline
LDFLAGS = $(CFLAGS)
all: main
main: main.o parser.o windows_event_handler.o executor.o windows_handler.o image_handler.o save.o
main.o: main.c parser.h windows_event_handler.h executor.h
executor.o: executor.c windows_handler.h image_handler.h save.h
parser.o: parser.c executor.h executor.h
windows_handler.o: windows_handler.c windows_event_handler.h
windows_event_handler.o: windows_event_handler.c windows_event_handler.h
image_handler.o: image_handler.c image_handler.h
save.o: save.c save.h
clean:rm -f main*.o
