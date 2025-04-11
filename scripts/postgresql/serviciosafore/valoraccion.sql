create table valoraccion
(
	keyx			serial not null,
	fechaalta		date not null default now(),
	siefore			smallint not null default 0,
	valoraccion		double precision not null default 0,
	fechaValor		date not null default '19000101',
	fechaOperacion	date not null default '19000101',
	constraint pk_valoraccion_siefore_fechavalor primary key(siefore,fechavalor)
)without oids;
alter table catretiros owner to sysserviciosafore;