--DROP TABLE retirosParcialesIssste
CREATE TABLE retirosParcialesIssste
(
	keyx 				serial not null,
	fechaalta 			timestamp without time zone not null default timenow(),
	folio				int not null default 0,
	consecutivo			bigint not null default 0,
	curp				character(18) not null default '',
	secpension			character(2) not null default '',
	rfc					character(13) not null default '',
	nss					character(11) not null default '',
	numissste			character(11) not null default '',
	foliosolicitud		int not null default 0,
	nombre				character(40) not null default '',
	apellidopaterno		character(40) not null default '',
	apellidomaterno		character(40) not null default '',
	numconcesion		character(6) not null default '',
	regimen				character(2) not null default '',
	tiposeguro			character(2) not null default '',
	grupo				smallint not null default 0,
	estadcod			int not null default 0,
	delegcod			int not null default 0,
	diagprocesar		character(3) not null default '',
	diagop56			character(3) not null default '',
	codigorechazo		smallint not null default 0,
	fechasolicitud		date not null default '19000101',
	usuariocaptura		character(15) not null default '',
	fechacaptura		date not null default '19000101',
	usuariomodifica		character(15) not null default '',
	fechamodifica		date not null default '19000101',
	usuarioconfirma		character(15) not null default '',
	fechaconfirma		date not null default '19000101',
	estadosolicitud		smallint not null default 0,
	CONSTRAINT retirosParcialesIssste_pkey PRIMARY KEY (foliosolicitud)
)WITHOUT OIDS;
ALTER TABLE retirosParcialesIssste OWNER TO sysserviciosafore;
ALTER SEQUENCE retirosParcialesIssste_keyx_seq OWNER TO sysserviciosafore;

CREATE INDEX indretirosParcialesIssste01
ON retirosParcialesIssste
USING btree
  (foliosolicitud);
  
CREATE INDEX indretirosParcialesIssste02
ON retirosParcialesIssste
USING btree
  (curp);
  
CREATE INDEX indretirosParcialesIssste03
ON retirosParcialesIssste
USING btree
  (nss);
  
CREATE INDEX indretirosParcialesIssste04
ON retirosParcialesIssste
USING btree
  (folio, consecutivo);