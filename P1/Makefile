DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIREST := estudiantes/
LOGFILE := log.txt

CFLAGS := -I$(DIRHEA) -c -Wall -ansi
LDLIBS := -lpthread -lrt
CC := gcc

all : dirs manager pa pb pc pd daemonBackup

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRHEA) $(DIRSRC)

manager: $(DIROBJ)manager.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pa: $(DIROBJ)pa.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pb: $(DIROBJ)pb.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pc: $(DIROBJ)pc.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pd: $(DIROBJ)pd.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

daemonBackup: $(DIROBJ)daemonBackup.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@

run:
	./$(DIREXE)daemonBackup &
	./$(DIREXE)manager

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIREST) $(LOGFILE) $(DIRHEA)*~ $(DIRSRC)*~
