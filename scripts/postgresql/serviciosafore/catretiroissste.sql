create table catretiroissste
(
	keyx			serial not null,
	fechaalta		date not null default now(),
	TipoRetiro		char(1) not null default '',
	CodigoTramite	smallint not null default 0,
	tipoMovimiento	smallint not null default 0,
	descripcion		char(100) not null default '',
	constraint pk_catretiroissste_Retiro_tramite_movto primary key(TipoRetiro,CodigoTramite,tipoMovimiento)
)
without oids;
alter table catretiroissste owner to sysserviciosafore;