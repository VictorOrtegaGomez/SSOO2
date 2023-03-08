# P1

## Folders and Files

- `src`: It contains source files
- `include`: It contains headers files
- `obj`: It contains the object files when compilation is made. It will be created after MAKEFILE is executed
- `exec`: It contains the binary files when compilation is made. It will be created after MAKEFILE is executed
- `estudiantes`: It contains the result folders and files from **manager**, **pa**, **pb** and **pc** execution. It will be created after normal execution
- `MODELOSEXAMEN`: It contains the exam models files that need to be copied to the folders in **estudiantes**
- `estudiantes.txt`: File from where the information needed by **pa**, **pb** and **pc** processes will be read
- `log.txt`: File created and written by the manager process. It will contain the information regarding the project execution.
- `Makefile`: It will allow us to automize compilation and processes execution

## Makefile

In order to compile the whole project we will hace to use:

> make

Every source file will be compiled after said sentence is executed, and every object and binary files will be put in the folders they belong to


If we just want to compile one specific file we will use:

> make *name_of_the_process*

It will be compiled and its object and binary files will be put in the folders they belong to.
The diferent *name_of_the_process* we can use are:

- **manager**
- **pa**
- **pb**
- **pc**
- **daemonBackup**


In order to clean our project folder we can also make use of the Makefile file. We can do this with:

> make clean

`exec`, `obj`, `log.txt`, `estudiantes` will be completed deleted if they existed.


After every file compilation is done we can execute the project with:

> make run