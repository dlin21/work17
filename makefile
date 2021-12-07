all: write.c
	gcc write.c

start:
	gcc control.c
	./a.out -create

run:
	gcc write.c
	./a.out

end:
	gcc control.c
	./a.out -remove
