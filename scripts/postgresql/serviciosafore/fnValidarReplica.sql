create or replace function fnValidarReplica()
returns int as 
$body$
	declare
		iRetorno int;
	begin
		iRetorno=0;
		select count(*) into iRetorno from controlwsmoduloafore where idcontrol between 100 and 199 and trim(valor)::date!= current_date;
		if iRetorno >0 then
			iRetorno=1; --no esta completa la replica
		else
			iRetorno=0; --replica completa, proceder a utlizar tablas
		end if;
		return iRetorno;
	end;
$body$
  LANGUAGE plpgsql VOLATILE;
alter function fnValidarReplica() owner to sysserviciosafore;