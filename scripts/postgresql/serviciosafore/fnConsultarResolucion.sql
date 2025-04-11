CREATE OR REPLACE FUNCTION fnConsultarResolucion(cNss CHAR(11), iTipoRetiro INT)
RETURNS tpoDatosResolucion AS
$BODY$
	DECLARE
		iTipoPrestacion	int;
		regResolucion	tpoDatosResolucion;
	BEGIN
		iTipoPrestacion = 0;
		regResolucion.nss = '';
		regResolucion.FechaInicioVigencia = '19000101';
		regResolucion.FechaFinVigencia = '19000101';
		regResolucion.numeroResolucion = 0;
		
		IF iTipoRetiro = 1024 THEN --desempleo
			iTipoPrestacion = 6;
		ELSIF iTipoRetiro = 1025 THEN -- matrimonio
			iTipoPrestacion = 7;
		END IF;
		
		SELECT nss, numresolucion, fechainiciovigencia, fechafinvigencia
		INTO regResolucion
		FROM retresolucionimss
		WHERE nss = cNss AND tipoprestacion = iTipoPrestacion and CURRENT_DATE BETWEEN fechainiciovigencia AND fechafinvigencia AND diagnosticoprocesar IN ('400', '106')
		ORDER BY fechainicioVigencia DESC
		LIMIT 1;
		
		IF regResolucion.nss IS NULL THEN
			regResolucion.nss = '';
			regResolucion.FechaInicioVigencia = '19000101';
			regResolucion.FechaFinVigencia = '19000101';
			regResolucion.numeroResolucion = 0;
		END IF;
	
		RETURN regResolucion;
	END;	
$BODY$
LANGUAGE plpgsql VOLATILE SECURITY DEFINER
COST 100;
ALTER FUNCTION fnConsultarResolucion(CHAR(11), INT) OWNER TO postgres;
GRANT EXECUTE ON FUNCTION fnConsultarResolucion(CHAR(11), INT) TO sysserviciosafore;