create table FoliadorServicios
(
	keyx			serial not null,
	fechaalta		date not null default now(),
	folio			int8 not null default 0,
	TipoServicio 	char(1) not null default '',
	fechaCambio		timestamp not null default '19000101 00:00:00'
)
without oids;
alter table FoliadorServicios owner to sysserviciosafore;