DROP FUNCTION fnConsultarComplResolucion(CHAR(11), SMALLINT);
CREATE FUNCTION fnConsultarComplResolucion( cNss CHAR(11), iTipoRetiro SMALLINT)
RETURNING INTEGER, INTEGER, DECIMAL(16,2), DECIMAL(16,2), DECIMAL(16,2), DECIMAL(16,2), SMALLINT
	
	DEFINE iNumResolucion				INTEGER;
	DEFINE idComplemento				INTEGER;
	DEFINE  dMontoPagado  				DECIMAL(16,2);
	DEFINE  dMontoPagadoComplemento  	DECIMAL(16,2);
	DEFINE  dSaldoRcv  					DECIMAL(16,2);
	DEFINE  dSaldoRcvAnterior  			DECIMAL(16,2);
	DEFINE  iRespuestaOtroDatamart		SMALLINT;
	DEFINE  iTipoPrestacion				SMALLINT;

	LET iNumResolucion = 0;
	LET idComplemento = 0;
	LET dMontoPagado = 0;
	LET dMontoPagadoComplemento = 0;
	LET dSaldoRcv = 0;
	LET dSaldoRcvAnterior = 0;
	LET iRespuestaOtroDatamart = 0;
	
	IF iTipoRetiro = 1024 THEN --desempleo
		LET iTipoPrestacion = 6;
	ELIF iTipoRetiro = 1025 THEN -- matrimonio
		LET iTipoPrestacion = 7;
	END IF;

	SELECT id_complemento, monto_pagado, num_resolucion, saldo_rcv_ant
	INTO idComplemento, dMontoPagado, iNumResolucion, dSaldoRcvAnterior
	FROM ret_parcial_resol
	WHERE id_complemento = 1 AND tipo_prestacion = iTipoPrestacion
	AND diag_procesar IN ('400', '106') AND fecha_fin_vigencia >= today AND nss = cNss AND num_resolucion NOT IN
	(SELECT num_resolucion FROM ret_parcial where estado_solicitud = 8 AND diag_cuenta_ind IN ('400', '106') AND nss = cNss);

	IF idComplemento = 0 THEN
		SELECT monto_pagado, saldo_rcv
		INTO dMontoPagadoComplemento, dSaldoRcv
		FROM ret_resol_comp
		WHERE monto_pagado > 0 AND nss = cNss;
		
		LET idComplemento = 1;
		IF dSaldoRcv > 0 and dMontoPagadoComplemento > 0 THEN
			LET dSaldoRcv = dSaldoRcv * 0.115;
			LET dMontoPagadoComplemento = dSaldoRcv - dMontoPagadoComplemento;
			LET iRespuestaOtroDatamart = 1;
		END IF;
	END IF;
	RETURN NVL(iNumResolucion, 0), NVL(idComplemento, 0), NVL(dMontoPagado, 0), NVL(dMontoPagadoComplemento, 0), NVL(dSaldoRcv, 0), NVL(dSaldoRcvAnterior, 0), NVL(iRespuestaOtroDatamart, 0);
END FUNCTION;