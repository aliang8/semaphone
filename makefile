GCC = gcc -g

all:
	$(GCC) control.c -o control
	$(GCC) client.c -o client

client.c: client.c
	$(GCC) -c client.c -o client

control.c: control.c
	$(GCC) -c control.c -o control

clean:
	rm *.o
	rm *~

control: all
	./control

client: all
	./client

