-- Function: fnCambiaAporte(DOUBLE PRECISION, DATE, DATE)

-- DROP FUNCTION fnCambiaAporte(DOUBLE PRECISION, DATE, DATE);

CREATE OR REPLACE FUNCTION fnCambiaAporte(DOUBLE PRECISION, DATE, DATE)
  RETURNS DOUBLE PRECISION AS
$BODY$

	/**************************************************************
	Autor:	        97408964 - Jose Antonio Corona Olivas
	Fecha:		05 de Agosto de 2017
	Descripción:    
	***************************************************************/
		
	DECLARE
		dMontoPesos		ALIAS FOR $1;
		dtFechaValor		ALIAS FOR $2;
		dtFechaSaldo		ALIAS FOR $3;
		dParticipacion		DOUBLE PRECISION;
		dPrecioFecha		DOUBLE PRECISION;
				
	BEGIN
		IF dMontoPesos IS NULL OR dtFechaValor IS NULL OR dtFechaSaldo IS NULL THEN
			RAISE EXCEPTION 'TODOS los parametros deben ser <> NULL';
		END IF;

		IF dtFechaValor >= '2004/08/01' THEN
			RAISE EXCEPTION 'Fecha valor: % >= 1 Agosto 2004', dtFechaValor;
		ELSE
			IF dtFechaSaldo <>  '2004/08/01' THEN
				RAISE EXCEPTION 'Fecha saldo: % <> 1 Agosto 2004', dtFechaSaldo;
			ELSE
				dPrecioFecha =  
				( 
					SELECT valoraccion
					FROM valoraccion
					WHERE fechavalor = dtFechaSaldo
					AND siefore = 11
				);
				IF dPrecioFecha IS NULL THEN
					RAISE EXCEPTION 'NO existe precio accion para: %', dtFechaSaldo;
				ELSE
					dParticipacion = fnCalculaInteres (dMontoPesos,
								   dtFechaValor,
								   dtFechaSaldo);

					dParticipacion = dParticipacion / dPrecioFecha ;
				END IF;
			END IF;
		END IF;
		
		RETURN dParticipacion;
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION fnCambiaAporte(DOUBLE PRECISION, DATE, DATE)
  OWNER TO sysserviciosafore;