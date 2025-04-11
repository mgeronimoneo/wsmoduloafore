CREATE OR REPLACE FUNCTION fnConsultarComplementoResolucion(cNss CHAR(11), iTipoRetiro INT)
RETURNS tpoComplementoResolucion AS
$BODY$
	DECLARE
		iTipoPrestacion 	INT;
		regComplResolucion 	tpoComplementoResolucion;
		regResol 			RECORD;
		regResolCompl 		RECORD;
	BEGIN
		iTipoPrestacion = 0;
		regComplResolucion.numeroResolucion = 0;
		regComplResolucion.idcomplemento = 0;
		regComplResolucion.montopagado = 0;
		regComplResolucion.montopagadocomplemento = 0;
		regComplResolucion.saldorcv = 0;
		regComplResolucion.saldorcvanterior = 0;
		regComplResolucion.respuestaotrodatamart = 0;

		IF iTipoRetiro = 1024 THEN --desempleo
			iTipoPrestacion = 6;
		ELSIF iTipoRetiro = 1025 THEN -- matrimonio
			iTipoPrestacion = 7;
		END IF;
		
		SELECT idcomplemento,montopagado,numresolucion,saldorcvant INTO regResol
		FROM retresolucionimss
		WHERE idcomplemento = 1 AND tipoprestacion = iTipoPrestacion AND diagnosticoprocesar IN ('400', '106')
		AND fechafinvigencia >= CURRENT_DATE AND nss = cNss AND numresolucion NOT IN
		(SELECT numeroresolucion FROM retirosparciales WHERE estadosolicitud = 8 AND diagnosticocuentaind IN ('400', '106') AND nss = cNss);

		IF regResol.idcomplemento = 0 THEN
			SELECT montopagado, saldorcv INTO regResolCompl FROM retResolucionComplemento WHERE nss = cNss AND montopagado > 0;

			regComplResolucion.idcomplemento = 1;
			regComplResolucion.montopagado = COALESCE(regResol.montopagado,0);
			regComplResolucion.saldorcvanterior = COALESCE(regResol.saldorcvant,0);
			
			IF regResolCompl.saldorcv > 0 AND regResolCompl.montopagado > 0 THEN
				regComplResolucion.saldorcv = regResolCompl.saldorcv * 0.115;
				regComplResolucion.montopagadocomplemento = regResolCompl.saldorcv-regResolCompl.montopagado;
				regComplResolucion.respuestaotrodatamart = 1;
			END IF;
		ELSE
			regComplResolucion.numeroResolucion = COALESCE(regResol.numresolucion,0);
			regComplResolucion.idcomplemento = 0;
			regComplResolucion.montopagado = COALESCE(regResol.montopagado, 0);
			regComplResolucion.montopagadocomplemento = 0;
			regComplResolucion.saldorcv = 0;
			regComplResolucion.saldorcvanterior = COALESCE(regResol.saldorcvant,0);
			regComplResolucion.respuestaotrodatamart = 0;
		END IF;
	
		RETURN regComplResolucion;
	END;	
$BODY$
LANGUAGE plpgsql VOLATILE SECURITY DEFINER
COST 100;
ALTER FUNCTION fnConsultarComplementoResolucion(CHAR(11), INT) OWNER TO postgres;
GRANT EXECUTE ON FUNCTION fnConsultarComplementoResolucion(CHAR(11), INT) TO sysserviciosafore;