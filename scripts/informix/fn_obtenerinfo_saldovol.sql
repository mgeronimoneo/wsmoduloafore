DROP FUNCTION fn_obtenerinfo_saldovol(CHAR);
CREATE FUNCTION fn_obtenerinfo_saldovol(cNss CHAR(11))
RETURNING  INTEGER  AS folio,
		   INTEGER  AS consecutivo_lote,
		   CHAR(11) AS nss,
		   INTEGER  AS siefore,
		   INTEGER  AS subcuenta,
		   CHAR(10) AS fecha_valor,
		   CHAR(10) AS fecha_conversion,
		   DECIMAL(16,6)  AS monto_en_pesos,
		   DECIMAL(16,6)  AS monto_en_acciones,
		   DECIMAL(16,6)  AS saldo_acciones,
		   CHAR(10) AS fecha_saldo,
		   CHAR(12) AS usuario,
		   INTEGER  AS iRespuesta

    /***
    * Pendiente: 991
    * Programador: Eduardo Chavez - 92919235
    * Descripcion: Se migra consulta directa a funcion en safre para validar
    * el tipo de afiliacion en caso de la fecha de la tabla cta_saldo_vol aun no tenga los 6 meses
    ***/
    
	DEFINE iFolioServicio      INTEGER;	
	DEFINE iConsecutivoLote    INTEGER;
	DEFINE xNss                CHAR(11);
	DEFINE iSiefore            INTEGER;	
	DEFINE iSubcuenta          INTEGER;	
	DEFINE cFechaValor         CHAR(10);
	DEFINE cFechaConversion    CHAR(10); 
	DEFINE cFechaVentanilla    CHAR(10); 
	DEFINE iMontoPesos         DECIMAL(16,6);
	DEFINE iMontoAcciones      DECIMAL(16,6);
	DEFINE iSaldoAcciones      DECIMAL(16,6);
	DEFINE cFechaSaldo         CHAR(10);
	DEFINE cFecha6Meses        CHAR(10);
	DEFINE cFechaHoy           CHAR(10);
	DEFINE cUsuario            CHAR(12);
	DEFINE cFechaPatron        CHAR(10);
	DEFINE iTipoAfil           INTEGER;
	DEFINE iRespuesta          INTEGER;
	
	LET iConsecutivoLote    = 0;
	LET iSiefore            = 0;
	LET iSubcuenta          = 0;
	LET iMontoPesos         = 0;
	LET iMontoAcciones      = 0;
	LET iTipoAfil           = 0;
	LET iSaldoAcciones      = 0;
	LET iFolioServicio      = 0;
	LET iRespuesta          = 0;
	LET xNss                = '';
	LET cFechaConversion    = '';
	LET cFechaVentanilla    = '';
	LET cFechaHoy           = '';
	LET cFechaValor         = '';
	LET cFechaSaldo         = '';
	LET cFecha6Meses        = '';
	LET cFechaPatron        = '';
	LET cUsuario            = '';



    /*PDTE 991: Se crea variable para restarle 6 meses al dia presente, para hacer calculos*/
	SELECT to_char(ADD_MONTHS(TODAY,-6),'%Y-%m-%d'), TODAY INTO cFecha6Meses,cFechaHoy FROM systables WHERE tabid = 1;	



    /*PDTE 991: Se toman los datos que anteriormente hacia la consulta directa*/
    FOREACH

        SELECT 
        folio, 
        consecutivo_lote, 
        nss, 
        siefore, 
        subcuenta, 
        to_char(fecha_valor, '%Y-%m-%d')::CHAR(10),
        to_char(fecha_conversion, '%Y-%m-%d')::CHAR(10), 
        monto_en_pesos,
        monto_en_acciones, 
        saldo_acciones, 
        to_char(fecha_saldo, '%Y-%m-%d')::CHAR(10),
        usuario 
        INTO 
        iFolioServicio, 
        iconsecutivolote,
        xnss, 
        isiefore,
        isubcuenta,
        cfechavalor,
        cFechaConversion,
        imontopesos,
        imontoacciones,
        isaldoacciones,
        cfechasaldo,
        cusuario      
        FROM cta_saldo_vol WHERE nss = cNss	
    
	
		

        /*pdte 991: se compara fecha de conversion para validar si se avanzara o se toma de la tabla taa_rcv_recepcion*/
        IF ( cFechaConversion >  cFecha6Meses ) THEN
		
				LET iRespuesta = 1;
                /*pdte 991: se toma el tipo de afiliacion del trabajador*/
                SELECT tipo_solicitud INTO itipoafil FROM afi_mae_afiliado WHERE n_seguro  = cNss;
                
                /*pdte 991: se valida que sean tipo traspaso segun la tabla tab_tipo_solic*/
                IF ( itipoafil = 2 OR itipoafil = 7 OR itipoafil = 9 OR itipoafil = 13  OR itipoafil = 15 OR itipoafil = 16 OR itipoafil = 18) THEN 
                    
                    LET iRespuesta = 2;
                    /*pdte 991: se toma la fecha de la operacion de banxico en la afore cedente, por si existe mas de uno, toma el mas reciente*/
                    SELECT FIRST 1 to_char(fecha_vol_ven, '%Y-%m-%d')::CHAR(10),to_char(fecha_vol_pat, '%Y-%m-%d')::CHAR(10) 
                    INTO cFechaVentanilla,cFechaPatron FROM  taa_rcv_recepcion WHERE ident_operacion = '09'  AND nss = cNss
                    AND  fecha_mov_banxico  = (SELECT MAX(fecha_mov_banxico) FROM taa_rcv_recepcion WHERE ident_operacion = '09'  AND nss = cNss );


					
                    /*pdte 991: se valida que la fecha no sea default o vacia, ya que debe tomar una para validaciones en el serviciosafore*/
                    IF ( cFechaVentanilla != '0001-01-01' AND cFechaVentanilla IS NOT NULL AND (isubcuenta = 10 OR  isubcuenta = 23)) THEN
                    	LET cFechaConversion = cFechaVentanilla;
                    	LET iRespuesta = 3;
                    ELIF (cFechaPatron != '0001-01-01' AND cFechaPatron IS NOT NULL AND (isubcuenta = 3 OR  isubcuenta = 22)) THEN
                    	LET cFechaConversion = cFechaPatron;
                    	LET iRespuesta = 4;
                    END IF;

                END IF;

        END IF;



		/*pdte 991: retorno de los datos encontrados*/
        RETURN  iFolioServicio,
                iConsecutivoLote, 
                cNss,
                iSiefore,
                iSubcuenta,
                cFechaValor,
                cFechaConversion,
                iMontoPesos,
                iMontoAcciones,
                iSaldoAcciones,
                cFechaSaldo,
                cUsuario,
                iRespuesta
		WITH RESUME;
    END FOREACH;

END FUNCTION;