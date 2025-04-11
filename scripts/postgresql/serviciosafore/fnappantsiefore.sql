-- Function: fnAppAntSiefore(CHARACTER, SMALLINT, SMALLINT, DATE)

-- DROP FUNCTION fnAppAntSiefore(CHARACTER, SMALLINT, SMALLINT, DATE);

CREATE OR REPLACE FUNCTION fnAppAntSiefore(CHARACTER, SMALLINT, SMALLINT, DATE)
  RETURNS  SETOF tpAppAntSiefore  AS
$BODY$

	/**************************************************************
	Autor:	        97408964 - Jose Antonio Corona Olivas
	Fecha:		07 de Agosto de 2017
	Descripción:    
	***************************************************************/
		
	DECLARE  
		cNss			ALIAS FOR $1;
		iSieforeP		ALIAS FOR $2;
		iSubCuentaP		ALIAS FOR $3;
		dtFechaPension		ALIAS FOR $4;
		retorno			tpAppAntSiefore;

		-- Declaracion de variables
		cPeriodo		CHARACTER(6);
		dtFip			DATE;
		dtFechaVivAnt		DATE;
		dtFechaConversion	DATE;
		dtFechaTransf		DATE;

		iFolio			INTEGER;
		iConsecutivo		INTEGER;

		iMes			SMALLINT;
		iCociente		SMALLINT;
		iResiduo		SMALLINT;
		iSubcuenta		SMALLINT;
		iSiefore		SMALLINT;
		iSieforeCed		SMALLINT;
		iSieforeRec		SMALLINT;
		iContador		SMALLINT;
		iGrupo			SMALLINT;

		dPrecioCed		DOUBLE PRECISION;
		dPrecioRec		DOUBLE PRECISION;
		dMontoAcc		DOUBLE PRECISION;
		dParticipaAnt		DOUBLE PRECISION;
		dParticipaPos		DOUBLE PRECISION;
		dAcciones		DOUBLE PRECISION;
		dSumaAcciones		DOUBLE PRECISION;
		dPesos			DOUBLE PRECISION;

		dAporteAnt		DOUBLE PRECISION;
				
	BEGIN
		dtFechaVivAnt = '2004-08-01';
		iSieforeRec   = 0 ;
		dAporteAnt    = 0 ;
		iContador      = 0 ;
		iSiefore       = 0 ;
		dAcciones      = 0 ;
		dSumaAcciones    = 0 ;
		dtFip           = dtFechaPension;

		--01--Calcula el periodo de pension------------------------------
		iMes      = EXTRACT (MONTH FROM dtFechaPension);
		iCociente = iMes / 2;
		iResiduo  = iMes - ( iCociente * 2 );

		IF iResiduo = 1 THEN
			iMes = iMes + 1 ;
			dtFechaPension = EXTRACT (YEAR FROM dtFechaPension) || '/' || iMes || '/01' ;
		END IF;

		cPeriodo = to_char(dtFechaPension, 'YYYYMM');
		--01-------------------------------------------------------------

		--02--Fecha del primer decimo del trabajador---------------------
		SELECT MIN(fecha_conversion)
		INTO   dtFechaTransf
		FROM   tmp_dis_cuenta         --Es creada por el programa que invoca la funcion
		WHERE  nss             = cNss
		AND    tipo_movimiento = 215; --Cargo de decimos

		IF (dtFechaTransf IS NULL) OR (dtFechaTransf < dtFip) THEN
			dtFechaTransf = NOW()::DATE;
		END IF;
		--02-------------------------------------------------------------

		-- Se obtiene el grupo que corresponda a la subcuenta dada
		iGrupo = 0 ;

		SELECT A.gruporegimen
		INTO   iGrupo
		FROM   catAgrupaSubCtaRegimen A
		WHERE  A.subcuenta = iSubCuentaP ;

		PERFORM fnTraspasoSie(cNss, iGrupo, dtFechaTransf);

		-- Si al ejecutarse la verificacion de decimos no se encuentran registros
		-- se toma la fecha de transferencia hoy
		SELECT COUNT(*)
		INTO iContador
		FROM tmp_trasp_siefore;

		IF dtFechaTransf <> NOW()::DATE AND (iContador <= 0 OR iContador IS NULL) THEN 
			dtFechaTransf = NOW()::DATE;
		END IF;

		iContador = 0;

		--Se obtiene folio y consecutivo de los aportes posteriores a la FIP--
		--del nss dado como parametro                                       --
		FOR iFolio, iConsecutivo IN
		(
			SELECT  folio,
				consecreglote
			FROM    disDetAporte
			WHERE   nss = cNss
			AND     periodopago > cPeriodo
		)
		LOOP
			-- Se verifica primero si es subcuenta de vivienda para hacer el tratamiento de los aportes
			-- con fecha menor al 08/01/2004
			IF (iSubCuentaP <> 4) AND (iSubCuentaP <> 8) THEN
				FOR dtFechaConversion, iSubcuenta, iSiefore, dMontoAcc IN
				(
					-- Se obtienen los movimientos correspondientes a cada folio y consecutivo
					SELECT fecha_conversion     ,
					      subcuenta             ,
					      siefore               ,
					      SUM(monto_en_acciones)
					FROM   tmp_dis_cuenta
					WHERE  folio             = iFolio
					AND    consecutivo_lote  = iConsecutivo
					AND    subcuenta         = iSubCuentaP
					AND    fecha_conversion <= dtFechaTransf
					GROUP BY 1,2,3
				)
				LOOP
					dAcciones = dMontoAcc ;

					-- Obtenemos todos los posibles traspasos entre siefores anteriores a
					-- la fecha de id del trabajador
					FOR iSieforeCed, dPrecioCed, iSieforeRec, dPrecioRec IN
					(
						SELECT  sieforeced     ,
							precioced      ,
							sieforerec     ,
							preciorec	
						FROM   tmp_trasp_siefore
						WHERE  fechatraspaso >= dtFechaConversion
						AND    fechatraspaso <= dtFechaTransf
						ORDER BY fechatraspaso
					)
					LOOP
					   iContador = iContador + 1 ;
					   
					   dPesos    = dAcciones * dPrecioCed;
					   dAcciones = dPesos / dPrecioRec;
					   
					END LOOP;

					dSumaAcciones = dSumaAcciones + dAcciones;
					
				END LOOP;

				IF iContador = 0 THEN 
					iSieforeRec = iSiefore;
				END IF;

			ELSE
				iSieforeRec = 11;

				FOR dParticipaAnt IN
				(
					SELECT fnCambiaAporte(
						monto_en_pesos   ,
						fecha_conversion ,
						dtFechaVivAnt
					)
					FROM  tmp_dis_cuenta
					WHERE folio            = iFolio
					AND   consecutivo_lote = iConsecutivo
					AND   subcuenta        = iSubCuentaP
					AND   fecha_conversion < dtFechaVivAnt
				)
				LOOP
					IF dParticipaAnt IS NULL THEN
						dParticipaAnt = 0;
					END IF;

					dSumaAcciones = dSumaAcciones + dParticipaAnt ;
				END LOOP;

				SELECT SUM(monto_en_acciones)
				INTO   dParticipaPos
				FROM   tmp_dis_cuenta
				WHERE  folio            = iFolio
				AND    consecutivo_lote = iConsecutivo
				AND    subcuenta        = iSubCuentaP
				AND    fecha_conversion >= dtFechaVivAnt;

				IF dParticipaPos IS NULL THEN 
					dParticipaPos = 0;
				END IF;

					dSumaAcciones = dSumaAcciones + dParticipaPos ;
			END IF;
		
		END LOOP;

		DROP TABLE tmp_trasp_siefore;

		IF iSieforeRec = 0 THEN
			SELECT codigosiefore
			INTO   iSieforeRec
			FROM   ctanssregimen
			WHERE  nss           = cNss
			AND    gruporegimen = iGrupo;
		END IF;

		retorno.iSieforeRec = iSieforeRec;
		retorno.iSubCuenta = iSubCuentaP;
		retorno.dSumaAcciones = dSumaAcciones;
		
		RETURN NEXT retorno;
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnAppAntSiefore(CHARACTER, SMALLINT, SMALLINT, DATE)
  OWNER TO sysserviciosafore;