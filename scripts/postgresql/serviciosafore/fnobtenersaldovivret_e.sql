-- Function: fnobtenersaldovivret_e(character, date, date)

-- DROP FUNCTION fnobtenersaldovivret_e(character, date, date);

CREATE OR REPLACE FUNCTION fnobtenersaldovivret_e(character, date, date)
  RETURNS SETOF tpobtenersaldovivret_e AS
$BODY$


DECLARE

	tp			tpobtenersaldovivret_e;
	sNss			ALIAS FOR $1;
	dFechasolicitud 	ALIAS FOR $2;
	dFechaIniPension	ALIAS FOR $3;
	ffechaviv		DATE;
	dprecioviv		NUMERIC;
	fapertura		DATE;
	dsaldoacc92		NUMERIC;
	dsaldoacc97		NUMERIC;
	ifipmes			INTEGER;
	sfipanio		INTEGER;
	speriodopago		CHARACTER(6);
	daportaacc		NUMERIC;

BEGIN
	ffechaviv	=	'1900-01-01';
	dprecioviv	=	0;
	fapertura	=	'1900-01-01';
	dsaldoacc92	=	0;
	dsaldoacc97	=	0;
	ifipmes		=	0;
	sfipanio	=	0;
	speriodopago	=	'';
	daportaacc	=	0;

	tp.dmontoAccViv =	0;
	tp.dmontoPesViv =	0;
	tp.Nss		=	sNss;


	-- Asignacion de valor para la fecha de vivienda
	SELECT TO_CHAR(dFechasolicitud,'YYYY-MM-01') INTO ffechaviv;

	RAISE NOTICE 'NSS %', sNss;
	-- Obtencion del precio de accion para vivienda
	SELECT valoraccion
	INTO dprecioviv
	FROM valoraccion
	WHERE fechavalor = ffechaviv
	AND siefore = 11;

	RAISE NOTICE 'PRECIO DEL DIA %', dprecioviv;

	IF dprecioviv IS NULL THEN
		RAISE NOTICE 'ERROR: Precio de accion Inexistente para el dia %', ffechaviv;
	END IF;

	-- Obtencion del la fecha de apertura de la cuenta.
	SELECT finicta
	INTO fapertura
	FROM afop_cuota_diaria
	WHERE n_seguro = sNss;

	RAISE NOTICE 'FECHA INI CTA %', fapertura;

	IF fapertura IS NULL THEN
		RAISE NOTICE 'ERROR: Cuenta no aperturada o fecha nula para %', pnss;
	END IF;

	-- Obtener saldo total de vivienda 92
	SELECT COALESCE(SUM(montoacciones),0)
	INTO dsaldoacc92
	FROM discuentahistorico
	WHERE nss = sNss
	AND subcuenta IN (8)
	AND siefore = 11
	AND replicar=1;

	RAISE NOTICE 'dsaldoacc92 %', dsaldoacc92;

	-- Obtener saldo total de vivienda 97
	SELECT COALESCE(SUM(montoacciones),0)
	INTO dsaldoacc97
	FROM discuentahistorico
	WHERE nss = sNss
	AND subcuenta IN (4)
	AND siefore = 11
	AND replicar=1;

	RAISE NOTICE 'dsaldoacc97 %', dsaldoacc97;

	-- Si la fecha de apertura de la cuenta es menor a la fecha de inicio de
	-- pension el monto a reportar se calculara restandoles al saldo de las
	-- subcuentas de vivienda las aportaciones con periodo de pago posterior a
	-- la fecha de inicio de pencion

	IF fapertura < dFechaIniPension THEN

		-- Para la fecha de inicio de pencion si el mes es non se toma el mes siguiente
		SELECT TO_CHAR(dFechaIniPension,'MM')::INTEGER INTO ifipmes;
		RAISE NOTICE 'MES %', ifipmes;
		SELECT TO_CHAR(dFechaIniPension,'YYYY')::INTEGER INTO sfipanio;
		RAISE NOTICE 'AÑO %', sfipanio;

		IF  ifipmes = 1 OR ifipmes = 3 OR ifipmes = 5 OR ifipmes = 7 OR ifipmes = 9 OR ifipmes = 11 THEN

			ifipmes = ifipmes + 1;

		END IF;
		-- Se le asigna el mes que le corresponde a la fecha maxima de periodo de pago
		SELECT sfipanio || SUBSTR('0'||ifipmes, 2) INTO speriodopago;

		RAISE NOTICE 'PERIODO DE PAGO %', speriodopago;

		SELECT SUM(COALESCE(b.partviv/1000000,0) + COALESCE(b.apliremviv/1000000 ,0) + COALESCE(b.apliextviv/1000000 ,0))
		INTO daportaacc
		FROM discuentahistorico AS a
		INNER JOIN disdetaporte AS b ON a.nss=b.nss AND a.folio=b.folio AND a.consecutivolote=b.consecreglote
		WHERE a.nss= sNss
		AND a.subcuenta=4
		AND a.tipomovimiento=1
		AND b.periodopago::INTEGER > speriodopago::INTEGER
		AND a.replicar=2
		GROUP BY b.nss;

		RAISE NOTICE 'daportaacc %', daportaacc;

		IF daportaacc IS NULL THEN
				daportaacc = 0;
		END IF;

	ELSE

		RAISE NOTICE 'SE PASO AL ELSE';

		SELECT COALESCE(round((s.pesviv97post/g.valoraccion )::NUMERIC,6),0)
		INTO daportaacc
		FROM retsolicitudsaldo AS s
		INNER JOIN valoraccion AS g ON g.fechavalor = (TO_CHAR(frecepprocesar,'YYYY')||'-'||TO_CHAR(frecepprocesar,'MM')||'-01')::DATE
		AND g.siefore = 11
		INNER JOIN marcaoperativaactual AS m ON m.nss = s.nss
		AND m.correlativo = s.idsolicitudsaldo
		AND m.codigomarca = 921
		WHERE s.nss = sNss
		AND s.estadosolicitud <> 110
		AND s.indsaldofip = 2;

		IF  daportaacc IS NULL THEN
				daportaacc = 0;
		END IF;


	END IF;

	tp.dmontoaccviv = ROUND(dsaldoacc92 + dsaldoacc97 - daportaacc, 5);
	tp.dmontopesviv = ROUND(tp.dmontoaccviv * dprecioviv,5);

	RETURN NEXT tp;

END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;
ALTER FUNCTION fnobtenersaldovivret_e(character, date, date)
  OWNER TO sysaforeglobal;
