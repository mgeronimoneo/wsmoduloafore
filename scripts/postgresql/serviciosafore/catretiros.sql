create table catretiros
(
	keyx			serial not null,
	fechaalta		date not null default now(),
	TipoRetiro		char(1) not null default '',
	tipoMovimiento	smallint not null default 0,
	descripcion		char(100) not null default '',
	constraint pk_catretiro_Retiro_movto primary key(TipoRetiro,tipoMovimiento)
)
without oids;
alter table catretiros owner to sysserviciosafore;