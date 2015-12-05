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

DEFINES = -D _HPUX_

all:	hpsar hpiostat

hpsar:	hpprocess.o util.o hpsarmain.o hpsar.o hpother.o hpfilesystem.o hpnetwork.o
	$(CC) -o hpsar hpprocess.o util.o hpsarmain.o hpsar.o hpother.o hpfilesystem.o hpnetwork.o $(LIBS)

hpiostat:	hpiostatmain.o util.o
	$(CC) -o hpiostat hpiostatmain.o util.o $(LIBS)

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
	
clean:
	rm -f hpsar hpiostat *.o *~ core *.core

install:
	cp -f hpsar $(BIN_DIR)
	cp -f hpiostat $(BIN_DIR)
	rm -rf $(CONFIG_DIR)
	mkdir $(CONFIG_DIR)
	cp -f *.txt $(CONFIG_DIR)

uninstall:
	rm -f $(BIN_DIR)/hpsar
	rm -f $(BIN_DIR)/hpiostat
	rm -rf $(CONFIG_DIR)
