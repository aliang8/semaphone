all:
	gcc control.c -o control
	gcc client.c -o client

client.c: client.c
	gcc -c client.c -o client

control.c: control.c
	gcc -c control.c -o control

clean:
	rm *.o
	rm *~

control: all
	./control

client: all
	./client
