-- DROP FUNCTION fnvalidarsolicitudretiropmgimss(CHARACTER(13));

CREATE OR REPLACE FUNCTION fnvalidarsolicitudretiropmgimss(CHARACTER(13))
RETURNS tpoperacionespmgimss AS
$BODY$
	DECLARE
		cNss			ALIAS FOR $1;
		tpDatos			tpoperacionespmgimss;
		sRechazado		SMALLINT;
		sLiquidado		SMALLINT;
		sConsecutivo	INTEGER;
		sMensualidad	SMALLINT;
		sEstado			SMALLINT;
		dMonto       	DOUBLE PRECISION;
		cEstatusSol		CHARACTER(2);
		
	BEGIN
		tpDatos.Mensaje = '';
		tpDatos.Codigo	= 0;
		tpDatos.MontoPmg = 0.0;
		
		sRechazado = 80; 
		sLiquidado = 70; 
		
		sConsecutivo = 0; 
		sMensualidad = 0; 
		sEstado 	 = 0;
		dMonto 		 = 0.0;
		cEstatusSol = '';
		
		SELECT 'OK' INTO cEstatusSol FROM pensolicitudpmgimss
		WHERE nss = cNss AND estadosolicitud NOT IN (sRechazado, sLiquidado)
		GROUP BY 1;

		SELECT consecutivo INTO sConsecutivo
		FROM pensolicitudpmgimss WHERE nss = cNss
		AND estadosolicitud NOT IN (sRechazado, sLiquidado);

		IF cEstatusSol =  'OK' THEN
			
		   SELECT MAX(nummensualidad) INTO sMensualidad FROM penctrpagodetimss
		   WHERE nss = cNss AND consecutivo = sConsecutivo;
		  
		   SELECT estado INTO sEstado FROM penctrpagodetimss
		   WHERE  nss = cNss AND consecutivo = sConsecutivo
		   AND nummensualidad = sMensualidad;
		   
		   --Condicion con la cual sabemos si se encuentra una solicitud previa
		   IF sEstado != sRechazado THEN 
				tpDatos.Mensaje = 'EXISTE UNA SOLICITUD PREVIA PARA ESTE NSS';
				tpDatos.Codigo = 2;
			ELSE
				tpDatos.Mensaje = 'NO EXISTE UNA SOLICITUD PREVIA PARA ESTE NSS';
				tpDatos.Codigo = 3;
		   END IF;
		   
		ELSE
			--Retornamos 1, si el no se encuntra registro
			tpDatos.Mensaje = 'NO SE ENCONTRO REGISTRO';
			tpDatos.Codigo = 1;
		END IF;
		
		RETURN tpDatos;
	END;
$BODY$
LANGUAGE plpgsql VOLATILE SECURITY DEFINER
COST 100;
ALTER FUNCTION fnvalidarsolicitudretiropmgimss(CHARACTER(13)) OWNER TO postgres;
ALTER FUNCTION fnvalidarsolicitudretiropmgimss(CHARACTER(13)) OWNER TO sysserviciosafore;