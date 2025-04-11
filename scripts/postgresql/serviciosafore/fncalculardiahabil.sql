CREATE OR REPLACE FUNCTION fncalculardiahabil(date, integer)
  RETURNS date AS
$BODY$

        DECLARE
    dFechaActual        ALIAS FOR $1;
    iDias           ALIAS FOR $2;
        dFechaHabil     DATE;
        iContador       SMALLINT;
        iDiaSemana      SMALLINT;
    iFeriado        SMALLINT;
    bContinua       BOOLEAN;
    iIncremento     SMALLINT;
        
    BEGIN
        iContador = 0;
        bContinua = TRUE;
        dFechaHabil = dFechaActual;

        IF iDias > 0 THEN
            iIncremento = 1;
        ELSE
            iIncremento = -1;
        END IF;
        
        WHILE bContinua LOOP

            iFeriado = 0 ;
            iDiaSemana = ( SELECT EXTRACT(DOW FROM dFechaHabil)) ;

            IF iDiaSemana = 0 OR iDiaSemana = 6 THEN
                iFeriado = 1 ;
            ELSE
                IF EXISTS ( SELECT fechaferiada FROM DiasFeriados WHERE fechaferiada = dFechaHabil )  THEN
                    iFeriado = 1;
                END IF; 
            END IF;
                
            IF iFeriado = 1 THEN
                dFechaHabil = dFechaHabil + iIncremento;
            ELSE
                iContador = iContador + 1 ;
                IF iContador > ABS ( iDias ) THEN
                    bContinua = FALSE;
                ELSE
                    dFechaHabil = dFechaHabil + iIncremento;
                END IF;
            END IF;
        END LOOP;

        RETURN dFechaHabil;
    END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fncalculardiahabil(date, integer)  OWNER TO sysserviciosafore;