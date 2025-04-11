-- Type: tpobtenerdatamartissste

-- DROP TYPE tpobtenerdatamartissste;

CREATE TYPE tpobtenerdatamartissste AS
   (	folio			INTEGER,
	nss			CHARACTER(11),
	nssissste		CHARACTER(11),
	curp			CHARACTER(18),
	secpension		CHARACTER(2),
	nombredatamart		CHARACTER(40),
	paternodatamart		CHARACTER(40),
	maternodatamart		CHARACTER(40),
	numconcesion		CHARACTER(9),
	delegacion		SMALLINT,
	tipomovimiento		SMALLINT,
	tiporetiro		CHARACTER(1),
	regimen			CHARACTER(2),
	tiposeguro		CHARACTER(2),
	tipopension		CHARACTER(2),
	cvepension		CHARACTER(3),
	tipoprestacion		SMALLINT,
	fechainiciopen		DATE,
	fecharesolucion		DATE,
	semanascotizadas	SMALLINT,
	diagdatamart		SMALLINT,
	estadoregistro		SMALLINT);
ALTER TYPE tpobtenerdatamartissste
  OWNER TO sysserviciosafore;
