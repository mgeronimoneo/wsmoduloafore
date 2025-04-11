--DROP FUNCTION fn_consulta_resolpension_retaportaciones(CHAR(18), CHAR(11));
CREATE FUNCTION fn_consulta_resolpension_retaportaciones(pcCurp CHAR(18), pcNss CHAR(11))

RETURNING 
	integer AS Opcion, 
	char(4) AS Tipo_Pension;

/*********************************************************************
	-- B.D.        : safre_af
	-- FECHA       : 29/Diciembre/2017
	-- DESCRIPCION :
*********************************************************************/
	--DEFINIR VARIABLES
	DEFINE tTipo_Pension 	CHAR(2);
	DEFINE iOpcion 		INTEGER;
	DEFINE iCount			INTEGER;

	--INICIALIZAR VARIABLES
	LET tTipo_Pension 	= '00';
	LET iOpcion 		= 0;
	LET iCount 		= 0;

	SELECT COUNT(*) INTO iCount FROM ret_det_datamart WHERE diag_datamart = 101 AND curp = pcCurp AND nss = pcNss;

	IF (iCount > 0) THEN
		FOREACH
			SELECT LIMIT 1 tipo_pension INTO tTipo_Pension FROM ret_det_datamart WHERE diag_datamart = 101 AND curp = pcCurp AND nss = pcNss ORDER BY sec_pension DESC			
		END FOREACH;
		LET iOpcion = 1;--IMSS
	ELSE
		SELECT COUNT(*) INTO iCount FROM ret_datamart_issste WHERE diag_datamart = 101 AND curp = pcCurp;

		IF (iCount > 0) THEN
			FOREACH
				SELECT LIMIT 1 tipo_pension INTO tTipo_Pension FROM ret_datamart_issste WHERE diag_datamart = 101 AND curp = pcCurp
			END FOREACH;
			LET iOpcion = 2;--ISSSTE
		END IF;
	END IF;
	
	RETURN iOpcion,tTipo_Pension;
END FUNCTION;