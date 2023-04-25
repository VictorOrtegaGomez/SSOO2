DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRTXT := textos/

CFLAGS := -I$(DIRHEA) -c
LDLIBS := -pthread -std=c++11
CC := g++

all : dirs requests client main 

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRHEA) $(DIRSRC)

requests: $(DIROBJ)requests.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) -Xlinker $(DIRHEA)

client: $(DIROBJ)client.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) -Xlinker $(DIRHEA)	

main: $(DIROBJ)main.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) $(DIRSRC)client.cpp $(DIRSRC)requests.cpp 

test: 
	./$(DIREXE)main

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~