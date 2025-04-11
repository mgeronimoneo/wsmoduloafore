create table marcaoperativaactual
(
	keyx			serial not null,
	fechaalta		date not null default now(),
	nss 			char(11) NOT NULL DEFAULT '',
	codigoMarca		smallint NOT NULL DEFAULT 0,
	fechainicio 	timestamp not null default timenow(),
	estadomarca 	smallint NOT NULL DEFAULT 0,
	marcacausa 		smallint NOT NULL DEFAULT 0,
	fechacausa 		date NOT NULL DEFAULT '19000101',
	correlativo 	integer NOT NULL DEFAULT 0,
	constraint pk_marcaoperativaactual_nss_marca primary key(nss,codigomarca)
)
without oids;
alter table marcaoperativaactual owner to sysserviciosafore;