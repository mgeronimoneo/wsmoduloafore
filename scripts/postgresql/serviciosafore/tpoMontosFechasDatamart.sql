create type tpoMontosFechasDatamart as
(
	nss char(11),	
	salarioBaseA double precision,
	salarioBaseB double precision,
	FechaMatrimonioDesempleo date,
	FechaInicioVigencia date,
	ultimoSalarioBaseCot double precision,
	Ultimas250SemanasSBC double precision
);
