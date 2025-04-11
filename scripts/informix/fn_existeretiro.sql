DROP FUNCTION fn_existeretiro(SMALLINT, CHAR(11));
CREATE FUNCTION fn_existeretiro(p_tipo SMALLINT, p_nss CHAR(11))
RETURNING SMALLINT AS clave;

    DEFINE r_clave      SMALLINT;
    DEFINE v_fecha_Sol  DATE;
    DEFINE v_fecha		DATE;

    LET r_clave = 0;
    IF p_tipo = 1 THEN
        --Verificar Si Existe El Retiro Por Desempleo
        SELECT MAX(fecha_solicitud) INTO v_fecha_Sol
		FROM ret_Parcial
		WHERE nss = p_nss
		AND tipo_Prestacion = 6
		AND diag_Cuenta_Ind IN (400 , 106)
		AND num_resolucion IN (SELECT num_resolucion FROM ret_parcial_resol
		WHERE  NVL(id_Complemento, 0) = 0 AND tipo_Prestacion = 6 AND nss = p_nss);

        IF NOT v_fecha_Sol IS NULL THEN
            LET v_fecha = MDY( MONTH(TODAY), DAY(TODAY), YEAR(TODAY) - 5);
            IF v_fecha_Sol > v_fecha THEN
                LET r_clave = 1;
            END IF;
        ELSE
            LET r_clave = 0;
        END IF;
    ELSE
        --Verificar Si Existe El Retiro Por Matrimonio
        IF EXISTS (SELECT folio_solicitud FROM ret_Parcial WHERE (nss = p_nss AND tipo_Prestacion = 7 AND diag_Cuenta_Ind IN (400,106))
						OR (nss = p_nss AND tipo_Prestacion = 7 AND estado_solicitud = 2)) THEN
            LET r_clave = 1;
        ELSE
            LET r_clave = 0;
        END IF;
    END IF;

    RETURN r_clave;

END FUNCTION;