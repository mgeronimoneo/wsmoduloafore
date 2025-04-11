create table convivenciamarcaoperativa
(
	keyx			serial not null,
	fechaalta		date not null default now(),
	MarcaEntra		smallint not null default 0,
	MarcaActiva		smallint not null default 0,
	CodigoConvive	smallint not null default 0,
	codigoRechazo	smallint not null default 0,
	FechaCambio		timestamp not null default '19000101 00:00:00'
)
without oids;
alter table convivenciamarcaoperativa owner to sysserviciosafore;