--DROP FUNCTION fn_Consultar_Saldo_Pmg (CHARACTER(13), CHARACTER(5), INTEGER);
CREATE FUNCTION fn_Consultar_Saldo_Pmg(cNss CHARACTER(13), cSecuencia CHARACTER(5), sFolio INTEGER)
RETURNING CHARACTER(100) AS Mensaje,
		  SMALLINT		 AS Codigo,
		  DECIMAL(10,2)	 AS MontoPmg;		 

	DEFINE cEstatusDat	CHARACTER(2); --Insuficiencia por datamart
	DEFINE cEstatusCap	CHARACTER(2); --Insuficiencia por captura
	DEFINE cMensaje		CHARACTER(100);
	DEFINE sCodigo		SMALLINT;
	DEFINE dMonto       DECIMAL(10,2);
	
	
	LET cEstatusDat = '';
	LET cEstatusCap = '';
	LET cMensaje = '';
	LET sCodigo = 0;
	LET dMonto = 0;
	
	SELECT "OK" INTO cEstatusDat FROM pen_detalle_op70 
	WHERE nss = cNss AND sec_pension = cSecuencia AND folio_datamart = sFolio 
	AND origen_informacion = 1 AND id_tipo_notifica = 600 GROUP BY 1;
	
	IF cEstatusDat = "OK" THEN
		LET sCodigo = 1;
		LET cEstatusDat = '';
		LET cMensaje = 'SALDO INSUFICIENTE POR PARTE DE DATAMART';
	ELSE
		SELECT "OK" INTO cEstatusCap FROM pen_detalle_op70 
		WHERE nss = cNss AND sec_pension = cSecuencia 
		AND folio_datamart = 0 AND origen_informacion = 2 AND id_tipo_notifica = 600 GROUP BY 1;
		
		IF cEstatusCap = "OK" THEN
			LET sCodigo = 2;
			LET cMensaje = 'SALDO INSUFICIENTE POR CAPTURA';
		ELSE
			LET sCodigo = 0;
			LET cEstatusCap = '';
			LET cMensaje = 'SALDO EN CUENTA';
		END IF;
	END IF;
	
	RETURN 	NVL(cMensaje,''), NVL(sCodigo,0), NVL(dMonto,0.0);
	
END FUNCTION;