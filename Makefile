CFLAGS=-g -Wall
OBJS= f2_poly.o arithm.o
EXE=count_irred count_primitive random_primitive random_irred
all: $(EXE)

count_irred: count_irred.o $(OBJS)
	gcc $(CFLAGS) -o count_irred f2_poly.o count_irred.o arithm.o

count_primitive: count_primitive.o $(OBJS)
	gcc $(CFLAGS) -o count_primitive f2_poly.o count_primitive.o arithm.o
	
random_primitive: random_primitive.o $(OBJS)
	gcc $(CFLAGS) -o random_primitive $(OBJS) random_primitive.o

random_irred: random_irred.o $(OBJS)
	gcc $(CFLAGS) -o random_irred $(OBJS) random_irred.o

	
f2_poly.o: f2_poly.c arithm.h
	gcc -c  $(CFLAGS)  f2_poly.c -o f2_poly.o
	
count_primitive.o:count_primitive.c f2_poly.h
	gcc -c $(CFLAGS) count_primitive.c -o count_primitive.o

count_irred.o: count_irred.c f2_poly.h
	gcc -c $(CFLAGS) count_irred.c -o count_irred.o

random_primitive.o: random_primitive.c f2_poly.h
	gcc -c $(CFLAGS) random_primitive.c -o random_primitive.o


random_irred.o: random_irred.c f2_poly.h
	gcc -c $(CFLAGS) random_irred.c -o random_irred.o

arithm.o: arithm.c
	gcc -c $(CFLAGS) arithm.c -o arithm.o
	
clean:
	find . -name '*.o' -exec rm -f {} ';'
	find . -name '*.a' -exec rm -f {} ';'
	find . -name '*.so' -exec rm -f {} ';'
	find . -name '*~' -exec rm -f {} ';'
	find . -name '#*#' -exec rm -f {} ';'
	find . -name '.#*' -exec rm -f {} ';'
	find . -name 'core' -exec rm -f {} ';'
	find . -name '*.core' -exec rm -f {} ';'


mrproper: clean
	rm -f $(EXE)
