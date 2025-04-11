-- DROP FUNCTION fnconsultarsaldopmg (CHARACTER(13), CHARACTER(2), INTEGER);

CREATE OR REPLACE FUNCTION fnconsultarsaldopmg (CHARACTER(13), CHARACTER(2), INTEGER) 
RETURNS tpoperacionespmgimss AS 

$BODY$
	DECLARE
		cNss		ALIAS FOR $1;
		cSecuencia	ALIAS FOR $2;
		iFolio		ALIAS FOR $3;
		tpDatos		tpoperacionespmgimss;
		
		sCodigo 	SMALLINT;
		cEstatusDat	CHARACTER(2);
		cEstatusCap	CHARACTER(2);
		cMensaje 	CHARACTER(100);
		
	BEGIN
		
		tpDatos.Mensaje  = ''; 
		tpDatos.Codigo 	 = 0; 
		tpDatos.MontoPmg = 0.0;
		sCodigo 	= 0;
		cEstatusDat = '';
		cEstatusCap = '';
		cMensaje 	= '';
		
		SELECT 'OK' INTO cEstatusDat FROM pendetalleop70pmgimss 
		WHERE nss = cNss AND secpension = cSecuencia AND foliodatamart = iFolio 
		AND origeninformacion = 1 AND idtiponotifica = 600 GROUP BY 1;
		
		IF cEstatusDat = 'OK' THEN
			sCodigo = 1;
			cMensaje = 'SALDO INSUFICIENTE POR PARTE DE DATAMART';
		ELSE
			SELECT 'OK' INTO cEstatusCap FROM pendetalleop70pmgimss 
			WHERE nss = cNss AND secpension = cSecuencia 
			AND foliodatamart = 0 AND origeninformacion = 2 AND idtiponotifica = 600 GROUP BY 1;
			
			IF cEstatusCap = 'OK' THEN
				sCodigo = 2;
				cMensaje = 'SALDO INSUFICIENTE POR CAPTURA';
			ELSE
				cMensaje = 'SALDO EN CUENTA';
			END IF;
		END IF;
		
		tpDatos.Mensaje  = cMensaje; 
		tpDatos.Codigo 	 = sCodigo; 
		
		RETURN tpDatos;
	END;
$BODY$
LANGUAGE plpgsql VOLATILE SECURITY DEFINER
COST 100;
ALTER FUNCTION fnconsultarsaldopmg (CHARACTER(13), CHARACTER(2), INTEGER) OWNER TO postgres;
ALTER FUNCTION fnconsultarsaldopmg (CHARACTER(13), CHARACTER(2), INTEGER) OWNER TO sysserviciosafore;