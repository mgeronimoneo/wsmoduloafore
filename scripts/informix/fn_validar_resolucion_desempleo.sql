DROP FUNCTION fn_validar_resolucion_desempleo(char(11));
CREATE FUNCTION fn_validar_resolucion_desempleo(pcNss char(11))
RETURNING char(10) as Fecha_Vigencia, char(10) as Fecha_Vig_Menos_3, char(10) as Fecha_Vig_Mas_1, INTEGER as shRet, char(50) as shRetMensaje, INTEGER as DiagnosticoProcesar

   	-- SERVIDOR    : 10.26.54.136
	-- B.D.        : safre_af
	-- MODULO      :
	-- PROGRAMADOR : Manuel De Jesus Camez Vazquez
	-- FECHA       : 22/05/2015
	-- DESCRIPCION :

	--DEFINIR VARIABLES
	DEFINE tFechaActual		DATE;
	DEFINE tFechaFinVigencia DATE;
	DEFINE tFechaFinVigencia_Menos3d DATE;
	DEFINE tFechaProxSol 	DATE;
	DEFINE shRet			INTEGER;
	DEFINE cShRetMensaje	CHAR(50);
	DEFINE iDiagProcesar	INTEGER;
	DEFINE iRespuesta5Anos	INTEGER;

	--INICIALIZAR VARIABLES
	LET tFechaActual = today;
	LET tFechaFinVigencia = '';
	LET tFechaFinVigencia_Menos3d = '';
	LET tFechaProxSol = '';
	LET shRet = 0;
	LET cShRetMensaje = '';
	LET iDiagProcesar = 0;
	LET iRespuesta5Anos = 0;

	SELECT LIMIT 1 max(fecha_fin_vigencia) INTO tFechaFinVigencia
	FROM ret_parcial_resol where nss = pcNss and tipo_prestacion = 6;

	SELECT LIMIT 1 NVL(diag_procesar,'0')::INTEGER INTO iDiagProcesar
	FROM ret_parcial_resol WHERE fecha_fin_vigencia = tFechaFinVigencia AND nss = pcNss AND diag_procesar IN (400, 106) AND tipo_prestacion = 6;

	IF(iDiagProcesar IS NULL) THEN
		LET iDiagProcesar = 0;
	END IF;

    LET tFechaFinVigencia_Menos3d = fn_habil_anterior(tFechaFinVigencia, 5);

	IF(tFechaActual <= tFechaFinVigencia_Menos3d AND (iDiagProcesar IN (400, 106))) THEN
		
		LET cShRetMensaje = 'SE ENCONTRO RESOLUCION VIGENTE ACEPTADA';

			LET iRespuesta5Anos = fn_ExisteRetiro(1,pcNss);
			
			IF(iRespuesta5Anos != -1) THEN
				IF(iRespuesta5Anos != 0) THEN
					LET shRet = -1; --ENTRAR AL PROCESO EN LINEA EL CUAL VALIDA LOS 5 AÃ?OS
				ELSE
					LET shRet = 1;  --ENTRAR DIRECTO SIN CONSUMIR EL SERVICIO, NO CUENTA CON RETIRO EN LOS 5 AÃ?OS ANTERIORES
					--NO SE INVOCA EL SERVICIO
				END IF;
			ELSE 
				LET shRet = -1;
			END IF;
		
		--MUESTRA LA PANTALLA
	ELIF(tFechaActual <= tFechaFinVigencia_Menos3d AND (iDiagProcesar NOT IN (400, 106))) THEN
		LET shRet = 2;
		LET cShRetMensaje = 'SE ENCONTRO RESOLUCION VIGENTE RECHAZADA';
    ELIF(tFechaActual > tFechaFinVigencia_Menos3d AND tFechaActual <= tFechaFinVigencia ) THEN
		LET shRet = 3;
		LET cShRetMensaje = 'SE ENCONTRO RESOLUCION PERO NO CUENTA CON MAS DE 3 DIAS HABILES VIGENTES';
		--MANDAR MENSAJE DE DIA DESPUES DE LA FECHA FIN VIGENCIA
		LET tFechaProxSol = fn_habil_siguiente(tFechaFinVigencia,1);
	ELIF(tFechaFinVigencia IS NULL OR tFechaFinVigencia = '') THEN
		LET shRet = -1;
		LET cShRetMensaje = 'NO SE ENCONTRO RESOLUCIÃ?N';
	ELSE
		LET shRet = -2;
		LET cShRetMensaje = 'NO SE ENCONTRO RESOLUCION VIGENTE';
	END IF;
	
	RETURN TO_CHAR(tFechaFinVigencia,'%d/%m/%Y')::CHAR(10),TO_CHAR(tFechaFinVigencia_Menos3d,'%d/%m/%Y')::CHAR(10),TO_CHAR(tFechaProxSol,'%d/%m/%Y')::CHAR(10),shRet,cShRetMensaje,iDiagProcesar;

END FUNCTION;