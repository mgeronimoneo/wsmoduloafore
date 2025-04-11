PATH_INS	=	/sysx/progs/gsoap/wsmoduloafore
CC			=	g++ #-DWITH_OPENSSL
CFLAGS		=	-Wall -W -o4 -fpack-struct=1 -fPIC -g -w
INCLUDE		=	-I/usr/local/include -I/sysx/proyectos/include -I./ -I /clases -I /clasessql 
LIBSC++ 	=   -lodbc -lssl -lcrypto /sysx/progs/afore/librerias/cutileriasafo.so
OBJ_X64		=	/sysx/proyectos/include/x86_64
LIBSC   =	-lodbc -lssl -lcrypto 

all:
	@rm -f ./bin/*
	#=============  Clases =======================================
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CConsultaInt.o ../clasessql/CConsultaInt.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CCampoTexto.o ../clasessql/CCampoTexto.cpp			
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CBaseDato.o ../clases/CBaseDato.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CConsultadeSaldoVivienda.o ../clasessql/CConsultadeSaldoVivienda.cpp	
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CConsultaSaldoVivienda.o CConsultaSaldoVivienda.cpp
	#=============================================================
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBSC++) -shared -o ./bin/cconsultasaldovivienda.so ./bin/CConsultaSaldoVivienda.o \
	$(OBJ_X64)/SqlTime.o $(OBJ_X64)/RecordSet.o $(OBJ_X64)/C_ODBC.o $(OBJ_X64)/md5.o \
	./bin/CConsultaInt.o ./bin/CCampoTexto.o ./bin/CConsultadeSaldoVivienda.o ./bin/CBaseDato.o
	
			
	@rm -f ./bin/*.o	
	@cp -f ./bin/cconsultasaldovivienda.so $(PATH_INS)
	@echo "======================================================="
	@echo "=            ! ! !  ProCompilationeR   ! ! !          ="
	@echo "=======================================================" 
	
	