create or replace function fnFoliadorServicios(cTipoServicio CHAR(1))
returns int as 
$body$
	declare
		iFolio	INT8;
	begin
		iFolio=0;
		LOCK TABLE FoliadorServicios IN SHARE UPDATE EXCLUSIVE MODE;
		select folio into iFolio from  FoliadorServicios where tipoServicio=cTipoServicio;
		if iFolio>0 and iFolio is not null then
			iFolio = iFolio+1;
			update FoliadorServicios set folio= iFolio, fechacambio=timenow() where tipoServicio=cTipoServicio;
		else
			iFolio=0;
		end if;
		return iFolio;
	end;
$body$
  LANGUAGE plpgsql VOLATILE;
alter function fnFoliadorServicios(char(1)) owner to sysserviciosafore;