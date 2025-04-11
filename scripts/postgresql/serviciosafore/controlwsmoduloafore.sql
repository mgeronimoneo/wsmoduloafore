create table controlwsmoduloafore
(
	keyx		serial not null,
	fechaalta	date not null default now(),
	idcontrol	int not null default 0 primary key,
	descripcion	char(500) not null default '',
	valor		char(50) not null default ''
)without oids;
alter table controlwsmoduloafore owner to sysserviciosafore;