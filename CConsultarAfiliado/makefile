PATH_INS	=	/sysx/progs/gsoap/wsmoduloafore
CC      	=   g++
CFLAGS  	=   -Wall -W -o4 -fpack-struct=1 -fPIC -g -w
INCLUDE		=   -I/usr/local/lib64 -I/usr/include -I/usr/local/include -I/sysx/proyectos/include -I./ -I../ -I../clases -I../clasessql
LIBSC++ 	=   -lodbc /sysx/progs/afore/librerias/cutileriasafo.so
OBJ_X64		=	 /sysx/proyectos/include/x86_64
				
all:
	@rm -f ./bin/*	
	
	#=============  ClasesSql ====================================
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CConsultaInt.o ../clasessql/CConsultaInt.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CCampoTexto.o ../clasessql/CCampoTexto.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CTbMemoriaCompartida.o ../clasessql/CTbMemoriaCompartida.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CInfoTrabajadorIni.o clases/CInfoTrabajadorIni.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CInfoTrabajadorCompleta.o clases/CInfoTrabajadorCompleta.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/funcionescomparar.o ../clases/funcionescomparar.cpp
	#=============  Clases =======================================
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CBaseDato.o ../clases/CBaseDato.cpp
	#=============================================================
	
	$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/CConsultarAfiliado.o CConsultarAfiliado.cpp
	
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBSC++) -shared -o ./bin/cconsultarafiliado.so ./bin/CConsultarAfiliado.o \
	$(OBJ_X64)/SqlTime.o $(OBJ_X64)/RecordSet.o $(OBJ_X64)/C_ODBC.o $(OBJ_X64)/md5.o \
	./bin/CBaseDato.o ./bin/CConsultaInt.o ./bin/CCampoTexto.o ./bin/CTbMemoriaCompartida.o ./bin/CInfoTrabajadorIni.o \
	./bin/funcionescomparar.o ./bin/CInfoTrabajadorCompleta.o
	
	@cp ./bin/cconsultarafiliado.so $(PATH_INS)
	@rm -f ./bin/*.o
	@echo "======================================================="
	@echo "=            ! ! !    E X I T O   ! ! !               ="
	@echo "=======================================================" 