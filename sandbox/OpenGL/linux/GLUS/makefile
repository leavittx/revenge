INCLUDE_DIRS = -I../External/include

NAME = GLUS

all: GLUS

GLUS:
	   gcc $(INCLUDE_DIRS) -c ../../src/$(NAME)/glus_linux.c ../../src/$(NAME)/glus_load.c ../../src/$(NAME)/glus_math.c ../../src/$(NAME)/glus_matrix.c ../../src/$(NAME)/glus_modelview.c ../../src/$(NAME)/glus_point.c ../../src/$(NAME)/glus_shaderprogram.c ../../src/$(NAME)/glus_shape.c ../../src/$(NAME)/glus_vector.c
	   mv *.o lib/
	   ar -r lib/lib$(NAME).a lib/*.o

clean:
	   rm lib/*.o lib/lib$(NAME).a
