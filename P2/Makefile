DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRTXT := textos/

CFLAGS := -I$(DIRHEA) -c
LDLIBS := -pthread -std=c++11
CC := g++

all : dirs ssooiigle 

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRHEA) $(DIRSRC)

threadData: $(DIROBJ)threadData.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) -Xlinker $(DIRHEA)	

ssooiigle: $(DIROBJ)ssooiigle.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) $(DIRSRC)threadData.cpp

test: 
	./$(DIREXE)ssooiigle $(DIRTXT)prueba.txt david 5

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~