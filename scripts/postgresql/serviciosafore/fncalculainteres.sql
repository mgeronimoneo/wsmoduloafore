-- Function: fnCalculaInteres(DOUBLE PRECISION, DATE, DATE)

-- DROP FUNCTION fnCalculaInteres(DOUBLE PRECISION, DATE, DATE);

CREATE OR REPLACE FUNCTION fnCalculaInteres(DOUBLE PRECISION, DATE, DATE)
  RETURNS DOUBLE PRECISION AS
$BODY$

	/**************************************************************
	Autor:	        97408964 - Jose Antonio Corona Olivas
	Fecha:		05 de Agosto de 2017
	Descripción:    
	***************************************************************/
		
	DECLARE
		dAporte			ALIAS FOR $1;
		dtFechaInicio		ALIAS FOR $2;
		dtFechaFin		ALIAS FOR $3;
		dInteres		DOUBLE PRECISION;
		dTasaValorViv		DOUBLE PRECISION;
		dInteresesViv		DOUBLE PRECISION;
		dtFechaSig		DATE;
		cFechaSig		CHARACTER(10);
		iDifDias		SMALLINT;
		iVerdadero		SMALLINT;
				
	BEGIN
		dInteres = 0 ;
		iVerdadero = 1 ;

		IF dtFechaInicio > dtFechaFin THEN
			RAISE EXCEPTION 'Fechas invalidas : %s > %s', dtFechaInicio, dtFechaFin;
		END IF;

		WHILE  iVerdadero = 1
		LOOP
			IF MONTH(dtFechaInicio) = 12 THEN
				cFechaSig = EXTRACT(YEAR FROM dtFechaInicio) + 1 ||'/01/01';
				dtFechaSig = cFechaSig ;
			ELSE
				cFechaSig = EXTRACT(YEAR FROM dtFechaInicio)|| '/' || EXTRACT(MONTH FROM dtFechaInicio) + 1 || '/01';
				dtFechaSig = cFechaSig ;
			END IF;

			IF dtFechaSig  > dtFechaFin THEN
				EXIT;
			ELSE
				SELECT tasa_valor
				INTO   dTasaValorViv
				FROM   tab_tasa_remanente
				WHERE  tasa_fecha  = dtFechaSig
				AND  tasa_origen = "VIV";

				IF dTasaValorViv IS NULL THEN
					RAISE EXCEPTION 'No existe tasa para: %s', dtFechaSig;
				END IF;

				iDifDias = dtFechaSig  - dtFechaInicio ;

				dInteresesViv = 0 ;
				dInteresesViv = dAporte  * iDifDias * dTasaValorViv/36000 ;

				dAporte  = dAporte + dInteresesViv ;
				dInteres  = dInteres + dInteresesViv ;

			END IF;
			
			dtFechaInicio = dtFechaSig ;
			
		END LOOP;

		RETURN dAporte;
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnCalculaInteres(DOUBLE PRECISION, DATE, DATE)
  OWNER TO sysserviciosafore;