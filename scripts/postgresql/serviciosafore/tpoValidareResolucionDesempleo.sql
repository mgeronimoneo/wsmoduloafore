create type tpoValidareResolucionDesempleo as
(
		FechaVigencia date,
		FechaVigenciaAnt date,
		FechaProximaSolicitud date,
		Retorno int,
		Mensaje char(100),
		DiagnosticoProcesar int 
);
alter type tpoValidareResolucionDesempleo owner to sysserviciosafore;