create type tpoconvivenciamarcas as
(
	Codigo integer,
	Descripcion char(200),
	tienemarca smallint
);
alter type tpoconvivenciamarcas owner to sysserviciosafore;