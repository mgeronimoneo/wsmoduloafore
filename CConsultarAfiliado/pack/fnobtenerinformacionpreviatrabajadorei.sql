drop FUNCTION fnobtenerinformacionpreviatrabajadorei (INT);
CREATE FUNCTION fnobtenerinformacionpreviatrabajadorei (iFolio INT)
RETURNING   CHAR(18) AS curp,
            CHAR(11) AS nss,
            CHAR(10) AS fechaservicio;

            DEFINE cCurp CHAR(18);
            DEFINE cNss CHAR(11);
            DEFINE cFechaServicio CHAR(10);
            DEFINE iMaxRowId INT;

           LET cCurp = '';
		   LET cNss = '';
		   LET cFechaServicio = '';
		   LET iMaxRowId = 0;

		   SELECT MAX(rowid) INTO iMaxRowId from rec_solicitud where folio_rec = iFolio AND tipo_id = 'S';
		   SELECT  NVL(n_unico,''),NVL(n_seguro,''),TO_CHAR(NVL(freclamo,'1900-01-01'::date), '%Y-%m-%d')
		   INTO cCurp, cNss, cFechaServicio
		   FROM rec_solicitud
		   WHERE folio_rec = iFolio
		   AND tipo_id = 'S' and rowid = iMaxRowId;

		   RETURN cCurp, cNss, cFechaServicio;
END FUNCTION;
