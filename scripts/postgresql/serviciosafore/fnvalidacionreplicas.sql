-- Function: fnvalidacionreplicas(INTEGER, INTEGER)

-- DROP FUNCTION fnvalidacionreplicas(INTEGER, INTEGER);

CREATE OR REPLACE FUNCTION fnvalidacionreplicas(INTEGER, INTEGER)
  RETURNS INTEGER AS
$BODY$
	DECLARE
		iOpcion		ALIAS FOR $1;
		iIdBinario	ALIAS FOR $2;
		cDependencias	CHARACTER(100);
		iConteo		INTEGER;
		iRetorno	INTEGER;
		
	BEGIN
		iRetorno = 0;

		IF(iOpcion = 1) THEN
			SELECT dependencias INTO cDependencias FROM catdependenciasreplicabinario WHERE idbinario = iIdBinario;
			
			IF(cDependencias <> '' AND cDependencias IS NOT NULL) THEN 
				cDependencias = REPLACE(cDependencias, '|', ', ');

				SELECT COUNT(*) INTO iConteo FROM controlwsmoduloafore 
				WHERE idcontrol BETWEEN 100 AND 199 
				AND idcontrol IN (SELECT regexp_split_to_table(cDependencias, ',')::INTEGER) 
				AND TRIM(valor)::DATE!= CURRENT_DATE;

				IF(iConteo > 0) THEN
					iRetorno=1; --REPLICA INCOMPLETA
				ELSE
					iRetorno=0; --REPLICA COMPLETA
				END IF;
			END IF;
		END IF;
			
		RETURN iRetorno;
	end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnvalidacionreplicas(INTEGER, INTEGER)
  OWNER TO sysserviciosafore;