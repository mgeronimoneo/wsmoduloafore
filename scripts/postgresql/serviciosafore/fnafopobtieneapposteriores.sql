-- Function: fnAfopObtieneApPosteriores(SMALLINT, CHARACTER, DATE, SMALLINT)

-- DROP FUNCTION fnAfopObtieneApPosteriores(SMALLINT, CHARACTER, DATE, SMALLINT);

CREATE OR REPLACE FUNCTION fnAfopObtieneApPosteriores(SMALLINT, CHARACTER, DATE, SMALLINT)
  RETURNS SETOF tpAfopObtieneApPosteriores AS
$BODY$

	/**************************************************************
	Autor:	        97408964 - Jose Antonio Corona Olivas
	Fecha:		24 de Agosto de 2017
	Descripción:    
	***************************************************************/
		
	DECLARE
		iOpcion			ALIAS FOR $1;
		cNss			ALIAS FOR $2;
		dtFechaInicioPension	ALIAS FOR $3;
		iGrupo			ALIAS FOR $4;
		tp			tpAfopObtieneApPosteriores;

		-- Declaracion de variables

		cNombreTablas		CHAR(20);
		iSie			SMALLINT; 
		iSieafore		SMALLINT;
		iSubcuenta		SMALLINT;
		dMontoAcciones		DOUBLE PRECISION;
		dMontoAportacionPost	DOUBLE PRECISION;
		cMensaje		CHAR(200);
		iNumSesionID		INTEGER;
		dtTraspaso		DATE;
				
	BEGIN
		cNombreTablas		= '';
		iSie			= 0;
		iSieafore		= 0;
		iSubcuenta		= 0;
		dMontoAcciones		= 0;
		dMontoAportacionPost	= 0;
		cMensaje		= '';
		iNumSesionID		= 0;
		dtTraspaso		= '1900-01-01'::DATE;

		IF iOpcion = 1 THEN

			
			SELECT MAX(finicta) 
			INTO dtTraspaso
			FROM afop_cuota_diaria 
			WHERE n_seguro = cNss;

			IF dtFechaInicioPension  > dtTraspaso THEN
			
				IF dtFechaInicioPension  < (now() - interval '1 year')::DATE THEN

					CREATE TEMP TABLE tmp_dis_cuenta ( 
					tipo_movimiento  	SMALLINT NOT NULL,
					subcuenta        	SMALLINT NOT NULL,
					siefore          	SMALLINT,
					folio            	INTEGER NOT NULL,
					consecutivo_lote 	INTEGER,
					nss              	CHAR(11) NOT NULL,
					curp             	CHAR(18),
					folio_sua        	CHAR(6),
					fecha_pago       	DATE,
					fecha_valor      	DATE,
					fecha_conversion 	DATE,
					monto_en_pesos   	DECIMAL,
					monto_en_acciones	DECIMAL,
					precio_accion    	DECIMAL,
					dias_cotizados   	SMALLINT,
					sucursal         	CHAR(10),
					id_aportante     	CHAR(11),
					estado           	INTEGER,
					fecha_proceso    	DATE,
					usuario          	CHAR(8),
					fecha_archivo    	DATE,
					etiqueta         	INTEGER 
					);

					-- se agrega barrido para el split dis_cuenta FERNANDO MURILLO 07/09/2011
					-- SELECT fnAfopCrearSaldosDisCuentaSplit 
					-- INTO iNumSesionID 
					-- FROM fnAfopCrearSaldosDisCuentaSplit(1, cNss, dtFechaInicioPension ); 

					INSERT INTO tmp_dis_cuenta 
					-- (tipo_movimiento, subcuenta, siefore, folio, consecutivo_lote, nss, curp, 
-- 					folio_sua, fecha_pago, fecha_valor, fecha_conversion, monto_en_pesos, monto_en_acciones, precio_accion, 
-- 					dias_cotizados, sucursal, id_aportante, estado, fecha_proceso, usuario, fecha_archivo, etiqueta)
					(tipo_movimiento, subcuenta, siefore, folio, consecutivo_lote, nss, fecha_pago, fecha_conversion, 
					monto_en_pesos, monto_en_acciones, precio_accion)
					SELECT 
					-- tipomovimiento, subcuenta, siefore, folio, consecutivolote, nss, curp, folio_sua, fecha_pago, 
-- 					fecha_valor, fecha_conversion, monto_en_pesos, monto_en_acciones, precio_accion, dias_cotizados, 
-- 					sucursal, id_aportante, estado, fecha_proceso, usuario, fecha_archivo, etiqueta
					tipomovimiento, subcuenta, siefore, folio, consecutivolote, nss, 
					fechapago, fechaconversion, montopesos, montoacciones, precioaccion
					FROM discuentahistorico
					WHERE  nss = cNss 
					AND tipomovimiento NOT IN (888,999);

					 
					-- SELECT tipo_movimiento, subcuenta, siefore, folio, consecutivo_lote, nss, curp, folio_sua, fecha_pago, 
-- 					-- fecha_valor, fecha_conversion, monto_en_pesos, monto_en_acciones, precio_accion, dias_cotizados, 
-- 					-- sucursal, id_aportante, estado, fecha_proceso, usuario, fecha_archivo, etiqueta
-- 					-- FROM stmp_dis_cuenta_split
-- 					-- WHERE nss = cNss  
-- 					-- And SessionID = iNumSesionID; 


					CREATE INDEX tmp_dis_cuenta1 ON tmp_dis_cuenta (folio, consecutivo_lote, subcuenta, siefore);
					
					--DELETE FROM stmp_dis_cuenta_split WHERE SessionID = iNumSesionID AND nss = cNss;

					FOR iSubcuenta IN 

						SELECT subcuenta
						FROM catAgrupaSubcuenta
						WHERE grupo::INTEGER = iGrupo
						AND subcuenta::INTEGER > 0

					LOOP
						SELECT A.isieforerec, A.isubcuenta, A.dsumaacciones
						INTO iSieafore, iSubcuenta, dMontoAcciones
						FROM fnAportesPosteriores(cNss, iSie, iSubcuenta, dtFechaInicioPension) AS A;

						IF dMontoAcciones IS NULL OR dMontoAcciones < 0 THEN
							dMontoAcciones = 0;
						END IF;

						dMontoAportacionPost  =  dMontoAportacionPost + dMontoAcciones;
					END LOOP;

					IF dMontoAportacionPost <= 0 THEN 
						cMensaje = 'No existen aportes de periodos posteriores al otorgamiento de su pensión, por lo cual no podemos realizar la disposición de recursos solicitada.';
					END IF;

					IF EXISTS (SELECT 1 FROM pg_tables WHERE tablename = 'tmp_dis_cuenta') THEN
							DROP TABLE tmp_dis_cuenta;
					END IF;
		   
				ELSE -- fip  un año menor a la fecha solictud
					cMensaje = 'Debe haber transcurrido mínimamente un año a partir del otorgamiento de su inicio de pensión, por lo cual no podemos realizar la disposición del recurso solicitado.';
				END IF;
			END IF;
		END IF;

		tp.cMontoAportacionPosterior = dMontoAportacionPost::CHARACTER(25);
		tp.cMensaje = cMensaje;
		
		RETURN NEXT tp;
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnAfopObtieneApPosteriores(SMALLINT, CHARACTER, DATE, SMALLINT)
  OWNER TO sysserviciosafore;