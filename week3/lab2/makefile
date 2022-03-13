CC = gcc
TARGET =  A B
INCLUDE = -I ./

all : $(TARGET)

A : A.o common.o
	$(CC) -o A A.o common.o -lpthread
B : B.o common.o
	$(CC) -o B B.o common.o -lpthread

common.o : common.c
	$(CC) -o $@ -c $< $(INCLUDE)

A.o : A.c
	$(CC) -o $@ -c $<  $(INCLUDE)

B.o : B.c
	$(CC) -o $@ -c $<  $(INCLUDE)

realclean:
	rm A B A.o B.o common.o
clean:
	rm A.o B.o common.o
