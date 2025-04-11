-- Function: fnvalidardatamartimssrt(integer, character, character, smallint)

-- DROP FUNCTION fnvalidardatamartimssrt(integer, character, character, smallint);

CREATE OR REPLACE FUNCTION fnvalidardatamartimssrt(
    iopcion integer,
    cnss character,
    ccurp character,
    stipodocumento smallint)
  RETURNS SETOF tpvalidardatamartimssrt AS
$BODY$
DECLARE

iKeyx				INTEGER;

datamart			tpvalidardatamartimssrt;
codigo_rechazo			SMALLINT;

BEGIN
	codigo_rechazo			=0;

	datamart.iexisteresolucion	=0;
	datamart.iafiliado		=0;
	datamart.cregimen		='';
	datamart.ctiposeguro 		='';
	datamart.ctipopension 		='';
	datamart.stipoprestacion 	=0;
	datamart.cnombredatamart 	='';
	datamart.ifolio 		=0;
	datamart.csecpension 		='';
	datamart.dfechaini 		='1990-01-01';
	datamart.dfechapago 		='1990-01-01';
	datamart.ssemanascotizadas 	=0;
	datamart.sdiagdatamart 		=0;
	datamart.sestadosubviv 		=0;
	datamart.dfecharesol 		='1990-01-01';
	datamart.cdesregimen 		='';
	datamart.cdestipopension 	='';
	datamart.cdestipopresta 	='';
	datamart.cdestiporetiro 	='';
	datamart.cdestiposeguro 	='';
	datamart.cdescedosubviv 	='';
	datamart.iidmovimiento 		=0;


	SELECT a.regimen,a.tipoprestacion
	INTO   datamart.cregimen,datamart.stipoprestacion
	FROM catrettipodocumento AS a
	INNER JOIN cattipoprestacionimss AS b on (a.tipoprestacion = b.tipoprestacion) WHERE tipodocumento=sTipodocumento;

	IF EXISTS (SELECT * FROM retresolucionestotalesimss WHERE nss=cnss) THEN
		datamart.iexisteresolucion = 1;
	END IF;
	IF EXISTS (SELECT * FROM afop_cuota_diaria WHERE nss=cnss) THEN
		datamart.iafiliado = 1 ;
	END IF;

IF iOpcion = 1 THEN  --SE TRATA DE TIPO RETIRO E
	IF EXISTS (SELECT * FROM retresolucionestotalesimss WHERE nss=cNss AND curp=cCurp AND regimen=datamart.cregimen AND tiposeguro in ('IV','RT')
			AND tipopension in('AS','OR','VI','VO','IN','CE','VE','IP') AND tipoprestacion=datamart.stipoprestacion AND diagdatamart = 101 )THEN
			SELECT MAX(keyx)
			INTO iKeyx
			FROM retresolucionestotalesimss
			WHERE nss=cNss 	AND regimen=datamart.cregimen AND tiposeguro in ('IV','RT') AND
			tipopension in('AS','OR','VI','VO','IN','CE','VE','IP') AND tipoprestacion=datamart.stipoprestacion AND diagdatamart = 101;

			SELECT res.diagdatamart,res.secpension,res.fechainipen,res.fecharesolucion,res.nombredatamart,res.tipomovimiento,res.tiporetiro,res.regimen,
			reg.descripcion,res.tiposeguro,seg.descripcion,res.tipopension,pens.descripcion,
			res.tipoprestacion,pres.descripcion,res.semanascotizadas

			INTO datamart.sdiagdatamart,datamart.csecpension,datamart.dfechaini,datamart.dfecharesol,datamart.cnombredatamart,
			datamart.iidmovimiento,datamart.cdestiporetiro,datamart.cregimen,datamart.cdesregimen,datamart.ctiposeguro,datamart.cdestiposeguro,
			datamart.ctipopension,datamart.cdestipopension,datamart.stipoprestacion,datamart.cdestipopresta,
			datamart.ssemanascotizadas

			FROM retresolucionestotalesimss AS res
			INNER JOIN catregimenimss AS reg on (reg.regimen = res.regimen)
			INNER JOIN cattiposeguroimss AS seg on (seg.tiposeguro = res.tiposeguro)
			INNER JOIN cattipopensionimss AS pens on (pens.tipopension = res.tipopension)
			INNER JOIN cattipoprestacionimss AS pres on (pres.tipoprestacion = res.tipoprestacion)
			WHERE res.nss=cNss AND res.regimen=datamart.cregimen AND res.tiposeguro in ('IV','RT')
			AND res.tipopension in('AS','OR','VI','VO','IN','CE','VE','IP') AND res.tipoprestacion=datamart.stipoprestacion
			AND res.diagdatamart=101 AND res.folio >= 0 AND res.keyx=iKeyx;

			codigo_rechazo=0;

			ELSE

			 codigo_rechazo=1010;
			 datamart.sdiagdatamart = 0;
		         datamart.csecpension = '';
		       --datamart.dfechaini = '1900-01-01';
		       --datamart.dfecharesol = '1900-01-01';
		         datamart.cnombredatamart = '';
		         datamart.iidmovimiento = 0;
		         datamart.cdestiporetiro = '';
		         datamart.cregimen = '';
		         datamart.cdesregimen ='';
		         datamart.ctiposeguro ='';
		         datamart.cdestiposeguro ='';
		         datamart.ctipopension ='';
		         datamart.cdestipopension ='';
		         datamart.stipoprestacion = 0;
		         datamart.cdestipopresta ='';
		         datamart.ssemanascotizadas = 0;
	END IF;
	ELSE IF iOpcion = 2 THEN  --SE TRATA DE TIPO RETIRO D
		IF EXISTS (SELECT * FROM retresolucionestotalesimss WHERE nss=cNss AND curp=cCurp AND regimen=datamart.cregimen AND tiposeguro in ('IM','CV') AND  tipopension in('AS','OR','VI','VO','IN','CE','VE','IP') AND tipoprestacion=datamart.stipoprestacion AND diagdatamart = 101 )THEN

			SELECT MAX(keyx)
			INTO iKeyx
			FROM retresolucionestotalesimss
			WHERE nss=cNss 	AND regimen=datamart.cregimen AND tiposeguro in ('IM','CV') AND
			tipopension in('AS','OR','VI','VO','IN','CE','VE','IP') AND tipoprestacion=datamart.stipoprestacion AND diagdatamart = 101;

			SELECT res.diagdatamart,res.secpension,res.fechainipen,res.fecharesolucion,res.nombredatamart,res.tipomovimiento,res.tiporetiro,res.regimen,
			reg.descripcion,res.tiposeguro,seg.descripcion,res.tipopension,pens.descripcion,
			res.tipoprestacion,pres.descripcion,res.semanascotizadas

			INTO datamart.sdiagdatamart,datamart.csecpension,datamart.dfechaini,datamart.dfecharesol,datamart.cnombredatamart,
			datamart.iidmovimiento,datamart.cdestiporetiro,datamart.cregimen,datamart.cdesregimen,datamart.ctiposeguro,datamart.cdestiposeguro,
			datamart.ctipopension,datamart.cdestipopension,datamart.stipoprestacion,datamart.cdestipopresta,
			datamart.ssemanascotizadas

			FROM retresolucionestotalesimss AS res
			INNER JOIN catregimenimss AS reg on (reg.regimen = res.regimen)
			INNER JOIN cattiposeguroimss AS seg on (seg.tiposeguro = res.tiposeguro)
			INNER JOIN cattipopensionimss AS pens on (pens.tipopension = res.tipopension)
			INNER JOIN cattipoprestacionimss AS pres on (pres.tipoprestacion = res.tipoprestacion)
			WHERE res.nss=cNss AND res.regimen=datamart.cregimen AND res.tiposeguro in ('IM','CV')
			AND res.tipopension in('AS','OR','VI','VO','IN','CE','VE','IP') AND res.tipoprestacion=datamart.stipoprestacion
			AND res.diagdatamart=101 AND res.folio >= 0 AND res.keyx=iKeyx;

			codigo_rechazo=0;

			ELSE

			 codigo_rechazo=1010;
			 datamart.sdiagdatamart = 0;
		         datamart.csecpension = '';
		       --datamart.dfechaini = '1900-01-01';
		       --datamart.dfecharesol = '1900-01-01';
		         datamart.cnombredatamart = '';
		         datamart.iidmovimiento = 0;
		         datamart.cdestiporetiro = '';
		         datamart.cregimen = '';
		         datamart.cdesregimen ='';
		         datamart.ctiposeguro ='';
		         datamart.cdestiposeguro ='';
		         datamart.ctipopension ='';
		         datamart.cdestipopension='';
		         datamart.stipoprestacion = 0;
		         datamart.cdestipopresta ='';
		         datamart.ssemanascotizadas = 0;
		END IF;
	END IF;
END IF;

	RETURN NEXT  datamart;


END;


$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;
ALTER FUNCTION fnvalidardatamartimssrt(integer, character, character, smallint)
  OWNER TO sysserviciosafore;
