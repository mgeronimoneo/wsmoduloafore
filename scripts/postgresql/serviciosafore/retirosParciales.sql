create table retirosParciales
(
	keyx 					serial not null,
	fechaalta 				timestamp without time zone not null default timenow(),
	folioTramiteProcesar	char(50) not null default '',
	nss						char(11) not null default '',
	consecutivo				int8 not null default 0,
	folio					int	 not null default 0,
	foliosolicitud			int	 not null default 0,
	idtipo					char(1) not null default '',
	curp					char(18) not null default '',
	tiporetiro				char(1) not null default '',
	tipoprestacion			smallint not null default 0,
	tipodesempleo			char(1) not null default '',
	tipopago				smallint not null default 0,
	numeroresolucion		int	not null default 0,
	salariobasecotizado		double precision not null default 0,
	fecharesolucion			date not null default '19000101',
	fechaMatrimonioDesempleo	date not null default '19000101',
	importeautorizado		double precision not null default 0,
	pagodesempleo			double precision not null default 0,
	importeretiro97			double precision not null default 0,
	importecesantia_vejez	double precision not null default 0,
	importecuotasocial		double precision not null default 0,
	importetotalsubtotalrcv	double precision not null default 0,
	diagnosticocuentaind	char(3) not null default '',
	fechasolicitud			date not null default '19000101',
	estadosolicitud			smallint not null default 0,
	entidad					smallint not null default 0,
	codigorechazoent		smallint not null default 0,
	rechazocodigo			smallint not null default 0,
	fechacaptura			date not null default '19000101',
	fechamodifica			date not null default '19000101',
	fechaconfirma			date not null default '19000101',
	fechagenera				date not null default '19000101',
	fechaenvio				date not null default '19000101',
	usuariocaptura			char(12) not null default '',
	usuariomodifica			char(12) not null default '',
	usuarioconfirma			char(12) not null default '',
	carta					smallint not null default 0,
	constraint retirosParciales_pkey PRIMARY KEY (foliosolicitud, idtipo)
)without oids;
alter table retirosParciales owner to sysserviciosafore;
alter sequence retirosParciales_keyx_seq OWNER TO sysserviciosafore;

CREATE INDEX indretirosParciales01
ON retirosParciales
USING btree
  (foliosolicitud);
  
CREATE INDEX indretirosParciales02
ON retirosParciales
USING btree
  (curp);
  
CREATE INDEX indretirosParciales03
ON retirosParciales
USING btree
  (nss);
  
CREATE INDEX indretirosParciales04
ON retirosParciales
USING btree
  (folioTramiteProcesar);
  
CREATE INDEX indretirosParciales05
ON retirosParciales
USING btree
  (folio, consecutivo);