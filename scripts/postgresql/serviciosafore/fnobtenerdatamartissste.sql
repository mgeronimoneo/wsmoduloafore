-- Function: fnobtenerdatamartissste(CHARACTER,CHARACTER)

-- DROP FUNCTION fnobtenerdatamartissste(CHARACTER,CHARACTER);

CREATE OR REPLACE FUNCTION fnobtenerdatamartissste(CHARACTER,CHARACTER)
  RETURNS SETOF tpobtenerdatamartissste AS
$BODY$
DECLARE

	cCurp		ALIAS FOR $1;
	cTipoRetiro	ALIAS FOR $2;
	tp		tpobtenerdatamartissste;

BEGIN

	--FOR registro IN select distinct fechacorte  from incincentivofacilitadorafore order by fechacorte desc limit 1
	FOR tp IN SELECT folio,nss,nssissste,curp,secpension,nombredatamart,
			paternodatamart,maternodatamart,numconcesion,delegacion,
			tipomovimiento,tiporetiro,regimen,tiposeguro,tipopension,
			cvepension,tipoprestacion,fechainiciopen,fecharesolucion,
			semanascotizadas,diagdatamart,estadoregistro
			FROM retresolucionestotalesissste
			WHERE curp=cCurp AND tiporetiro=cTipoRetiro
	LOOP
		RETURN NEXT tp;
	END LOOP;

	RETURN ;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1;
ALTER FUNCTION fnobtenerdatamartissste(CHARACTER,CHARACTER)
  OWNER TO postgres;
