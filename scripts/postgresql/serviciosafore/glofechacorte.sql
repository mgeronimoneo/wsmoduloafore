-- Table: gloFechaCorte

DROP TABLE gloFechaCorte;

CREATE TABLE gloFechaCorte
(
	keyx serial NOT NULL,
	fechaalta timestamp without time zone NOT NULL DEFAULT now(),
	corte smallint,
	fecha_corte date
)
WITH (
  OIDS=FALSE
);
ALTER TABLE gloFechaCorte
  OWNER TO sysserviciosafore;