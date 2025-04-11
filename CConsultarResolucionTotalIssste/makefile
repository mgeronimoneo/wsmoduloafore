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
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CConsultaDataMartIssste.o ../clasessql/CConsultaDataMartIssste.cpp	
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CConsultarResolucionTotalIssste.o CConsultarResolucionTotalIssste.cpp
	#=============================================================
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBSC++) -shared -o ./bin/CConsultarResolucionTotalIssste.so ./bin/CConsultarResolucionTotalIssste.o \
	$(OBJ_X64)/SqlTime.o $(OBJ_X64)/RecordSet.o $(OBJ_X64)/C_ODBC.o $(OBJ_X64)/md5.o \
	./bin/CConsultaInt.o ./bin/CCampoTexto.o ./bin/CConsultaDataMartIssste.o ./bin/CBaseDato.o
	
			
	@rm -f ./bin/*.o	
	@cp -f ./bin/CConsultarResolucionTotalIssste.so $(PATH_INS)
	@echo "======================================================="
	@echo "=            ! ! !  ProCompilationeR   ! ! !          ="
	@echo "=======================================================" 
	
	