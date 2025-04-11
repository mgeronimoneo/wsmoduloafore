CREATE OR REPLACE FUNCTION fnValidarResolucionDesempleo(cNss CHAR(11))
RETURNS SETOF tpoValidareResolucionDesempleo AS
$BODY$
	DECLARE
		regResp					tpoValidareResolucionDesempleo;
		iDiagnosticoProcesar	INT;
		dFechaFinVigencia		DATE;
		dFechaVigenciaAnt		DATE;
		iRetorno				INT;
	BEGIN
		regResp.FechaVigencia = '19000101';
		regResp.FechaVigenciaAnt = '19000101';
		regResp.FechaProximaSolicitud = '19000101';
		regResp.Retorno = 0;
		regResp.Mensaje = '';
		regResp.DiagnosticoProcesar = 0;
		iRetorno = 0;
		
		--Se obtiene la fecha fin de vigencia maxima que pueda tener el trabajador
		SELECT MAX(fechaFinVigencia)
		INTO dFechaFinVigencia
		FROM retResolucionImss
		WHERE nss = cNss AND tipoPrestacion = 6;
		
		--Se obtiene el diagnostico de procesar del datamart
		SELECT CASE WHEN diagnosticoProcesar = '' THEN 0 ELSE diagnosticoProcesar::INT END
		INTO iDiagnosticoProcesar
		FROM retResolucionImss 
		WHERE fechaFinVigencia = dFechaFinVigencia AND nss = cNss AND diagnosticoProcesar::INT IN (400, 106) AND tipoPrestacion = 6;
		
		RAISE NOTICE '%', iDiagnosticoProcesar;
		
		IF(iDiagnosticoProcesar IS NULL) THEN
			iDiagnosticoProcesar = 0;
		END IF;
		
		--Se calcula la fecha a 5 dias habiles posteriores a la fecha fin de vigencia
		SELECT fncalculardiahabil INTO dFechaVigenciaAnt FROM fncalculardiahabil(dFechaFinVigencia, -5);
		
		IF(CURRENT_DATE <= dFechaVigenciaAnt AND (iDiagnosticoProcesar IN (400, 106))) THEN
			regResp.Mensaje='SE ENCONTRO RESOLUCION VIGENTE ACEPTADA';
			
			--Validar si tiene un retiro en los ultimos 5 años
			SELECT fnValidarUltimoRetParcial INTO iRetorno FROM fnValidarUltimoRetParcial(1, cNss);
			
			IF(iRetorno != -1) THEN
				IF(iRetorno != 0) THEN
					regResp.Retorno = -1; --ENTRAR AL PROCESO EN LINEA EL CUAL VALIDA LOS 5 AÑOS
				ELSE
					regResp.Retorno = 1;  --ENTRAR DIRECTO SIN CONSUMIR EL SERVICIO, NO CUENTA CON RETIRO EN LOS 5 AÑOS ANTERIORES
					--NO SE INVOCA EL SERVICIO
				END IF;
			ELSE 
				regResp.Retorno = -1;
			END IF;
			
			--MUESTRA LA PANTALLA
		ELSIF(CURRENT_DATE <= dFechaVigenciaAnt AND (iDiagnosticoProcesar NOT IN (400, 106))) THEN
			regResp.Retorno= 2;
			regResp.Mensaje= 'SE ENCONTRO RESOLUCION VIGENTE RECHAZADA';
		ELSIF(CURRENT_DATE > dFechaVigenciaAnt AND CURRENT_DATE <= dFechaFinVigencia) THEN
			regResp.Retorno= 3;
			regResp.Mensaje= 'SE ENCONTRO RESOLUCION PERO NO CUENTA CON MAS DE 5 DIAS HABILES VIGENTES';
			--MANDAR MENSAJE DE DIA DESPUES DE LA FECHA FIN VIGENCIA
			SELECT fncalculardiahabil INTO regResp.FechaProximaSolicitud FROM fncalculardiahabil(dFechaFinVigencia, 1);
		ELSIF(dFechaFinVigencia IS NULL OR dFechaFinVigencia = '19000101') THEN
			regResp.Retorno= -1;
			regResp.Mensaje= 'NO SE ENCONTRO RESOLUCION';
		ELSE
			regResp.Retorno= -2;
			regResp.Mensaje= 'NO SE ENCONTRO RESOLUCION VIGENTE';
		END IF;
		regResp.FechaVigencia = dFechaFinVigencia;
		regResp.FechaVigenciaAnt = dFechaVigenciaAnt;
		regResp.DiagnosticoProcesar = iDiagnosticoProcesar;
		RETURN NEXT regResp;
	END;
$BODY$
LANGUAGE plpgsql VOLATILE SECURITY DEFINER
COST 100;
ALTER FUNCTION fnValidarResolucionDesempleo(CHAR(11)) OWNER TO postgres;
GRANT EXECUTE ON FUNCTION fnValidarResolucionDesempleo(CHAR(11)) TO sysserviciosafore;