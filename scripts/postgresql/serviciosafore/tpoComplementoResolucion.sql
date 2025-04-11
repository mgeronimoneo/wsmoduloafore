create type tpoComplementoResolucion as
(
	numeroResolucion int,
	idcomplemento int,
	montopagado double precision,
	montopagadocomplemento double precision,
	saldorcv double precision,
	saldorcvanterior double precision,
	respuestaotrodatamart smallint
);
alter type tpoComplementoResolucion owner to sysserviciosafore;