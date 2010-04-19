INCLUDE_DIRS = -I../External/include

NAME = GLUS

all: GLUS

GLUS:
	   gcc $(INCLUDE_DIRS) -c glus_linux.c glus_load.c glus_matrix.c glus_modelview.c glus_shaderprogram.c glus_shape.c glus_vector.c
	   ar -r lib$(NAME).a *.o

clean:
	   rm *.o lib$(NAME).a
