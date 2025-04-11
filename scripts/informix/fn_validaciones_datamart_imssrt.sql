CREATE FUNCTION fn_validaciones_datamart_imssrt (p_opcion INTEGER, p_cNss CHAR(11),p_cCurp CHAR(18), p_Tipo_documento INTEGER)

	RETURNING
	INTEGER 	AS Existeresolucion,
	INTEGER 	AS Existeafiliado,
	CHAR(2) 	AS regimen,
	CHAR(2) 	AS tipo_seguro,
	CHAR(2) 	AS tipo_pension,
	SMALLINT 	AS tipo_prestacion,
	CHAR(50)	AS NombreDatamart,
	INT 	  	AS Folio,
	CHAR(2) 	AS sec_pension,
	DATE	 	AS fecha_ini_pen,
	DATE 		AS FechaPago,
	SMALLINT 	AS semanas_cotizadas,
	SMALLINT 	AS diag_datamart,
	SMALLINT  	AS EstadoSubViv,
	DATE 		AS fecha_resolucion,
	CHAR(40) 	AS descripcion_regimen,
	CHAR(40) 	AS descripcion_pension,
	CHAR(40) 	AS descripcion_prestacion,
	CHAR(40) 	AS descripcion_retiro,
	CHAR(40) 	AS descripcion_seguro,
	CHAR(40)	AS DesEdoSubViv,
	INT			AS IdMovimiento;

	DEFINE iExisteresolucion		INTEGER;
	DEFINE Existeafiliado			INTEGER;
	DEFINE iDiag_datamart			SMALLINT;
	DEFINE sSec_pension				CHAR(2);
	DEFINE dFecha_ini_pen			DATE;
	DEFINE dFecha_resolucion		DATE;
	DEFINE sNombre_datamart			CHAR(50);
	DEFINE sTipo_retiro				CHAR(2);
	DEFINE sDescripcion_retiro		CHAR(40);
	DEFINE sRegimen					CHAR(2);
	DEFINE sDescripcion_regimen		CHAR(40);
	DEFINE sTipo_seguro				CHAR(2);
	DEFINE sDescripcion_seguro		CHAR(40);
	DEFINE sTipo_pension			CHAR (2);
	DEFINE sDescripcion_pension		CHAR(40);
	DEFINE iTipo_prestacion			SMALLINT;
	DEFINE sDescripcion_prestacion	CHAR(40);
	DEFINE iSemanas_cotizadas		SMALLINT;
	DEFINE iAfiliado				INTEGER;
	DEFINE iFolio					INTEGER;
	DEFINE sEstadoSubViv			INTEGER;
	DEFINE dFechaPago				DATE;
	DEFINE cDescEdoSubviv			CHAR(40);
	DEFINE iIdMovimiento			INTEGER;

	DEFINE iRowid					INTEGER;
	DEFINE sClave					CHAR (2);
	DEFINE iCount					INTEGER;
	DEFINE sMensaje					CHAR(250);

	LET iDiag_datamart				= 101;
	LET sSec_pension				= '';
	LET dFecha_ini_pen				= MDY(01,01,1900);
	LET dFecha_resolucion			= MDY(01,01,1900);
	LET sNombre_datamart			= '';
	LET sTipo_retiro				= '';
	LET sDescripcion_retiro			= '';
	LET sRegimen					= '';
	LET sDescripcion_regimen 		= '';
	LET sTipo_seguro				= '';
	LET sDescripcion_seguro 		= '';
	LET sTipo_pension 				= '';
	LET sDescripcion_pension 		= '';
	LET iTipo_prestacion			= 0;
	LET sDescripcion_prestacion 	= '';
	LET iSemanas_cotizadas			= 0;
	LET iRowid						= 0;
	LET sClave						= '';
	LET iCount						= 0;
	LET sMensaje					= '';
	LET iExisteresolucion			= 0;
	LET iAfiliado					= 0;
	LET iFolio						= 0;
	LET sEstadoSubViv				= 0;
	LET dFechaPago					= MDY(01,01,1900);
	LET cDescEdoSubviv				= '';
	LET iIdMovimiento				= 0;

-- Tipo de retiro E
IF p_opcion = 1 THEN
	IF EXISTS (SELECT * FROM ret_det_datamart WHERE nss= p_cNss AND curp = p_cCurp AND tipo_seguro in ('IV','RT')
	AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO') AND diag_datamart = 101) THEN

				SELECT td.regimen, td.tipo_prestacion, tp.descripcion
				INTO sRegimen, iTipo_prestacion, sDescripcion_prestacion
				FROM ret_tipo_documento AS td
				INNER JOIN tab_prestacion AS tp
				ON (td.tipo_prestacion = tp.tipo_prestacion)
				WHERE tipo_documento = p_Tipo_documento;

				SELECT COUNT (*) INTO iCount
				FROM ret_det_datamart
				WHERE nss = p_cNss
				AND regimen = sRegimen
				AND tipo_seguro in ('IV','RT')
				AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO')
				AND tipo_prestacion = iTipo_prestacion
				AND diag_datamart = 101;

			IF iCount > 0 THEN

				SELECT MAX(rowid)
				INTO iRowid
				FROM ret_det_datamart
				WHERE nss = p_cNss
				AND regimen = sRegimen
				AND tipo_seguro in ('IV','RT')
				AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO')
				AND tipo_prestacion = iTipo_prestacion
				AND diag_datamart = 101
				AND folio >= 0;

				SELECT diag_datamart, sec_pension, fecha_ini_pen, fecha_resolucion, nombre_datamart, tipo_retiro, regimen,
				tipo_seguro, tipo_pension, tipo_prestacion, semanas_cotizadas
				INTO iDiag_datamart, sSec_pension, dFecha_ini_pen, dFecha_resolucion, sNombre_datamart, sTipo_retiro, sRegimen,
				sTipo_seguro, sTipo_pension, iTipo_prestacion, iSemanas_cotizadas
				FROM ret_det_datamart
				WHERE nss = p_cNss
				AND regimen = sRegimen
				AND tipo_seguro in ('IV','RT')
				AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO')
				AND tipo_prestacion = iTipo_prestacion
				AND diag_datamart = 101
				AND rowid = iRowid;

				SELECT descripcion
				INTO sDescripcion_retiro
				FROM tab_retiro WHERE tipo_retiro = sTipo_retiro;
				SELECT descripcion
				INTO sDescripcion_regimen
				FROM tab_regimen WHERE regimen = sRegimen;
				SELECT descripcion
				INTO  sDescripcion_seguro
				FROM tab_seguro WHERE clave = sTipo_seguro;
				SELECT descripcion
				INTO sDescripcion_pension
				FROM tab_pension WHERE tipo_pension = sTipo_pension;
				SELECT descripcion
				INTO sDescripcion_prestacion
				FROM tab_prestacion WHERE tipo_prestacion = iTipo_prestacion;

			END IF;

		END IF;

-- Tipo de retiro D
ELSE IF p_opcion = 2 THEN

	IF EXISTS (SELECT * FROM ret_det_datamart WHERE nss= p_cNss AND curp = p_cCurp AND tipo_seguro in ('CV','IM')
	AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO') AND diag_datamart = 101) THEN

				SELECT td.regimen, td.tipo_prestacion, tp.descripcion
				INTO sRegimen, iTipo_prestacion, sDescripcion_prestacion
				FROM ret_tipo_documento AS td
				INNER JOIN tab_prestacion AS tp
				ON (td.tipo_prestacion = tp.tipo_prestacion)
				WHERE tipo_documento = p_Tipo_documento;

				SELECT COUNT (*) INTO iCount
				FROM ret_det_datamart
				WHERE nss = p_cNss
				AND regimen = sRegimen
				AND tipo_seguro in ('CV','IM')
				AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO')
				AND tipo_prestacion = iTipo_prestacion
				AND diag_datamart = 101;

			IF iCount > 0 THEN

				SELECT MAX(rowid)
				INTO iRowid
				FROM ret_det_datamart
				WHERE nss = p_cNss
				AND regimen = sRegimen
				AND tipo_seguro in ('CV','IM')
				AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO')
				AND tipo_prestacion = iTipo_prestacion
				AND diag_datamart = 101
				AND folio >= 0;

				SELECT diag_datamart, sec_pension, fecha_ini_pen, fecha_resolucion, nombre_datamart, tipo_retiro, regimen,
				tipo_seguro, tipo_pension, tipo_prestacion, semanas_cotizadas
				INTO iDiag_datamart, sSec_pension, dFecha_ini_pen, dFecha_resolucion, sNombre_datamart, sTipo_retiro, sRegimen,
				sTipo_seguro, sTipo_pension, iTipo_prestacion, iSemanas_cotizadas
				FROM ret_det_datamart
				WHERE nss = p_cNss
				AND regimen = sRegimen
				AND tipo_seguro in ('CV','IM')
				AND tipo_pension in ('AS','CE','IN','OR','VE','VI','VO')
				AND tipo_prestacion = iTipo_prestacion
				AND diag_datamart = 101
				AND rowid = iRowid;

				SELECT descripcion
				INTO sDescripcion_retiro 
				FROM tab_retiro WHERE tipo_retiro = sTipo_retiro;
				SELECT descripcion 
				INTO sDescripcion_regimen
				FROM tab_regimen WHERE regimen = sRegimen;
				SELECT descripcion 
				INTO  sDescripcion_seguro
				FROM tab_seguro WHERE clave = sTipo_seguro;
				SELECT descripcion 
				INTO sDescripcion_pension
				FROM tab_pension WHERE tipo_pension = sTipo_pension;
				SELECT descripcion
				INTO sDescripcion_prestacion
				FROM tab_prestacion WHERE tipo_prestacion = iTipo_prestacion; 

			END IF;

		END IF;
	END IF; 
END IF;
	
	RETURN 
	NVL(iExisteresolucion, 0), NVL(iAfiliado, 0), NVL(sRegimen, ''), NVL(sTipo_seguro, ''), NVL(sTipo_pension, ''), 
	NVL(iTipo_prestacion, -1), NVL(sNombre_datamart, ''), NVL(iFolio, 0), NVL(sSec_pension, ''), NVL(dFecha_ini_pen, ''), 
	NVL(dFechaPago, ''), NVL(iSemanas_cotizadas, 0), NVL(iDiag_datamart, 0), NVL(sEstadoSubViv, 0), NVL(dFecha_resolucion, ''), 
	NVL(sDescripcion_regimen, ''), NVL(sDescripcion_pension, ''), NVL(sDescripcion_prestacion, ''), NVL(sDescripcion_retiro, ''), 
	NVL(sDescripcion_seguro, ''), NVL(cDescEdoSubviv, ''), NVL(iIdMovimiento, 0); 

END FUNCTION;