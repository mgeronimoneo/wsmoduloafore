-- Function: fnSeguimientoDecimos(character, double precision)

-- DROP FUNCTION fnSeguimientoDecimos(character, double precision);

CREATE OR REPLACE FUNCTION fnSeguimientoDecimos(character, double precision)
  RETURNS DOUBLE PRECISION AS
$BODY$

	/**************************************************************
	Autor:	        97408964 - Jose Antonio Corona Olivas
	Fecha:		03 de Agosto de 2017
	Descripción:    
	***************************************************************/
		
	DECLARE
		cNss			ALIAS FOR $1;
		dMontoAccS2		ALIAS FOR $2;
		dtFechaTAcelerada	DATE;
		iSieforeSB1		SMALLINT;
		iSieforeSB2		SMALLINT;
		dTAceleradaSB1		DOUBLE PRECISION;
		dTAceleradaSB2		DOUBLE PRECISION;
		iNumRegistro		SMALLINT;
		dPrecioActSB1		DOUBLE PRECISION;
		dPrecioActSB2		DOUBLE PRECISION;
		dDecimaParteSB1		DOUBLE PRECISION;
		dDecimaParteSB2		DOUBLE PRECISION;
		iDecimosPagados		SMALLINT;
		
	BEGIN
		
		IF NOT EXISTS (
			SELECT 'OK'
			FROM   discuentahistorico
			WHERE  nss             = cNss
			AND    tipo_movimiento = 215
			AND    replicar        = 1
			GROUP BY 1) 			
		THEN
			RETURN dMontoAccS2; --Regresa el mismo monto recibido como parametro
		ELSE
			SELECT MAX(fechaconversion)
			INTO   dtFechaTAcelerada
			FROM   discuentahistorico
			WHERE  nss             = cNss
			AND    tipo_movimiento = 215
			AND    replicar        = 1;
		END IF;

		iSieforeSB1   = 1 ;
		iSieforeSB2   = 2 ;
		dTAceleradaSB1 = 0.0;
		iNumRegistro     = 0;
		
		SELECT COUNT(*)
		INTO   iNumRegistro
		FROM   discuentahistorico A, ctactrcuenta B
		WHERE  A.nss               = B.nss
		AND    A.nss               = cNss
		AND    A.tipomovimiento   = 215                 --Cargo Transferencia de Decimos
		AND    A.fechaconversion >= B.fechaindtransf
		AND    A.fechaconversion <= dtFechaTAcelerada
		AND    A.replicar         = 1;

		IF  iNumRegistro = 0 THEN
			RETURN dMontoAccS2; --Regresa el mismo monto recibido como parametro
		END IF;

		--Obtenemos los precios de accion para la fecha de transferencia acelerada--
		SELECT valoraccion
		INTO   dPrecioActSb1
		FROM   valoraccion
		WHERE  siefore  = iSieforeSB1
		AND    fechavalor = dtFechaTAcelerada;
		
		SELECT valoraccion
		INTO   dPrecioActSb2
		FROM   valoraccion
		WHERE  siefore  = iSieforeSB2
		AND    fechavalor = dtFechaTAcelerada;

		IF dPrecioActSB1 IS NULL OR dPrecioActSB2 IS NULL THEN
			RAISE EXCEPTION 'Falta precio de accion para : %', dtFechaTAcelerada;
		END IF;

		--Determinamos la decima parte a transferir del monto en acciones--
		dDecimaParteSB2 = dMontoAccS2 / 10;

		-- Encontramos las fechas y precios de todos los aportes en decimos para el nss dado entre la fecha de--
		-- identificacion del trabajador hasta la fecha de transferencia acelerada.                           --

		CREATE TABLE tmp_vende_dec AS
			SELECT A.fecha_conversion, A.siefore AS siefore_vend , A.precio_accion AS precio_vend
			FROM   discuentahistorico A, ctactrcuenta B
			WHERE  A.nss               = b.nss
			AND    A.nss               = cNss
			AND    A.tipo_movimiento   = 215                 --Cargo Transferencia de Decimos
			AND    A.fecha_conversion >= b.fechaindtransf
			AND    A.fecha_conversion <= dtFechaTAcelerada
			AND    replicar        = 1
			GROUP BY 1,2,3
			ORDER BY 1;

		--Encontramos los movimientos 15(compras de decimos)--
		CREATE TABLE tmp_compra_dec AS
			SELECT A.fecha_conversion, A.siefore AS siefore_comp , A.precio_accion AS precio_comp
			FROM   discuentahistorico A, ctactrcuenta B
			WHERE  A.nss             = b.nss
			AND    A.nss             = cNss
			AND    A.tipo_movimiento = 15                    --Abono Transferencia Decimos
			AND    A.fecha_pago     >= b.fechaindtransf
			AND    A.fecha_pago     <= dtFechaTAcelerada
			AND    replicar        = 1
			GROUP BY 1, 2, 3
			ORDER BY 1;

		--Unimos los datos en una misma tabla temporal tmp_trasp_decimos--
		CREATE TABLE tmp_trasp_decimos AS
			SELECT A.fecha_conversion ,
			   A.siefore_vend     ,
			   A.precio_vend      ,
			   B.siefore_comp     ,
			   B.precio_comp
			FROM   tmp_vende_dec A, tmp_compra_dec B
			WHERE  A.fecha_conversion = B.fecha_conversion;

		DROP TABLE tmp_vende_dec ;
		DROP TABLE tmp_compra_dec;
		
		--Determinamos el número de traspasos realizados antes de la fecha de transferencia acelerada--
		SELECT COUNT(*)
		INTO   iDecimosPagados
		FROM   tmp_trasp_decimos;

		--La cantidad de acciones a transferir el dia de proceso de trans. acelerada es la resta del--
		--monto total menos las decimas partes enviadas antes de ese dia.                           --
		dTAceleradaSB2 = dMontoAccS2 - (dDecimaParteSB2 * iDecimosPagados);

		--A partir del número de decimos que faltaron por transferir, calculamos las acciones en SB1--
		--que seran transferidas el día de proceso de transferencia acelerada                       --
		dTAceleradaSB1 = (dTAceleradaSB2 * dPrecioActSB2) / dPrecioActSB1;

		--Sumamos las acciones obtenidas en el paso anterior mas las acciones que se transfirieron     --
		--en decimos. El valor obtenido debe estar en acciones de SB1 y es el valor que debe retornarse--

		FOR dPrecioActSB2, dPrecioActSB1 IN
		(	
			SELECT precio_vend , precio_comp
			FROM   tmp_trasp_decimos
		)
		LOOP
			dDecimaParteSB1 = (dDecimaParteSB2 * dPrecioActSB2) / dPrecioActSB1;

			dTAceleradaSB1  = dTAceleradaSB1 + dDecimaParteSB1;
		END LOOP;

		DROP TABLE tmp_trasp_decimos;
		RETURN dTAceleradaSB1;
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnSeguimientoDecimos(character, double precision)
  OWNER TO sysserviciosafore;