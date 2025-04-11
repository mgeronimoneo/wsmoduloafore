-- Function: fnconsultaresolpensionretaportaciones(character,character)

-- DROP FUNCTION fnconsultaresolpensionretaportaciones(character,character);

CREATE OR REPLACE FUNCTION fnconsultaresolpensionretaportaciones(character,character)
  RETURNS integer AS
$BODY$
/**************************************************************
-- B.D.        	: SERVICIOSAFORE
-- FECHA       	: 28/Dic/2017
-- DESCRIPCION 	: obtener la resolución de pensión para aportaciones IMSS / ISSSTE
-- iResp 		: 0.No hay Resolución / 1.Titular / 2.Beneficiario / 3.Ambos
**************************************************************/
DECLARE
	cCurp 		ALIAS FOR $1;
	cNss		ALIAS FOR $2;

	cTipoPension 	CHARACTER(2);
	iOpcion		SMALLINT;
	iResp 		SMALLINT;

BEGIN
	iOpcion = 0;
	iResp 	= 0;

	--Primero busca la resolución en la tabla de los IMSS
	IF EXISTS (SELECT 'x' FROM retresolucionestotalesimss WHERE curp = cCurp AND nss = cNss AND diagdatamart = 101) THEN
		SELECT tipopension INTO cTipoPension FROM retresolucionestotalesimss WHERE curp = cCurp AND nss = cNss AND diagdatamart = 101 ORDER BY secpension DESC LIMIT 1;
		iOpcion = 1;--IMSS
	ELSE
	--No se encuentra la resolución entonces lo buscamos en la tabla de los ISSSTE
		IF EXISTS (SELECT 'x' FROM retresolucionestotalesissste WHERE curp = cCurp AND diagdatamart = 101) THEN
			SELECT tipopension INTO cTipoPension FROM retresolucionestotalesissste WHERE curp = cCurp AND diagdatamart = 101 ORDER BY secpension DESC LIMIT 1;
			iOpcion = 2;--ISSSTE
		END IF;
	END IF;

	IF iOpcion != 0 THEN
		iResp = fnobtenertiposolicitantextipopension(iOpcion,cTipoPension);
	END IF;

	RETURN iResp;
END;

$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER;
ALTER FUNCTION fnconsultaresolpensionretaportaciones(character,character)
  OWNER TO sysserviciosafore;