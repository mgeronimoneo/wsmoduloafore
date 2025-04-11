DROP FUNCTION fn_obtenerdatosretdetdatamrtrt( SMALLINT, CHARACTER(11),  CHARACTER(18));
CREATE FUNCTION fn_obtenerdatosretdetdatamrtrt(iOpcion SMALLINT,cNss CHARACTER(11), cCurp CHARACTER(18))
RETURNING INTEGER 	AS Existeresolucion,
		  INTEGER 	AS Existeafiliado,
		  CHAR(2) 	AS Regimen,
		  CHAR(2) 	AS TipoSeguro,
		  CHAR(2) 	AS TipoPension,
		  SMALLINT 	AS TipoPrestacion,
		  CHAR(50)	AS NombreDatamart,		--Nuevo
		  INT 	  	AS Folio,
		  CHAR(10) 	AS SecPension,
		  DATE 		AS FechaIni,
		  DATE 		AS FechaPago,
		  SMALLINT  AS SemanasCotizadas,	--Nuevo
		  SMALLINT  AS DiagDatamart,		--Nuevo
		  SMALLINT  AS EstadoSubViv,
		  DATE		AS FechaResol,
		  CHAR(40)	AS cDesRegimen,			--Nuevo
		  CHAR(40)	AS DesTipoPension,
		  CHAR(40)	AS DesTipoPresta,
		  CHAR(40)	AS DesTipoRetiro,
		  CHAR(40)	AS DesTipoSeguro,
		  CHAR(40)	AS DesEdoSubViv,		--Nuevo
		  INT		AS IdMovimiento;

	DEFINE iAfiliado 			SMALLINT;
	DEFINE iExisteresolucion 	INTEGER;
	DEFINE iRowid 				INTEGER;
	DEFINE iRegafiliado 		INTEGER;
	DEFINE iNoCorrecto			INTEGER;

	--Datos nuevos de retorno
	DEFINE cRegimen				CHAR(2);
	DEFINE cTipoSeguro			CHAR(2);
	DEFINE cTipoPension			CHAR(2);
	DEFINE sTipoPrestacion		SMALLINT;
	DEFINE cNombreDatamart		CHAR(50);	--Nuevo
	DEFINE iFolio				INT;
	DEFINE cSecPension			CHAR(10);
	DEFINE dFechaIni			DATE;
	DEFINE dFechaPago			DATE;
	DEFINE sSemanasCotizadas	SMALLINT;	--Nuevo
	DEFINE sDiagDatamart		SMALLINT;	--Nuevo
	DEFINE sEstadoSubViv		SMALLINT;
	DEFINE dFechaResol			DATE;

	DEFINE cDesRegimen			CHAR(40);	--Nuevo
	DEFINE cDesTipoPension		CHAR(40);
	DEFINE cDesTipoPresta		CHAR(40);
	DEFINE cDesTipoRetiro		CHAR(40);
	DEFINE cDesTipoSeguro		CHAR(40);
	DEFINE cDescEdoSubviv		CHAR(40);	--Nuevo

	DEFINE iIdMovimiento	INT;

	LET iAfiliado = 0;
	LET iExisteresolucion =0;
	LET iRowid = 0;
	LET iRegafiliado=0;

	--Datos nuevos de retorno
	LET cRegimen = '';
	LET cTipoSeguro = '';
	LET cTipoPension = '';
	LET sTipoPrestacion = 0;
	LET cNombreDatamart = '';	--Nuevo
	LET iFolio = 0;
	LET cSecPension = '';
	LET dFechaIni = MDY(01,01,1900);
	LET dFechaPago = MDY(01,01,1900);
	LET sSemanasCotizadas = 0;	--Nuevo
	LET sDiagDatamart = 0;	--Nuevo
	LET sEstadoSubViv = 0;
	LET dFechaResol = MDY(01,01,1900);

	LET cDesRegimen = '';	--Nuevo
	LET cDesTipoPension = '';
	LET cDesTipoPresta  = '';
	LET cDesTipoRetiro  = '';
	LET cDesTipoSeguro  = '';
	LET cDescEdoSubviv = '';	--Nuevo
	LET iNoCorrecto = 0;
	LET iIdMovimiento = 0;

	IF iOpcion = 1 THEN

		SELECT COUNT(*) INTO iRegafiliado FROM cuota_diaria WHERE n_seguro = cNss;
		IF iRegafiliado > 0 THEN
			LET iAfiliado = 1;
		END IF;

		SELECT MAX(rowid) INTO iRowid FROM ret_det_datamart WHERE nss = cNss AND curp = cCurp AND diag_datamart IN (101,300,301,501);
		IF iRowid > 0 THEN
			LET iExisteresolucion = 1;

			IF EXISTS (SELECT regimen FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid) THEN

				SELECT regimen,tipo_seguro,tipo_pension,tipo_prestacion, nombre_datamart, folio,sec_pension,fecha_ini_pen,fec_ini_pago, semanas_cotizadas, diag_datamart, estado_sub_viv,fecha_resolucion
				INTO cRegimen,cTipoSeguro,cTipoPension,sTipoPrestacion, cNombreDatamart, iFolio,cSecPension,dFechaIni,dFechaPago, sSemanasCotizadas, sDiagDatamart, sEstadoSubViv,dFechaResol
				FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid;

				SELECT descripcion INTO cDesRegimen FROM tab_regimen WHERE regimen = cRegimen;
				LET cDesRegimen = cRegimen||'-'||cDesRegimen;

				SELECT descripcion INTO cDesTipoPension FROM tab_pension WHERE tipo_pension = cTipoPension;
				LET cDesTipoPension = cTipoPension||'-'||cDesTipoPension;

				SELECT descripcion, movimiento INTO cDesTipoRetiro, iIdMovimiento FROM tab_retiro WHERE tipo_retiro = 'S';
				LET cDesTipoRetiro = 'S-'||cDesTipoRetiro;

				SELECT descripcion INTO cDesTipoSeguro FROM tab_seguro WHERE clave = cTipoSeguro;
				LET cDesTipoSeguro = cTipoSeguro||'-'||cDesTipoSeguro;

				SELECT descripcion INTO cDesTipoPresta FROM tab_prestacion WHERE tipo_prestacion = sTipoPrestacion;
				LET cDesTipoPresta =  sTipoPrestacion||'-'||cDesTipoPresta;

				SELECT descripcion INTO cDescEdoSubviv FROM tab_estado_sub_viv WHERE estado_sub_viv = sEstadoSubViv;
				LET cDescEdoSubviv =  sEstadoSubViv||'-'||cDescEdoSubviv;

			END IF;

		END IF;
	END IF;

	IF iOpcion = 2 THEN

		SELECT COUNT(*) INTO iRegafiliado FROM cuota_diaria WHERE n_seguro = cNss;
		IF iRegafiliado > 0 THEN
			LET iAfiliado = 1;
		END IF;

		SELECT MAX(rowid) INTO iRowid FROM ret_det_datamart WHERE nss = cNss AND curp = cCurp AND diag_datamart IN (101,300,301,210,302,303,501);
		IF iRowid > 0 THEN
			LET iExisteresolucion = 1;

			IF EXISTS (SELECT regimen FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid) THEN

				SELECT regimen,tipo_seguro,tipo_pension,tipo_prestacion, nombre_datamart,folio,sec_pension,fecha_ini_pen,fec_ini_pago, semanas_cotizadas, diag_datamart, estado_sub_viv,fecha_resolucion
				INTO cRegimen,cTipoSeguro,cTipoPension,sTipoPrestacion, cNombreDatamart, iFolio,cSecPension,dFechaIni,dFechaPago, sSemanasCotizadas, sDiagDatamart, sEstadoSubViv,dFechaResol
				FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid;

				SELECT descripcion INTO cDesRegimen FROM tab_regimen WHERE regimen = cRegimen;
				LET cDesRegimen = cRegimen||'-'||cDesRegimen;

				SELECT descripcion INTO cDesTipoPension FROM tab_pension WHERE tipo_pension = cTipoPension;
				LET cDesTipoPension = cTipoPension||'-E'||cDesTipoPension;

				SELECT descripcion, movimiento INTO cDesTipoRetiro, iIdMovimiento FROM tab_retiro WHERE tipo_retiro = 'E';
				LET cDesTipoRetiro = 'E-'||cDesTipoRetiro;

				SELECT descripcion INTO cDesTipoSeguro FROM tab_seguro WHERE clave = cTipoSeguro;
				LET cDesTipoSeguro = cTipoSeguro||'-'||cDesTipoSeguro;

				SELECT descripcion INTO cDesTipoPresta FROM tab_prestacion WHERE tipo_prestacion = sTipoPrestacion;
				LET cDesTipoPresta =  sTipoPrestacion||'-'||cDesTipoPresta;

				SELECT descripcion INTO cDescEdoSubviv FROM tab_estado_sub_viv WHERE estado_sub_viv = sEstadoSubViv;
				LET cDescEdoSubviv =  sEstadoSubViv||'-'||cDescEdoSubviv;

			END IF;

		END IF;

	END IF;

	IF iOpcion = 3 THEN --RETIRO J IMSS

		SELECT COUNT(*) INTO iRegafiliado FROM cuota_diaria WHERE n_seguro = cNss;
		IF iRegafiliado > 0 THEN
			LET iAfiliado = 1;
		END IF;

		SELECT MAX(rowid) INTO iRowid FROM ret_det_datamart WHERE nss = cNss AND curp = cCurp AND tipo_seguro IN ("CV","IV") AND diag_datamart IN (101,300,301,210,302,303);
		IF iRowid > 0 THEN
			LET iExisteresolucion = 1;

			IF EXISTS (SELECT regimen FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid) THEN

				SELECT regimen,tipo_seguro,tipo_pension,tipo_prestacion, nombre_datamart,folio,sec_pension,fecha_ini_pen,fec_ini_pago, semanas_cotizadas, diag_datamart, estado_sub_viv,fecha_resolucion
				INTO cRegimen,cTipoSeguro,cTipoPension,sTipoPrestacion, cNombreDatamart, iFolio,cSecPension,dFechaIni,dFechaPago, sSemanasCotizadas, sDiagDatamart, sEstadoSubViv,dFechaResol
				FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid;

				SELECT descripcion INTO cDesRegimen FROM tab_regimen WHERE regimen = cRegimen;
				LET cDesRegimen = cRegimen||'-'||cDesRegimen;

				SELECT descripcion INTO cDesTipoPension FROM tab_pension WHERE tipo_pension = cTipoPension;
				LET cDesTipoPension = cTipoPension||'-J'||cDesTipoPension;

				SELECT descripcion, movimiento INTO cDesTipoRetiro, iIdMovimiento FROM tab_retiro WHERE tipo_retiro = 'J';
				LET cDesTipoRetiro = 'J-'||cDesTipoRetiro;

				SELECT descripcion INTO cDesTipoSeguro FROM tab_seguro WHERE clave = cTipoSeguro;
				LET cDesTipoSeguro = cTipoSeguro||'-'||cDesTipoSeguro;

				SELECT descripcion INTO cDesTipoPresta FROM tab_prestacion WHERE tipo_prestacion = sTipoPrestacion;
				LET cDesTipoPresta =  sTipoPrestacion||'-'||cDesTipoPresta;

				SELECT descripcion INTO cDescEdoSubviv FROM tab_estado_sub_viv WHERE estado_sub_viv = sEstadoSubViv;
				LET cDescEdoSubviv =  sEstadoSubViv||'-'||cDescEdoSubviv;

			END IF;
			
		END IF;
		
		IF cTipoPension IS NULL OR cTipoPension = '' THEN 
			LET iNoCorrecto = 1;
		END IF;
		
		IF dFechaIni IS NULL OR dFechaIni = '' THEN 
			LET iNoCorrecto = 1;
		END IF;
		
		IF iNoCorrecto > 0 THEN
			LET iExisteresolucion = 0;--HAY ALGO MAL EN DATAMART
		END IF;
	END IF;
	
	IF iOpcion = 4 THEN
		
		SELECT COUNT(*) INTO iRegafiliado FROM cuota_diaria WHERE n_seguro = cNss;
		IF iRegafiliado > 0 THEN
			LET iAfiliado = 1;
		END IF;	
		
		SELECT MAX(rowid) INTO iRowid FROM ret_det_datamart WHERE nss = cNss AND curp = cCurp;
		IF iRowid > 0 THEN
			LET iExisteresolucion = 1;
		
			IF EXISTS (SELECT regimen FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid) THEN

				SELECT regimen,tipo_seguro,tipo_pension,tipo_prestacion, nombre_datamart,folio,sec_pension,fecha_ini_pen,fec_ini_pago, semanas_cotizadas, diag_datamart, estado_sub_viv,fecha_resolucion
				INTO cRegimen,cTipoSeguro,cTipoPension,sTipoPrestacion, cNombreDatamart, iFolio,cSecPension,dFechaIni,dFechaPago, sSemanasCotizadas, sDiagDatamart, sEstadoSubViv,dFechaResol
				FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid;

				SELECT descripcion INTO cDesRegimen FROM tab_regimen WHERE regimen = cRegimen;
				LET cDesRegimen = cRegimen||'-'||cDesRegimen;
					
				SELECT descripcion INTO cDesTipoPension FROM tab_pension WHERE tipo_pension = cTipoPension;
				LET cDesTipoPension = cTipoPension||'-F'||cDesTipoPension;
				
				SELECT descripcion, movimiento INTO cDesTipoRetiro, iIdMovimiento FROM tab_retiro WHERE tipo_retiro = 'F';
				LET cDesTipoRetiro = 'F-'||cDesTipoRetiro;
				
				SELECT descripcion INTO cDesTipoSeguro FROM tab_seguro WHERE clave = cTipoSeguro;
				LET cDesTipoSeguro = cTipoSeguro||'-'||cDesTipoSeguro;
				
				SELECT descripcion INTO cDesTipoPresta FROM tab_prestacion WHERE tipo_prestacion = sTipoPrestacion;
				LET cDesTipoPresta =  sTipoPrestacion||'-'||cDesTipoPresta;
				
				SELECT descripcion INTO cDescEdoSubviv FROM tab_estado_sub_viv WHERE estado_sub_viv = sEstadoSubViv;
				LET cDescEdoSubviv =  sEstadoSubViv||'-'||cDescEdoSubviv;
				
			END IF;
			
		END IF;
		
	END IF;
	
	IF iOpcion = 5 THEN --RETIRO P IMSS

		SELECT COUNT(*) INTO iRegafiliado FROM cuota_diaria WHERE n_seguro = cNss;
		IF iRegafiliado > 0 THEN
			LET iAfiliado = 1;
		END IF;

		SELECT MAX(rowid) INTO iRowid FROM ret_det_datamart WHERE nss = cNss AND curp = cCurp AND tipo_seguro IN ("CV","IM", "RT") AND diag_datamart IN (101,300,301,501) AND tipo_pension IN ('RA' ,'IP', 'OR', 'AS', 'IN', 'VE', 'VI', 'VO', 'CE') AND regimen = 97;
		IF iRowid > 0 THEN
			LET iExisteresolucion = 1;

			IF EXISTS (SELECT regimen FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid) THEN

				SELECT regimen,tipo_seguro,tipo_pension,tipo_prestacion, nombre_datamart,folio,sec_pension,fecha_ini_pen,fec_ini_pago, semanas_cotizadas, diag_datamart, estado_sub_viv,fecha_resolucion
				INTO cRegimen,cTipoSeguro,cTipoPension,sTipoPrestacion, cNombreDatamart, iFolio,cSecPension,dFechaIni,dFechaPago, sSemanasCotizadas, sDiagDatamart, sEstadoSubViv,dFechaResol
				FROM ret_det_datamart WHERE nss = cNss AND rowid = iRowid;

				SELECT descripcion INTO cDesRegimen FROM tab_regimen WHERE regimen = cRegimen;
				LET cDesRegimen = cRegimen||'-'||cDesRegimen;

				SELECT descripcion INTO cDesTipoPension FROM tab_pension WHERE tipo_pension = cTipoPension;
				LET cDesTipoPension = cTipoPension||'-E'||cDesTipoPension;

				SELECT descripcion, movimiento INTO cDesTipoRetiro, iIdMovimiento FROM tab_retiro WHERE tipo_retiro = 'E';
				LET cDesTipoRetiro = 'E-'||cDesTipoRetiro;

				SELECT descripcion INTO cDesTipoSeguro FROM tab_seguro WHERE clave = cTipoSeguro;
				LET cDesTipoSeguro = cTipoSeguro||'-'||cDesTipoSeguro;

				SELECT descripcion INTO cDesTipoPresta FROM tab_prestacion WHERE tipo_prestacion = sTipoPrestacion;
				LET cDesTipoPresta =  sTipoPrestacion||'-'||cDesTipoPresta;

				SELECT descripcion INTO cDescEdoSubviv FROM tab_estado_sub_viv WHERE estado_sub_viv = sEstadoSubViv;
				LET cDescEdoSubviv =  sEstadoSubViv||'-'||cDescEdoSubviv;

			END IF;

		END IF;

	END IF;
	
	RETURN 	NVL(iExisteresolucion, 0), NVL(iAfiliado, 0), NVL(cRegimen, ''), NVL(cTipoSeguro, ''), NVL(cTipoPension, ''), 
			NVL(sTipoPrestacion, -1), NVL(cNombreDatamart, ''), NVL(iFolio, 0), NVL(cSecPension, ''), NVL(dFechaIni, ''), 
			NVL(dFechaPago, ''), NVL(sSemanasCotizadas, 0), NVL(sDiagDatamart, 0), NVL(sEstadoSubViv, 0), NVL(dFechaResol, ''), 
			NVL(cDesRegimen, ''), NVL(cDesTipoPension, ''), NVL(cDesTipoPresta, ''), NVL(cDesTipoRetiro, ''), 
			NVL(cDesTipoSeguro, ''), NVL(cDescEdoSubviv, ''), NVL(iIdMovimiento, ''); 
	
END FUNCTION;