create table retResolucionComplemento
(
	keyx serial not null,
	fechaalta timestamp not null default timenow(),
	folio int not null default 0,
	nss	char(11) not null default '',
	montopagado double precision not null default 0,
	saldorcv double precision not null default 0,
	ultimosbc double precision not null default 0
)without oids;
alter table retResolucionComplemento owner to sysserviciosafore;
