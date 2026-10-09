CFLAGS = -g -Wall
RAYLIB_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
MAIN_NAME = tetris

default:
	gcc $(CFLAGS) $(MAIN_NAME).c -o $(MAIN_NAME) $(RAYLIB_FLAGS) 

.PHONY: clean
clean:
	-rm $(MAIN_NAME)


