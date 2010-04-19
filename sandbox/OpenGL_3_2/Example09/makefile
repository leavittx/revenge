INCLUDE_DIRS = -I../GLUS -I../External/include
LIBRARY_DIRS = -L../GLUS -L../External/lib
LIBRARIES = -lGLUS -lGLEW -lGL -lm -lrt

NAME = Example09

all: main

main: 
	   gcc $(INCLUDE_DIRS) $(LIBRARY_DIRS) main.c $(LIBRARIES) -o../Binaries/$(NAME)

clean:
	   rm ../Binaries/$(NAME)
