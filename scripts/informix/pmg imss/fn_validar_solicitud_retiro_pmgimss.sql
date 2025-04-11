-- DROP FUNCTION fn_validar_solicitud_retiro_pmgimss(CHARACTER(13))

CREATE FUNCTION fn_validar_solicitud_retiro_pmgimss(cNss CHARACTER(13))
RETURNING CHARACTER(100) AS Mensaje,
		  SMALLINT AS EstadoSol,
		  DECIMAL(10,2)	 AS MontoPmg;

	DEFINE sEstadoSol	SMALLINT;
	DEFINE cEstatusSol	CHARACTER(2);
	DEFINE cMensaje		CHARACTER(100);
	
	DEFINE sRechazado	SMALLINT;
	DEFINE sLiquidado	SMALLINT;
	
	DEFINE sConsecutivo	INTEGER;
	DEFINE sMensualidad	SMALLINT;
	DEFINE sEstado		SMALLINT;
	DEFINE dMonto       DECIMAL(10,2);
	
	LET sEstadoSol = 0;
	LET cEstatusSol = '';
	LET cMensaje = '';
	
	LET sRechazado = 80;
	LET sLiquidado = 70;
	
	LET sConsecutivo = 0;
	LET sMensualidad = 0;
	LET sEstado = 0;
	LET dMonto = 0;
	
	
	SELECT "OK" INTO cEstatusSol FROM pen_solicitud_pmg
    WHERE nss = cNss AND estado_solicitud NOT IN (sRechazado, sLiquidado)
    GROUP BY 1;
	
	SELECT consecutivo INTO sConsecutivo
    FROM pen_solicitud_pmg WHERE nss = cNss
    AND estado_solicitud NOT IN (sRechazado, sLiquidado);
	
	IF cEstatusSol =  "OK" THEN
		
       SELECT MAX(num_mensualidad) INTO sMensualidad FROM pen_ctr_pago_det
       WHERE nss = cNss AND consecutivo = sConsecutivo;
      
       SELECT estado INTO sEstado FROM pen_ctr_pago_det
       WHERE  nss = cNss AND consecutivo = sConsecutivo
       AND num_mensualidad = sMensualidad;
	   
	   --Condicion con la cual sabemos si se encuentra una solicitud previa
	   IF sEstado != sRechazado THEN 
			LET cMensaje = 'EXISTE UNA SOLICITUD PREVIA PARA ESTE NSS';
			LET sEstadoSol = 2;
		ELSE
			LET cMensaje = 'NO EXISTE UNA SOLICITUD PREVIA PARA ESTE NSS';
			LET sEstadoSol = 3;
       END IF;
	   
	ELSE
		--Retornamos 1, si el no se encuntra registro
		LET cMensaje = 'NO SE ENCONTRO REGISTRO';
		LET sEstadoSol = 1;
	END IF;

	RETURN 	NVL(cMensaje, ''),NVL(sEstadoSol, 0), NVL(dMonto,0.0);
	
END FUNCTION;