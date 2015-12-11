CC=gcc
CFLAGS=-O2 -Wall -g

BIN_DIR=/usr/bin
CONFIG_DIR=/etc/myprobe

MYSQL_INCLUDE_PATH=-I/usr/local/mysql/include
MYSQL_LIB_PATH=-L/usr/local/mysql/lib/mysql
LIBS=$(MYSQL_LIB_PATH) -lmysqlclient
INCLUDES = -I.. $(MYSQL_INCLUDE_PATH) 

#MYSQL_INCLUDE_PATH=-I/usr/include/mysql
#MYSQL_LIB_PATH=
#LIBS=$(MYSQL_LIB_PATH) -lmysqlclient
#INCLUDES = -I.. $(MYSQL_INCLUDE_PATH) 

# DEFINES = -D_HPUX_ -D__MYSQL_DEBUG__ -D__PIPE_DEBUG__
DEFINES = -D_HPUX_

all:	hpmnt1 hpmnt2

hpmnt1:	hpprocess.o util.o hpsarmain.o hpsar.o hpother.o hpfilesystem.o hpnetwork.o hplogfile.o
	$(CC) -o hpmnt1 hpprocess.o util.o hpsarmain.o hpsar.o hpother.o hpfilesystem.o hpnetwork.o hplogfile.o $(LIBS)

hpmnt2:	hpiostatmain.o util.o
	$(CC) -o hpmnt2 hpiostatmain.o util.o $(LIBS)

hpsarmain.o:	hpsarmain.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hpsarmain.c
	
hpprocess.o:	hpprocess.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hpprocess.c

util.o:	util.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c util.c

hpsar.o: hpsar.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hpsar.c

hpother.o: hpother.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hpother.c

hpiostatmain.o: hpiostatmain.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hpiostatmain.c

hpfilesystem.o: hpfilesystem.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hpfilesystem.c

hpnetwork.o: hpnetwork.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hpnetwork.c

hplogfile.o: hplogfile.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c hplogfile.c
	
clean:
	rm -f hpmnt1 hpmnt2 *.o *~ core *.core

install:
	cp -f hpmnt1 $(BIN_DIR)
	cp -f hpmnt2 $(BIN_DIR)
	rm -rf $(CONFIG_DIR)
	mkdir $(CONFIG_DIR)
	cp -f *.txt $(CONFIG_DIR)

uninstall:
	rm -f $(BIN_DIR)/hpmnt1
	rm -f $(BIN_DIR)/hpmnt2
	rm -rf $(CONFIG_DIR)
