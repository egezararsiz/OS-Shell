mysh: shell.c interpreter.c shellmemory.c pcb.c kernel.c cpu.c
	gcc -D FRAMESIZE=$(framesize) -D VARMEMSIZE=$(varmemsize) -c shellmemory.c shell.c interpreter.c pcb.c kernel.c cpu.c
	gcc -o mysh shellmemory.o shell.o interpreter.o pcb.o cpu.o kernel.o

clean: 
	rm mysh; rm *.o
