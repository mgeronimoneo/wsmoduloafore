-- DROP FUNCTION fnobtenerpagomensualpmg();

CREATE OR REPLACE FUNCTION fnobtenerpagomensualpmg()
RETURNS tpoperacionespmgimss AS 
$BODY$
	DECLARE
	
		tpDatos			tpoperacionespmgimss;
		dpImpMensualP	DOUBLE PRECISION;
		
	BEGIN
		tpDatos.Mensaje = '';
		tpDatos.Codigo	= 0;
		tpDatos.MontoPmg = 0.0;
		dpImpMensualP = 0.0;
		
		SELECT importemensual11p
		INTO   dpImpMensualP
		FROM   catpmghistoricaimss
		WHERE  fechahasta = '1900-01-01'::DATE;

		IF dpImpMensualP > 0 THEN
			tpDatos.MontoPmg = dpImpMensualP;
			tpDatos.Mensaje = 'CONSULTA EXITOSA';
		ELSE
			tpDatos.Mensaje = 'NO SE ENCONTRO REGISTRO EN LA TABLA';
			tpDatos.Codigo 	= 1;
		END IF;
		
		RETURN tpDatos;
	END;
$BODY$
LANGUAGE plpgsql VOLATILE SECURITY DEFINER
COST 100;
ALTER FUNCTION fnobtenerpagomensualpmg() OWNER TO postgres;
ALTER FUNCTION fnobtenerpagomensualpmg() OWNER TO sysserviciosafore;