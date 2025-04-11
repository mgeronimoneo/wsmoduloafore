--DROP FUNCTION fn_calcularetroactivopmgimss( CHAR(11), CHAR(02), DATE);
CREATE FUNCTION fn_calcularetroactivopmgimss(cNss CHAR(11), cSecPension CHAR(02), dFecha DATE)
RETURNING DECIMAL(16,6) AS Retroactivo,
          DECIMAL(16,6) AS Devengado,
		  INTEGER		AS Estatus,
		  INTEGER		AS Registros;

    DEFINE dRetroactivo    DECIMAL(16,6);
    DEFINE dDevengado      DECIMAL(16,6);
    DEFINE dImpMensual     DECIMAL(10,2);

    DEFINE dtFechaActiva	DATE;
    DEFINE dtFechaFinCiclo	DATE;
    DEFINE dtFechaIniPen	DATE;

    DEFINE sContAnio	SMALLINT;

    DEFINE dtMaxFecha	DATE;
    DEFINE iMaxFolio	INTEGER;
    DEFINE iEstatusCon	INTEGER;
    DEFINE iRegistros	INTEGER;

    LET dRetroactivo = 0;
    LET dImpMensual  = 0;
    LET dDevengado   = 0;
    LET sContAnio    = 0;
    LET iEstatusCon  = 0;
    LET iRegistros   = 0;
	
	--Obtenemos el numero de registros que se encuentran en la discuenta para saber el monto a pagar
	SELECT COUNT(*) INTO iRegistros FROM dis_cuenta WHERE nss = cNss;

    -- Obtenemos la fecha actual para el pago de pmg
    SELECT fecha_desde INTO dtFechaActiva
    FROM tab_pmg_historica
    WHERE fecha_hasta IS NULL;

    -- La fecha para detener el ciclo es el primer dia del mes anterior
    LET dtFechaFinCiclo = MONTH(dFecha)||"/01/"||YEAR(dFecha);
    LET dtFechaFinCiclo = dtFechaFinCiclo - 1 UNITS MONTH;

    -- Busca la ultima resolucion por folio
   SELECT MAX(folio) INTO iMaxFolio                         
   FROM ret_det_datamart                     
   WHERE nss = cNss     
   AND diag_datamart IN (101,300,301,501)
   AND sec_pension = cSecPension;
    
   IF iMaxFolio IS NULL THEN
      --Busca con criterioa de resolucion anteriores
      SELECT MAX(fec_carga_datamart) INTO dtMaxFecha
      FROM ret_det_datamart
      WHERE nss = cNss
      AND sec_pension = cSecPension;

      IF dtMaxFecha IS NULL THEN
         -- Si no se encuentra fecha entonces buscamos por fecha carga afore
         SELECT MAX(fecha_carga_afore) INTO dtMaxFecha
         FROM ret_det_datamart
         WHERE nss = cNss
         AND sec_pension = cSecPension;
       
         -- Recupera datos por fecha carga afore
         SELECT fecha_ini_pen INTO dtFechaIniPen
         FROM ret_det_datamart
         WHERE nss = cNss
         AND sec_pension = cSecPension
         AND fecha_carga_afore = dtMaxFecha;

         IF dtFechaIniPen IS NULL OR dtFechaIniPen = "01/01/0001" THEN
            SELECT fec_ini_pago INTO dtFechaIniPen
            FROM ret_det_datamart
            WHERE nss = cNss
            AND sec_pension = cSecPension
            AND fecha_carga_afore = dtMaxFecha;
         END IF
      ELSE
         -- Recupera datos por fecha carga datamart
         SELECT fecha_ini_pen INTO dtFechaIniPen
         FROM ret_det_datamart
         WHERE nss = cNss
         AND sec_pension = cSecPension
         AND fec_carga_datamart = dtMaxFecha;

         IF dtFechaIniPen IS NULL OR dtFechaIniPen = "01/01/0001" THEN
            SELECT fec_ini_pago INTO dtFechaIniPen
            FROM ret_det_datamart
            WHERE nss = cNss
            AND sec_pension = cSecPension
            AND fec_carga_datamart = dtMaxFecha;
         END IF
      END IF
   ELSE
      -- Recupera Datos por folios
      SELECT fecha_ini_pen INTO dtFechaIniPen
      FROM ret_det_datamart
      WHERE nss = cNss
      AND sec_pension = cSecPension
      AND folio = iMaxFolio;
      
      IF dtFechaIniPen IS NULL OR dtFechaIniPen = "01/01/0001" THEN
         SELECT fec_ini_pago INTO dtFechaIniPen
         FROM ret_det_datamart
         WHERE nss = cNss
         AND sec_pension = cSecPension
         AND folio = iMaxFolio;
      END IF
   END IF

    LET dtFechaIniPen = MONTH(dtFechaIniPen)||"/01/"||YEAR(dtFechaIniPen);

    -- Ciclamos para acumular el monto de pmg para cada mes desde
    -- la fecha ini pen hasta un mes antes a la fecha dada
    WHILE dtFechaFinCiclo >= dtFechaIniPen

        LET sContAnio = sContAnio + 1;
		LET iEstatusCon = 1;

        -- Si la fecha del ciclo es mayor a la actual vigente se paga el monto actual de pmg
        IF dtFechaIniPen >= dtFechaActiva THEN
			SELECT importe_mensual_11p INTO dImpMensual
			FROM tab_pmg_historica
			WHERE fecha_hasta IS NULL;
        ELSE
			SELECT importe_mensual_11p INTO dImpMensual
			FROM tab_pmg_historica
			WHERE fecha_desde <= dtFechaIniPen
			AND fecha_hasta >= dtFechaIniPen;
        END IF;

        IF sContAnio <= 12 THEN
            LET dRetroactivo = dRetroactivo + dImpMensual;
        ELSE
            LET dDevengado = dDevengado + dImpMensual;
        END IF;

        LET dtFechaIniPen = dtFechaIniPen + 1 UNITS MONTH;

    END WHILE ;

    RETURN NVL(dRetroactivo, 0.0), NVL(dDevengado, 0.0), NVL(iEstatusCon, 0), NVL(iRegistros, 0);

END FUNCTION;