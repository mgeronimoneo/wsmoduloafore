create or replace function fnObtenerMontosFechasDatamart(cNss char(11), iTipoRetiro int)
returns tpoMontosFechasDatamart as
$BODY$
	declare
		iTipoPrestacion int;
		regSaldoFechas tpoMontosFechasDatamart;
		regSaldo record;
	begin
		iTipoPrestacion=0;
		regSaldoFechas.nss='';
		regSaldoFechas.salarioBaseA=0;
		regSaldoFechas.salarioBaseB=0;
		regSaldoFechas.FechaMatrimonioDesempleo='19000101';
		regSaldoFechas.FechaInicioVigencia='19000101';
		regSaldoFechas.ultimoSalarioBaseCot=0;
		regSaldoFechas.Ultimas250SemanasSBC=0;
		
		if iTipoRetiro = 1024 then --desempleo
			iTipoPrestacion=6;
		elsif iTipoRetiro = 1025 then -- matrimonio
			iTipoPrestacion=7;
		end if;

		SELECT nss, salariobasea, salariobaseb, fechamatrimoniodesempleo, fechainiciovigencia into regSaldo
		FROM retresolucionimss WHERE nss = cNss AND tipoprestacion = iTipoPrestacion  AND salariobasea >0 AND salariobasea >0
		ORDER BY fechaFinVigencia DESC limit 1;
		if regSaldo.salariobasea >0 then
			regSaldoFechas.ultimoSalarioBaseCot=regSaldo.salariobasea;
		end if;

		if regSaldo.salariobaseb >0 then
			regSaldoFechas.Ultimas250SemanasSBC=regSaldo.salariobaseb;
		end if;
		regSaldoFechas.nss=regSaldo.nss;
		regSaldoFechas.salarioBaseA=regSaldo.salariobasea;
		regSaldoFechas.salarioBaseB=regSaldo.salariobaseb;
		regSaldoFechas.FechaMatrimonioDesempleo=regSaldo.fechamatrimoniodesempleo;
		regSaldoFechas.FechaInicioVigencia=regSaldo.fechainiciovigencia;
		return regSaldoFechas;
	end;	
$BODY$
LANGUAGE plpgsql VOLATILE security definer;