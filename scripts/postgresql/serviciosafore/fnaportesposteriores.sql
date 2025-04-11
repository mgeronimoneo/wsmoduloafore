-- Function: fnAportesPosteriores(CHARACTER, SMALLINT, SMALLINT, DATE)

-- DROP FUNCTION fnAportesPosteriores(CHARACTER, SMALLINT, SMALLINT, DATE);

CREATE OR REPLACE FUNCTION fnAportesPosteriores(CHARACTER, SMALLINT, SMALLINT, DATE)
  RETURNS  SETOF tpAportesPosteriores  AS
$BODY$

	/**************************************************************
	Autor:	        97408964 - Jose Antonio Corona Olivas
	Fecha:		08 de Agosto de 2017
	Descripción:    
	***************************************************************/
		
	DECLARE  
		cNss			ALIAS FOR $1;
		iSieforeP		ALIAS FOR $2;
		iSubCuentaP		ALIAS FOR $3;
		dtFechaInicioPension	ALIAS FOR $4;
		retorno			tpAportesPosteriores;

		-- Declaracion de variables
		dtFechaTransf	DATE;

		iSieforeRec	SMALLINT;
		iSubcuenta	SMALLINT;
		iIndTransf	SMALLINT;

		dSaldoAccHoy	DOUBLE PRECISION;
		dSumaAcciones	DOUBLE PRECISION;
				
	BEGIN
		iIndTransf	= 0;
		iSieforeRec	= 0;
		iSubcuenta	= 0;

		dSumaAcciones	= 0;
		dSaldoAccHoy	= 0;


		SELECT A.iSieforeRec, A.iSubcuenta, A.dSumaAcciones
		INTO iSieforeRec, iSubcuenta, dSumaAcciones
		FROM fnAppAntSiefore (cNss, iSieforeP, iSubCuentaP, dtFechaInicioPension) AS A;

		IF (iSubCuentaP <> 4 AND iSubCuentaP <> 8) THEN

			SELECT indtransferencia
			INTO   iIndTransf
			FROM   ctactrcuenta
			WHERE  nss = cNss;

			IF iIndTransf <> 0 THEN

				dSaldoAccHoy = dSumaAcciones;
				SELECT fnSeguimientoDecimos(cNss, dSaldoAccHoy) INTO dSumaAcciones ;

			END IF;

		END IF;

		retorno.iSieforeRec = iSieforeRec;
		retorno.iSubCuenta = iSubCuentaP;
		retorno.dSumaAcciones = dSumaAcciones;
		
		RETURN NEXT retorno;
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnAportesPosteriores(CHARACTER, SMALLINT, SMALLINT, DATE)
  OWNER TO sysserviciosafore;