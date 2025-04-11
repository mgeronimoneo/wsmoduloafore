-- DROP FUNCTION fncalcularetroactivopmgimss(character, character);

CREATE OR REPLACE FUNCTION fncalcularetroactivopmgimss(character, character)
  RETURNS tpcalcularetroactivopmgimss AS
$BODY$
	DECLARE
		cNss			ALIAS FOR $1;
		cSecuencia		ALIAS FOR $2;
		tpDatos			tpcalcularetroactivopmgimss;
		dFechaIniPen	DATE;
		dFechaMax		DATE;
		dFechaFinCiclo	DATE;
		sFechaDesde		DATE;
		sFechaDefinida	DATE;
		cAnio			CHARACTER(4);
		cMes			CHARACTER(2);
		cAnioIni		CHARACTER(4);
		cMesIni			CHARACTER(2);
		iAnioIni		INTEGER;
		iMesIni			INTEGER;
		dpImpMensual	DOUBLE PRECISION;
		dpRetroActivo	DOUBLE PRECISION;
		dpDevengado		DOUBLE PRECISION;
		iContMeses		INTEGER;
		iFolio			INTEGER;
		iEstatus		INTEGER;
	BEGIN
		
		dFechaIniPen = '1900-01-01'::DATE;
		dFechaMax = '1900-01-01'::DATE;
		dFechaFinCiclo = '1900-01-01'::DATE;
		sFechaDesde = '1900-01-01'::DATE;
		sFechaDefinida = '1900-01-01'::DATE;
		cAnio = ''; cMes  = '';
		cAnioIni = ''; cMesIni = '';
		dpImpMensual = 0.0;
		dpRetroActivo = 0.0;
		dpDevengado = 0.0;
		iContMeses = 0;
		iFolio = 0;
		iEstatus = 0;
		tpDatos.retroactivo = 0.0;
		tpDatos.devengado = 0.0;
		tpDatos.estatus = 0;
		
		SELECT fechadesde INTO sFechaDesde
		FROM catpmghistoricaimss
		WHERE fechahasta = sFechaDefinida;
		
		SELECT EXTRACT(YEAR FROM CURRENT_DATE)::CHARACTER(4) INTO cAnio;
		SELECT EXTRACT(MONTH FROM CURRENT_DATE)::CHARACTER(2) INTO cMes;
		IF LENGTH(TRIM(cMes)) = 1 THEN
			IF cMes::INTEGER = 1 THEN
				cMes = '12';
				cAnio = ( cAnio::INTEGER -1 )::CHARACTER(4);
			ELSE
				cMes = ( cMes::INTEGER -1 )::CHARACTER(4);
			END IF;
		ELSE
			cMes = ( cMes::INTEGER -1 )::CHARACTER(4);
		END IF;

		IF cMes::INTEGER < 10 THEN
			cMes = '0'||cMes;
		END IF;
		
		dFechaFinCiclo = cAnio ||'-'||cMes||'-'||'01';
		dFechaFinCiclo = dFechaFinCiclo::DATE;
		
		--Busca la ultima resolucion por folio (Resoluciones nuevas)
		IF EXISTS (SELECT folio FROM retresolucionestotalesimss WHERE nss = cNss AND secpension = cSecuencia) THEN
		   
			SELECT MAX( folio ) INTO iFolio
			FROM retresolucionestotalesimss
			WHERE nss = cNss
			AND diagdatamart IN (101,300,301,501)
			AND secpension = cSecuencia;
		   
			IF iFolio > 0 THEN
				--RECUPERA DATOS POR FOLIO
				SELECT fechainipen INTO dFechaIniPen
				FROM retresolucionestotalesimss
				WHERE nss = cNss
				AND secpension = cSecuencia
				AND folio = iFolio;
				 
				IF dFechaIniPen = sFechaDefinida THEN
					SELECT fecinipago INTO dFechaIniPen
					FROM retresolucionestotalesimss
					WHERE nss = cNss
					AND secpension = cSecuencia
					AND folio = iFolio;
				END IF;
			ELSE
				-- Se busca con criterio de resoluciones anteriores
				SELECT MAX( feccargadatamart ) INTO dFechaMax
				FROM retresolucionestotalesimss
				WHERE nss = cNss
				AND sec_pension = cSecuencia;
				
				IF dFechaMax > sFechaDefinida THEN
					--Recupera los datos por fecha carga datamart
					SELECT fechainipen INTO dFechaIniPen
					FROM retresolucionestotalesimss
					WHERE nss = cNss
					AND secpension = cSecuencia
					AND fechacargaafore = dFechaMax;

					IF dFechaIniPen = sFechaDefinida THEN
						SELECT fecinipago INTO dFechaIniPen
						FROM retresolucionestotalesimss
						WHERE nss = cNss
						AND secpension = cSecuencia
						AND fechacargaafore = dFechaMax;
					END IF;
				ELSE
					-- Si no se encuentra buscamos por fecha carga afore
					SELECT MAX(fechacargaafore) INTO dFechaMax
					FROM retresolucionestotalesimss
					WHERE nss = cNss
					AND secpension = cSecuencia;
					 
					--Recupera los datos por fecha cargar afore
					SELECT fechainipen INTO dFechaIniPen
					FROM   retresolucionestotalesimss
					WHERE nss = cNss
					AND secpension = cSecuencia
					AND fechacargaafore = dFechaMax;
					
					IF dFechaIniPen = sFechaDefinida THEN
						SELECT fecinipago INTO dFechaIniPen
						FROM retresolucionestotalesimss
						WHERE nss = cNss
						AND secpension = cSecuencia
						AND fechacargaafore = dFechaMax;
					END IF;
				END IF; -- if fecha > 1900-01-01
			END IF; -- if folio > 0
			
			SELECT EXTRACT(YEAR FROM dFechaIniPen)::CHARACTER(4) INTO cAnioIni;
			SELECT EXTRACT(MONTH FROM dFechaIniPen)::CHARACTER(2) INTO cMesIni;
			
			iAnioIni = cAnioIni::INTEGER;
			iMesIni	= cMesIni::INTEGER;
			
			dFechaIniPen = cAnioIni||'-'||cMesIni||'-'||'01';
			dFechaIniPen = dFechaIniPen::DATE;
			IF dFechaIniPen <> sFechaDefinida THEN
				WHILE dFechaFinCiclo >= dFechaIniPen LOOP
					
					iContMeses = iContMeses + 1;
					iEstatus = 1;
					
					IF dFechaIniPen >= sFechaDesde THEN
						SELECT importemensual11p INTO dpImpMensual
						FROM catpmghistoricaimss
						WHERE fechahasta = sFechaDefinida;
					ELSE
						SELECT importemensual11p INTO dpImpMensual
						FROM catpmghistoricaimss
						WHERE fechadesde <= dFechaIniPen
						AND fechahasta >= dFechaIniPen;
					END IF;
					
					IF iContMeses < 12 THEN
						dpRetroActivo =  dpRetroActivo + dpImpMensual;
					ELSE
						dpDevengado = dpDevengado + dpImpMensual;
					END IF;
					
					IF iMesIni < 12 THEN
						iMesIni = iMesIni + 1;
						cMesIni = iMesIni::CHARACTER(2);
					ELSE
						iMesIni = 1;
						cMesIni = '01';
						iAnioIni = iAnioIni + 1;
						cAnioIni = iAnioIni::CHARACTER(4);
					END IF;
					
					dFechaIniPen = cAnioIni||'-'||cMesIni||'-'||'01';
					dFechaIniPen = dFechaIniPen::DATE;
				END LOOP;
			END IF;
		END IF; -- if exists
		
		tpDatos.retroactivo = dpRetroActivo;
		tpDatos.devengado = dpDevengado;
		tpDatos.estatus = iEstatus;
		
		RETURN tpDatos;
	END;

$BODY$
  LANGUAGE plpgsql VOLATILE SECURITY DEFINER
  COST 100;
ALTER FUNCTION fncalcularetroactivopmgimss(character, character)
  OWNER TO sysserviciosafore;
GRANT EXECUTE ON FUNCTION fncalcularetroactivopmgimss(character, character) TO public;
GRANT EXECUTE ON FUNCTION fncalcularetroactivopmgimss(character, character) TO sysserviciosafore;
GRANT EXECUTE ON FUNCTION fncalcularetroactivopmgimss(character, character) TO postgres;
