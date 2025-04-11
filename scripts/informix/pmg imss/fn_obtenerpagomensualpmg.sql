-- DROP FUNCTION fn_obtenerpagomensualpmg();

CREATE FUNCTION fn_obtenerpagomensualpmg()
RETURNING CHARACTER(100) AS Mensaje,
		  SMALLINT		AS	Estatus,
		  DECIMAL(10,2) AS  ImpMensual;
		  

    DEFINE sEstatus        	SMALLINT;
    DEFINE sMensaje        	CHARACTER(100);
    DEFINE dImpMensualP		DECIMAL(10,2);
    
    LET sEstatus     = 0;
    LET sMensaje     = '';
    LET dImpMensualP = 0;
    
    SELECT importe_mensual_11p
    INTO   dImpMensualP
    FROM   tab_pmg_historica
    WHERE  fecha_hasta IS NULL;

    IF dImpMensualP > 0 THEN
		LET sMensaje = 'CONSULTA EXITOSA';
	ELSE
		LET sMensaje = 'NO SE ENCONTRO REGISTRO EN LA TABLA';
		LET sEstatus = 1;
	END IF;

   RETURN NVL( sMensaje , ''), NVL(sEstatus,0), NVL(dImpMensualP,0.0);

END FUNCTION;