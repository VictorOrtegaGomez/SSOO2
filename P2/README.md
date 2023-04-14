# P2

## Folders and Files

- `src`: It contains source files
- `include`: It contains headers files
- `obj`: It contains the object files when compilation is made. It will be created after **Makefile** is executed
- `exec`: It contains the binary files when compilation is made. It will be created after **Makefile** is executed
- `textos`: It contains some example texts to try out the software
- `Makefile`: It will allow us to automize compilation and processes execution

## Makefile

In order to compile the whole project we will hace to use:

> make

Every source file will be compiled after said sentence is executed, and every object and binary files will be put in the folders they belong to


In order to clean our project folder we can also make use of the **Makefile** file. We can do this with:

> make clean

`exec`, `obj`, `log.txt`, `estudiantes` will be completed deleted if they existed.


After every source file compilation is done, we can test our software with:

> make test
