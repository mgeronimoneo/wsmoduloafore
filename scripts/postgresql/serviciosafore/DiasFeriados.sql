create table DiasFeriados
(
	keyx					serial not null,
	fechaalta				date not null default now(),
	fechaFeriada			date not null default '19000101',
	descripcion				char(50) not null default ''
)without oids;
alter table DiasFeriados owner to sysserviciosafore;