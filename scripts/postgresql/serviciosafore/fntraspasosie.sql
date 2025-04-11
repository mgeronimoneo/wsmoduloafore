-- Function: fnTraspasoSie(CHARACTER, SMALLINT, DATE)

-- DROP FUNCTION fnTraspasoSie(CHARACTER, SMALLINT, DATE);

CREATE OR REPLACE FUNCTION fnTraspasoSie(CHARACTER, SMALLINT, DATE)
  RETURNS VOID AS
$BODY$

	/**************************************************************
	Autor:	        97408964 - Jose Antonio Corona Olivas
	Fecha:		05 de Agosto de 2017
	Descripción:    
	***************************************************************/
		
	DECLARE
		cNss			ALIAS FOR $1;
		iGrupoRegimen		ALIAS FOR $2;
		dtFecha			ALIAS FOR $3;
		iNumPreciosCed		SMALLINT;
		iNumPreciosRec		SMALLINT;
		iExisteTss		SMALLINT;

		v_mensaje		CHAR(100);
		dtFechaCorte		DATE;
				
	BEGIN
		iExisteTss = 0 ;

		SELECT fecha_corte
		INTO dtFechaCorte
		FROM gloFechaCorte
		WHERE corte = 4;

		IF EXISTS 
		(  
			SELECT nss
			FROM tessolicitud
			WHERE nss = cNss
			AND fechatraspaso <= dtFecha
			AND tipotraspaso   = 1
		)
		THEN
			iExisteTss = 1 ;
		END IF;

		CREATE TABLE tmp_sie_ced AS
		(	
			SELECT  nss           ,
				fechatraspaso,
				sieforeced   ,
				valoraccion   ,
				folio
			FROM    tessolicitud ,
				valoraccion
			WHERE nss = cNss
			AND fechatraspaso <= dtFecha
			AND fechatraspaso <  dtFechaCorte
			AND fechatraspaso  = fechavalor
			AND sieforeced     = siefore
			AND estado         = 103 --********  Liquidado
			AND tipotraspaso  <> 3

			UNION ALL

			SELECT  nss           ,
				fechatraspaso,
				sieforeced   ,
				valoraccion   ,
				folio
			FROM    tessolicitud    ,
				valoraccion
			WHERE nss = cNss
			AND fechatraspaso <= dtFecha
			AND fechatraspaso >= dtFechaCorte
			AND fechatraspaso  = fechavalor
			AND gruporegimen   = iGrupoRegimen
			AND sieforeced     = siefore
			AND estado         = 103 --********  Liquidado
			AND tipotraspaso IN (1,5,10,13)
		);

		-- fprintf(cNumPreciosCed, '%l',  EXEC SQL sqlca.sqlerrd[2]);
 		-- iNumPreciosCed = cNumPreciosCed::SMALLINT;
		-- iNumPreciosCed = sqlca.sqlerrd[2];
		SELECT count(*) INTO iNumPreciosCed FROM tmp_sie_ced;

		CREATE TABLE tmp_sie_rec AS
		(
			SELECT  nss           ,
				fechatraspaso,
				sieforerec   ,
				valoraccion   ,
				folio
			FROM    tessolicitud ,
				valoraccion
			WHERE nss = cNss
			AND fechatraspaso <= dtFecha
			AND fechatraspaso <  dtFechaCorte
			AND fechatraspaso  = fechavalor
			AND sieforerec     = siefore
			AND estado         = 103 --********  Liquidado
			AND tipotraspaso  <> 3

			UNION ALL

			SELECT  nss           ,
				fechatraspaso,
				sieforerec   ,
				valoraccion   ,
				folio
			FROM    tessolicitud ,
				valoraccion
			WHERE nss = cNss
			AND fechatraspaso <= dtFecha
			AND fechatraspaso >= dtFechaCorte
			AND fechatraspaso  = fechavalor
			AND gruporegimen   = iGrupoRegimen
			AND sieforerec     = siefore
			AND estado          = 103 --********  Liquidado
			AND tipotraspaso IN (1,5,10,13)
		);

		-- iNumPreciosRec = sqlca.sqlerrd[2];
		SELECT count(*) INTO iNumPreciosCed FROM tmp_sie_ced;

		IF ( iNumPreciosCed <> iNumPreciosRec ) AND iExisteTss = 1 
		THEN
		   DROP TABLE tmp_sie_ced;
		   DROP TABLE tmp_sie_rec;
		   RAISE EXCEPTION 'Falta precio de accion';
		ELSE
			CREATE TABLE tmp_trasp_siefore AS
			(
				SELECT  c.fechatraspaso,
					c.sieforeced   ,
					c.valoraccion AS precioced,
					r.sieforerec   ,
					r.valoraccion AS preciorec
				FROM    tmp_sie_ced c,
					tmp_sie_rec r
				WHERE c.nss = r.nss
				AND c.folio            = r.folio
				AND c.fechatraspaso   = r.fechatraspaso
			);
		      
		   DROP TABLE tmp_sie_ced;
		   DROP TABLE tmp_sie_rec;
		END IF;
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnTraspasoSie(CHARACTER, SMALLINT, DATE)
  OWNER TO sysserviciosafore;