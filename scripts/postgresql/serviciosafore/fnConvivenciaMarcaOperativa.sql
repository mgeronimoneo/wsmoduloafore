create or replace function fnConvivenciaMarcaOperativa(iOpcion SMALLINT, cNss CHAR(11), iTipoTrabajador smallint , cTipoRetiro CHAR(2))
returns setof tpoconvivenciamarcas as 
$body$
	declare
		iMarcaActiva smallint;
		iTipoMovto	 smallint;
		iContador	 integer;
		regResp 	tpoconvivenciamarcas;
	begin
		iMarcaActiva=0;
		iContador=0;
		regResp.Codigo=0;
		regResp.descripcion='';
		regResp.tienemarca=0;

		--Validamos si hay convivencia de las marcas o si hay marca en proceso
		if iTipoTrabajador=2 then --ISSSTE
			select tipomovimiento into iTipoMovto from catretiroissste where tipoRetiro=cTipoRetiro;
			select count(*)::int into iContador
			from marcaoperativaactual as ax
			join convivenciamarcaoperativa as bx on(ax.codigoMarca=bx.MarcaActiva)
			where ax.nss=cNss and bx.marcaEntra=iTipoMovto and bx.CodigoRechazo!=0;
			if iContador>0 then
				regResp.Codigo=4;
				regResp.descripcion='LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO';
			end if;
		elsif iTipoTrabajador=1 then -- imss
			if cTipoRetiro='6' then --RETIRO POR DESEMPLEO
				iTipoMovto=875;
			elsif cTipoRetiro='7' then --RETIRO POR MATRIMONIO
				iTipoMovto=870;
			ELSIF cTipoRetiro = 'MD' THEN --MODIFICACION DE DATOS
				iTipoMovto = 600;
			else
				select tipomovimiento into iTipoMovto from catretiros where tipoRetiro=cTipoRetiro;
			end if;
			select count(*)::int into iContador
			from marcaoperativaactual as ax
			join convivenciamarcaoperativa as bx on(ax.codigoMarca=bx.MarcaActiva)
			where ax.nss=cNss and bx.marcaEntra=iTipoMovto and bx.CodigoRechazo!=0;
			if iContador>0 then
				if exists(select bx.CodigoRechazo from marcaoperativaactual as ax join convivenciamarcaoperativa as bx on(ax.codigoMarca=bx.MarcaActiva)
							where ax.nss=cNss and bx.marcaEntra=iTipoMovto and bx.CodigoRechazo=819 ) then
					regResp.Codigo=819;
				else
					regResp.Codigo=4;
				end if;
				regResp.descripcion='LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO';
			end if;
		end if;

		--Validamos la marca dependiendo de la opcion
		if iOpcion = 1 THEN --Es opcion de la marca separacion de cuentas
			--Verificamos si tiene la marca de Separacion de Cuentas 280
			SELECT codigomarca INTO iMarcaActiva FROM marcaoperativaactual WHERE nss = cNss AND codigomarca = 280;
			IF iMarcaActiva IS NOT NULL AND iMarcaActiva <> 0 THEN--Si tiene marca
				regResp.tienemarca = 1;
			ELSE --no tiene marca 
				regResp.tienemarca = 0;
			END IF;
		END IF;
		return next regResp;
	end;
$body$
  LANGUAGE plpgsql VOLATILE;
alter function  fnConvivenciaMarcaOperativa(SMALLINT, CHAR(11), smallint , CHAR(1)) owner to sysserviciosafore;