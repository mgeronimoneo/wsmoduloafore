CREATE OR REPLACE FUNCTION fnValidarUltimoRetParcial(iTipo INT, cNss char(11))
RETURNS INT AS
$BODY$
	DECLARE
		iRetorno INT;
		dFechaSolicitud DATE;
		dFechaAnterior  DATE;
	BEGIN
		iRetorno = 0;
		
		IF iTipo = 1 THEN --Verificar si tiene un retiro por desempleo
			--Se obtiene la fecha de solicitud maxima deacuerdo a un retiro por desempleo y al diagnostico de procesar
			SELECT MAX(fechasolicitud)
			INTO dFechaSolicitud 
			FROM retirosParciales 
			WHERE nss = cNss AND tipoPrestacion = 6 AND diagnosticoCuentaInd IN ('400', '106') AND numeroresolucion IN(
			SELECT numresolucion FROM retResolucionImss WHERE idComplemento= 0 AND tipoPrestacion = 6 AND nss = cNss);
			
			IF dFechaSolicitud IS NOT NULL and dFechaSolicitud != '19000101' THEN
				dFechaAnterior = (CURRENT_DATE - '5 year'::INTERVAL)::DATE;
				
				IF dFechaSolicitud > dFechaAnterior THEN
					iRetorno = 1;
				END IF;
			ELSE
				iRetorno = 0;
			END IF;
		ELSE --Verificar Si Existe El Retiro Por Matrimonio
			IF EXISTS (SELECT foliosolicitud FROM retirosParciales WHERE (nss = cNss AND tipoPrestacion = 7 AND diagnosticoCuentaInd IN ('400', '106'))
						OR (nss = cNss AND tipoPrestacion = 7 AND estadosolicitud = 2)) THEN
				iRetorno = 1;
			ELSE
				iRetorno = 0;
			END IF;
		END IF;
		RETURN iRetorno;
	END;
$BODY$
LANGUAGE plpgsql VOLATILE SECURITY DEFINER
COST 100;
ALTER FUNCTION fnValidarUltimoRetParcial(INT, CHAR(11)) OWNER TO postgres;
GRANT EXECUTE ON FUNCTION fnValidarUltimoRetParcial(INT, CHAR(11)) TO sysserviciosafore;