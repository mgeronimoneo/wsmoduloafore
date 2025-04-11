DROP FUNCTION fnobtenerinformaciontrabajadorei (INT);
CREATE FUNCTION fnobtenerinformaciontrabajadorei (iFolio INT)
RETURNING   CHAR(40) AS cApellidoPaterno,
            CHAR(40) AS cApellidoMaterno,
            CHAR(40) AS cNombre,
            CHAR(18) AS cCurp,
            CHAR(11) AS cNss,
            CHAR(10) AS cFechaServicio,
            CHAR(10) AS cFechaNacimiento,
            SMALLINT AS iSexo,
            CHAR(2) AS iEntidadNacimiento;

--FECHA        : 2015/02/19
--PROGRAMADOR  : Edgar Domingo Ruiz Paez
--MODIFICACIÓN : Extrae información de la tabla rec_solicitud y afi_mae_afiliado para el Expediente de Identificación 

    DEFINE cApellidoPaterno CHAR(40);
    DEFINE cApellidoMaterno CHAR(40);
    DEFINE cNombre CHAR(40);
    DEFINE cCurp CHAR(18);
    DEFINE cNss CHAR(11);
    DEFINE cFechaServicio CHAR(10);
    DEFINE cFechaNacimiento CHAR(10);
    DEFINE iSexo SMALLINT;
    DEFINE iEntidadNacimiento SMALLINT; 
    DEFINE cEntidadNacimiento CHAR(2);

    DEFINE cNti CHAR(1);
    DEFINE cCurpAfi CHAR(18);
    DEFINE cNssAfi CHAR(11);
    DEFINE iMaxRowId INT;
    
    LET cApellidoPaterno = '';
    LET cApellidoMaterno = '';
    LET cNombre = '';
    LET cCurp = '';
    LET cNss = '';
    LET cFechaServicio = '';
    LET cFechaNacimiento = '';
    LET iSexo = 0;
    LET iEntidadNacimiento = 0;
    LET cEntidadNacimiento = '';

    LET cCurpAfi = '';
    LET cNssAfi = '';
    LET iMaxRowId = 0;

    SELECT MAX(rowid) INTO iMaxRowId from rec_solicitud where folio_rec = iFolio AND tipo_id = 'S';
    SELECT trim(NVL(n_unico,'')),trim(NVL(n_seguro,'')),TO_CHAR(NVL(freclamo,'1900-01-01'::date), '%Y-%m-%d')
    INTO cCurpAfi, cNssAfi, cFechaServicio
    FROM rec_solicitud
    WHERE folio_rec = iFolio
    AND tipo_id = 'S' and rowid = iMaxRowId;

    SELECT NVL(n_seguro,''), NVL(n_unico,''), NVL(paterno,''), NVL(materno,''), NVL(nombres,''),
           TO_CHAR(NVL(fena,'1900-01-01'::date), '%Y-%m-%d'), NVL(sexo,0), NVL(estadon,0)
    INTO   cNss, cCurp, cApellidoPaterno, cApellidoMaterno, cNombre, 
           cFechaNacimiento, iSexo, iEntidadNacimiento
    FROM afi_mae_afiliado
    WHERE CASE WHEN LENGTH(cCurpAfi) > 0 THEN n_unico ELSE '1' END
          = 
          CASE WHEN LENGTH(cCurpAfi) > 0 THEN cCurpAfi ELSE '1' END
    AND CASE WHEN LENGTH(cNssAfi) > 0 THEN n_seguro ELSE '1' END
        = 
        CASE WHEN LENGTH(cNssAfi) > 0 THEN cNssAfi ELSE '1' END;

    IF iEntidadNacimiento != 0 THEN
        LET cEntidadNacimiento = LPAD(TO_CHAR(iEntidadNacimiento),2,'0');
    ELSE
        LET cEntidadNacimiento = TO_CHAR(iEntidadNacimiento);
    END IF;
	
    RETURN NVL(cApellidoPaterno, ''), 
           NVL(cApellidoMaterno, ''),
           NVL(cNombre, ''),
           NVL(cCurp, ''),
           NVL(cNss, ''),
           NVL(cFechaServicio,'1900-01-01'),
           NVL(cFechaNacimiento,'1900-01-01'),
           NVL(iSexo,0),
           NVL(cEntidadNacimiento,'');

END FUNCTION;