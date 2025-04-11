-- Function: fnobtenertiposolicitantextipopension(integer,character)

-- DROP FUNCTION fnobtenertiposolicitantextipopension(integer,character);

CREATE OR REPLACE FUNCTION fnobtenertiposolicitantextipopension(integer,character)
  RETURNS integer AS
$BODY$
/**************************************************************
-- B.D.        	: SERVICIOSAFORE
-- FECHA       	: 28/Dic/2017
-- DESCRIPCION 	: obtener tipo solicitud con el dato tipo pension de los catalogos: cattipopensionimss/cattipopensionissste
-- iResp 		: 1.Titular / 2.Beneficiario / 3.Ambos
**************************************************************/
DECLARE
	iOpcion		ALIAS FOR $1;
	cTipoPension 	ALIAS FOR $2;

	iResp 		SMALLINT;

BEGIN
	iResp = 0;

	IF iOpcion = 1 THEN
		SELECT tiposolretaportaciones INTO iResp FROM cattipopensionimss WHERE tipopension = cTipoPension;

	ELSIF iOpcion = 2 THEN
		SELECT tiposolretaportaciones INTO iResp FROM cattipopensionissste WHERE tipopension = cTipoPension;

	END IF;

	RETURN iResp;
END;

$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER;
ALTER FUNCTION fnobtenertiposolicitantextipopension(integer,character)
  OWNER TO sysserviciosafore;