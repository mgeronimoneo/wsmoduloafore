-- Function: fnvalidadatamartimssnuevos(smallint, character, character)

-- DROP FUNCTION fnvalidadatamartimssnuevos(smallint, character, character);

CREATE OR REPLACE FUNCTION fnvalidadatamartimssnuevos(smallint, character, character)
  RETURNS SETOF tpvalidadatamartimssnuevos AS
$BODY$

DECLARE
	sOpcion			ALIAS FOR $1;
	cNss			ALIAS FOR $2;
	cCurp			ALIAS FOR $3;
	tp  			tpvalidadatamartimssnuevos;

	iRegafiliado	 	INTEGER;
	iKeyx			INTEGER;
	iNoCorrecto		SMALLINT;

	BEGIN
		tp.iAfiliado	=0;
		iRegafiliado	=0;
		iKeyx		=0;
		iNoCorrecto	=0;

		IF sOpcion=1 THEN

			SELECT COUNT(*) INTO iRegafiliado FROM afop_cuota_diaria WHERE n_seguro=cNss;
			IF iRegafiliado > 0 THEN
				tp.iAfiliado=1;
			END IF;

			SELECT MAX(keyx) INTO iKeyx FROM retresolucionestotalesimss WHERE nss=cNss AND curp = cCurp AND diagdatamart IN (101,300,301,501);
			IF iKeyx > 0 THEN
				tp.iExisteresolucion = 1;

				IF EXISTS (SELECT regimen FROM retresolucionestotalesimss WHERE nss=cNss AND keyx=iKeyx) THEN

					SELECT regimen,tiposeguro,tipopension,tipoprestacion,nombredatamart,folio,secpension,fechainipen,fecinipago,semanascotizadas,diagdatamart,estadosubviv,fecharesolucion
					INTO tp.cRegimen,tp.cTipoSeguro,tp.cTipoPension,tp.sTipoPrestacion,tp.cNombreDatamart,tp.iFolio,tp.cSecPension,tp.dFechaIni,tp.dFechaPago,tp.sSemanasCotizadas,tp.sDiagDatamart,tp.sEstadoSubViv,tp.dFechaResol
					FROM retresolucionestotalesimss  WHERE nss = cNss AND keyx=iKeyx;

					SELECT descripcion INTO tp.cDesRegimen FROM catregimenimss WHERE regimen=tp.cRegimen;
					tp.cDesRegimen= tp.cRegimen||'-'||tp.cDesRegimen;

					SELECT descripcion INTO tp.cDesTipoPension FROM cattipopensionimss WHERE tipopension=tp.cTipoPension;
					tp.cDesTipoPension= tp.cTipoPension||'-'||tp.cDesTipoPension;

					SELECT descripcion,tipomovimiento INTO tp.cDesTipoRetiro,tp.iIdMovimiento FROM catretiros WHERE tiporetiro='S';
					tp.cDesTipoRetiro='S-'||tp.cDesTipoRetiro;

					SELECT descripcion INTO tp.cDesTipoSeguro FROM cattiposeguroimss  WHERE tiposeguro=tp.cTipoSeguro;
					tp.cDesTipoSeguro=tp.cTipoSeguro||'-'||tp.cDesTipoSeguro;

					SELECT descripcion INTO tp.cDesTipoPresta FROM cattipoprestacionimss WHERE tipoprestacion=tp.sTipoPrestacion;
					tp.cDesTipoPresta=tp.sTipoPrestacion||'-'||tp.cDesTipoPresta;

					SELECT descripcion INTO tp.cDescEdoSubviv FROM catestadosubvivrt WHERE estadosubviv=tp.sEstadoSubViv::CHARACTER;
					tp.cDescEdoSubviv=tp.sEstadoSubViv::CHARACTER||'-'||tp.cDescEdoSubviv;

				END IF;

			END IF;

		END IF;

		IF sOpcion=2 THEN

			SELECT COUNT(*) INTO iRegafiliado FROM afop_cuota_diaria WHERE n_seguro=cNss;
			IF iRegafiliado > 0 THEN
				tp.iAfiliado=1;
			END IF;

			SELECT MAX(keyx) INTO iKeyx FROM retresolucionestotalesimss WHERE nss=cNss AND curp = cCurp AND diagdatamart IN (101,300,301,210,302,303,501);
			IF iKeyx > 0 THEN
				tp.iExisteresolucion = 1;

				IF EXISTS (SELECT regimen FROM retresolucionestotalesimss WHERE nss=cNss AND keyx=iKeyx) THEN

					SELECT regimen,tiposeguro,tipopension,tipoprestacion,nombredatamart,folio,secpension,fechainipen,fecinipago,semanascotizadas,diagdatamart,estadosubviv,fecharesolucion
					INTO tp.cRegimen,tp.cTipoSeguro,tp.cTipoPension,tp.sTipoPrestacion,tp.cNombreDatamart,tp.iFolio,tp.cSecPension,tp.dFechaIni,tp.dFechaPago,tp.sSemanasCotizadas,tp.sDiagDatamart,tp.sEstadoSubViv,tp.dFechaResol
					FROM retresolucionestotalesimss  WHERE nss = cNss AND keyx=iKeyx;

					SELECT descripcion INTO tp.cDesRegimen FROM catregimenimss WHERE regimen=tp.cRegimen;
					tp.cDesRegimen= tp.cRegimen||'-'||tp.cDesRegimen;

					SELECT descripcion INTO tp.cDesTipoPension FROM cattipopensionimss WHERE tipopension=tp.cTipoPension;
					tp.cDesTipoPension= tp.cTipoPension||'-E'||tp.cDesTipoPension;

					SELECT descripcion,tipomovimiento INTO tp.cDesTipoRetiro,tp.iIdMovimiento FROM catretiros WHERE tiporetiro='E';
					tp.cDesTipoRetiro='E-'||tp.cDesTipoRetiro;

					SELECT descripcion INTO tp.cDesTipoSeguro FROM cattiposeguroimss  WHERE tiposeguro=tp.cTipoSeguro;
					tp.cDesTipoSeguro=tp.cTipoSeguro||'-'||tp.cDesTipoSeguro;

					SELECT descripcion INTO tp.cDesTipoPresta FROM cattipoprestacionimss WHERE tipoprestacion=tp.sTipoPrestacion;
					tp.cDesTipoPresta=tp.sTipoPrestacion||'-'||tp.cDesTipoPresta;

					SELECT descripcion INTO tp.cDescEdoSubviv FROM catestadosubvivrt WHERE estadosubviv=tp.sEstadoSubViv::CHARACTER;
					tp.cDescEdoSubviv=tp.sEstadoSubViv::CHARACTER||'-'||tp.cDescEdoSubviv;

				END IF;

			END IF;

		END IF;

		IF sOpcion=3 THEN

			SELECT COUNT(*) INTO iRegafiliado FROM afop_cuota_diaria WHERE n_seguro=cNss;
			IF iRegafiliado > 0 THEN
				tp.iAfiliado=1;
			END IF;

			SELECT MAX(keyx) INTO iKeyx FROM retresolucionestotalesimss WHERE nss=cNss AND curp = cCurp AND tiposeguro IN ('CV','IV') AND diagdatamart IN (101,300,301,210,302,303);
			IF iKeyx > 0 THEN
				tp.iExisteresolucion = 1;

				IF EXISTS (SELECT regimen FROM retresolucionestotalesimss WHERE nss=cNss AND keyx=iKeyx) THEN

					SELECT regimen,tiposeguro,tipopension,tipoprestacion,nombredatamart,folio,secpension,fechainipen,fecinipago,semanascotizadas,diagdatamart,estadosubviv,fecharesolucion
					INTO tp.cRegimen,tp.cTipoSeguro,tp.cTipoPension,tp.sTipoPrestacion,tp.cNombreDatamart,tp.iFolio,tp.cSecPension,tp.dFechaIni,tp.dFechaPago,tp.sSemanasCotizadas,tp.sDiagDatamart,tp.sEstadoSubViv,tp.dFechaResol
					FROM retresolucionestotalesimss  WHERE nss = cNss AND keyx=iKeyx;

					SELECT descripcion INTO tp.cDesRegimen FROM catregimenimss WHERE regimen=tp.cRegimen;
					tp.cDesRegimen= tp.cRegimen||'-'||tp.cDesRegimen;

					SELECT descripcion INTO tp.cDesTipoPension FROM cattipopensionimss WHERE tipopension=tp.cTipoPension;
					tp.cDesTipoPension= tp.cTipoPension||'-J'||tp.cDesTipoPension;

					SELECT descripcion,tipomovimiento INTO tp.cDesTipoRetiro,tp.iIdMovimiento FROM catretiros WHERE tiporetiro='J';
					tp.cDesTipoRetiro='J-'||tp.cDesTipoRetiro;

					SELECT descripcion INTO tp.cDesTipoSeguro FROM cattiposeguroimss  WHERE tiposeguro=tp.cTipoSeguro;
					tp.cDesTipoSeguro=tp.cTipoSeguro||'-'||tp.cDesTipoSeguro;

					SELECT descripcion INTO tp.cDesTipoPresta FROM cattipoprestacionimss WHERE tipoprestacion=tp.sTipoPrestacion;
					tp.cDesTipoPresta=tp.sTipoPrestacion||'-'||tp.cDesTipoPresta;

					SELECT descripcion INTO tp.cDescEdoSubviv FROM catestadosubvivrt WHERE estadosubviv=tp.sEstadoSubViv::CHARACTER;
					tp.cDescEdoSubviv=tp.sEstadoSubViv::CHARACTER||'-'||tp.cDescEdoSubviv;

				END IF;

			END IF;

			IF tp.cTipoPension IS NULL OR tp.cTipoPension ='' THEN
				iNoCorrecto=1;
			END IF;

			IF tp.dFechaIni IS NULL THEN
				iNoCorrecto=1;
			END IF;

			IF iNoCorrecto > 0 THEN
				tp.iExisteresolucion=0;
			END IF;

		END IF;

		IF sOpcion=4 THEN

			SELECT COUNT(*) INTO iRegafiliado FROM afop_cuota_diaria WHERE n_seguro=cNss;
			IF iRegafiliado > 0 THEN
				tp.iAfiliado=1;
			END IF;

			SELECT MAX(keyx) INTO iKeyx FROM retresolucionestotalesimss WHERE nss=cNss AND curp = cCurp;
			IF iKeyx > 0 THEN
				tp.iExisteresolucion = 1;

				IF EXISTS (SELECT regimen FROM retresolucionestotalesimss WHERE nss=cNss AND keyx=iKeyx) THEN

					SELECT regimen,tiposeguro,tipopension,tipoprestacion,nombredatamart,folio,secpension,fechainipen,fecinipago,semanascotizadas,diagdatamart,estadosubviv,fecharesolucion
					INTO tp.cRegimen,tp.cTipoSeguro,tp.cTipoPension,tp.sTipoPrestacion,tp.cNombreDatamart,tp.iFolio,tp.cSecPension,tp.dFechaIni,tp.dFechaPago,tp.sSemanasCotizadas,tp.sDiagDatamart,tp.sEstadoSubViv,tp.dFechaResol
					FROM retresolucionestotalesimss  WHERE nss = cNss AND keyx=iKeyx;

					SELECT descripcion INTO tp.cDesRegimen FROM catregimenimss WHERE regimen=tp.cRegimen;
					tp.cDesRegimen= tp.cRegimen||'-'||tp.cDesRegimen;

					SELECT descripcion INTO tp.cDesTipoPension FROM cattipopensionimss WHERE tipopension=tp.cTipoPension;
					tp.cDesTipoPension= tp.cTipoPension||'-F'||tp.cDesTipoPension;

					SELECT descripcion,tipomovimiento INTO tp.cDesTipoRetiro,tp.iIdMovimiento FROM catretiros WHERE tiporetiro='F';
					tp.cDesTipoRetiro='F-'||tp.cDesTipoRetiro;

					SELECT descripcion INTO tp.cDesTipoSeguro FROM cattiposeguroimss  WHERE tiposeguro=tp.cTipoSeguro;
					tp.cDesTipoSeguro=tp.cTipoSeguro||'-'||tp.cDesTipoSeguro;

					SELECT descripcion INTO tp.cDesTipoPresta FROM cattipoprestacionimss WHERE tipoprestacion=tp.sTipoPrestacion;
					tp.cDesTipoPresta=tp.sTipoPrestacion||'-'||tp.cDesTipoPresta;

					SELECT descripcion INTO tp.cDescEdoSubviv FROM catestadosubvivrt WHERE estadosubviv=tp.sEstadoSubViv::CHARACTER;
					tp.cDescEdoSubviv=tp.sEstadoSubViv::CHARACTER||'-'||tp.cDescEdoSubviv;

				END IF;

			END IF;

		END IF;

		IF sOpcion=5 THEN
		
			SELECT COUNT(*) INTO iRegafiliado FROM afop_cuota_diaria WHERE n_seguro=cNss;
			IF iRegafiliado > 0 THEN
				tp.iAfiliado=1;	
			END IF;

			SELECT MAX(keyx) INTO iKeyx FROM retresolucionestotalesimss WHERE nss=cNss AND curp = cCurp AND tiposeguro IN ('CV','IM', 'RT') AND diagdatamart IN (101,300,301,501) AND tipopension IN ('RA' ,'IP', 'OR', 'AS', 'IN', 'VE', 'VI', 'VO', 'CE') AND regimen = '97';
			IF iKeyx > 0 THEN
				tp.iExisteresolucion = 1;

				IF EXISTS (SELECT regimen FROM retresolucionestotalesimss WHERE nss=cNss AND keyx=iKeyx) THEN

					SELECT regimen,tiposeguro,tipopension,tipoprestacion,nombredatamart,folio,secpension,fechainipen,fecinipago,semanascotizadas,diagdatamart,estadosubviv,fecharesolucion 
					INTO tp.cRegimen,tp.cTipoSeguro,tp.cTipoPension,tp.sTipoPrestacion,tp.cNombreDatamart,tp.iFolio,tp.cSecPension,tp.dFechaIni,tp.dFechaPago,tp.sSemanasCotizadas,tp.sDiagDatamart,tp.sEstadoSubViv,tp.dFechaResol
					FROM retresolucionestotalesimss  WHERE nss = cNss AND keyx=iKeyx;

					SELECT descripcion INTO tp.cDesRegimen FROM catregimenimss WHERE regimen=tp.cRegimen;
					tp.cDesRegimen= tp.cRegimen||'-'||tp.cDesRegimen;

					SELECT descripcion INTO tp.cDesTipoPension FROM cattipopensionimss WHERE tipopension=tp.cTipoPension;
					tp.cDesTipoPension= tp.cTipoPension||'-E'||tp.cDesTipoPension;

					SELECT descripcion,tipomovimiento INTO tp.cDesTipoRetiro,tp.iIdMovimiento FROM catretiros WHERE tiporetiro='E';
					tp.cDesTipoRetiro='E-'||tp.cDesTipoRetiro;
					
					SELECT descripcion INTO tp.cDesTipoSeguro FROM cattiposeguroimss  WHERE tiposeguro=tp.cTipoSeguro;
					tp.cDesTipoSeguro=tp.cTipoSeguro||'-'||tp.cDesTipoSeguro;

					SELECT descripcion INTO tp.cDesTipoPresta FROM cattipoprestacionimss WHERE tipoprestacion=tp.sTipoPrestacion;
					tp.cDesTipoPresta=tp.sTipoPrestacion||'-'||tp.cDesTipoPresta;

					SELECT descripcion INTO tp.cDescEdoSubviv FROM catestadosubvivrt WHERE estadosubviv=tp.sEstadoSubViv::CHARACTER;
					tp.cDescEdoSubviv=tp.sEstadoSubViv::CHARACTER||'-'||tp.cDescEdoSubviv;

				END IF;

			END IF;

		END IF;

		RETURN NEXT tp;
		
	END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 100;
ALTER FUNCTION fnvalidadatamartimssnuevos(smallint, character, character)
  OWNER TO sysserviciosafore;
