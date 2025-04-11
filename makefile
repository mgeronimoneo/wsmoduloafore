PATH_LIB=	/sysx/progs/afore/librerias
OBJ_X64	=	 /sysx/proyectos/include/x86_64
PATH_INS=	/sysx/progs/gsoap/wsmoduloafore

CC		=	g++
CFLAGS	=	-Wall -W -o4 -fpack-struct=1 -fPIC -g -Wno-unused-parameter
INCLUDE	=	-I/usr/lib64/include -I/sysx/proyectos/include -ICConsultarAfiliado -ICSaldosPorSubCuenta -ICFoliadorServicios -ICValidarConvivenciaMarca \
			-ICSaldoDiarioRetParcial -ICValidarResolusionImss -ICMontosFechasDatamart -ICConsultarResolusion -ICComplementoResolucion -ICConsultarResolucionTotalImss -ICOperacionesPmg -ICMovimientosCuentas\
			-ICSaldosPorSubCuentaPmg -ICAportaciones -ICConsultarCtaRegimen -ICConsultarCtaSaldoVol -ICConsultaSaldoVivienda
LIBSC   =	-lodbc $(PATH_LIB)/cutileriasafo.so $(PATH_INS)/cconsultarafiliado.so $(PATH_INS)/csaldosporsubcuenta.so $(PATH_INS)/cfoliadorservicios.so \
			$(PATH_INS)/cvalidarconvivenciamarca.so $(PATH_INS)/csaldodiarioretparcial.so $(PATH_INS)/cvalidarresolusionimss.so \
			$(PATH_INS)/cmontosfechasdatamart.so $(PATH_INS)/cconsultarresolusion.so $(PATH_INS)/ccomplementoresolucion.so \
			$(PATH_INS)/cconsultarresoluciontotalimss.so $(PATH_INS)/CConsultarResolucionTotalIssste.so $(PATH_INS)/coperacionespmg.so \
			$(PATH_INS)/CMovimientosCuentas.so $(PATH_INS)/csaldosporsubcuentapmg.so $(PATH_INS)/CAportaciones.so \
			$(PATH_INS)/CConsultarCtaRegimen.so $(PATH_INS)/cconsultarctasaldovol.so $(PATH_INS)/cconsultasaldovivienda.so \
			$(PATH_INS)/cconsultaaportacionesposteriores.so $(PATH_INS)/cconsultarresolusionretaportaciones.so
all:
		#============== gSoap ========================================
		$(CC) $(CFLAGS) $(INCLUDES) -c -o ./bin/soapC.o ./gSoapServer/soapC.cpp
		$(CC) $(CFLAGS) $(INCLUDES) -c -o ./bin/soapServer.o ./gSoapServer/soapServer.cpp
		$(CC) $(CFLAGS) $(INCLUDES) -c -o ./bin/stdsoap2.o ./gSoapServer/stdsoap2.cpp
		#=============================================================
										
		$(CC) $(CFLAGS) $(INCLUDE) -c -o ./bin/wsModuloAfore.o wsModuloAfore.cpp
				
		$(CC) $(CFLAGS) $(INCLUDE) $(LIBSC) -o ./bin/wsmoduloafore ./bin/wsModuloAfore.o \
		./bin/soapC.o ./bin/soapServer.o ./bin/stdsoap2.o
				
		@rm -f ./bin/*.o
		@cp  ./bin/wsmoduloafore $(PATH_INS)
		@chmod 777 ./bin/wsmoduloafore
		@echo "======================================================="
		@echo "=            ¡ ¡ ¡   E X I T O   ! ! !                ="
		@echo "======================================================="
