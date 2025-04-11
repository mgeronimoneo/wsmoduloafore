-- DROP TABLE catestadosubvivrt;
CREATE TABLE catestadosubvivrt
(
  keyx serial NOT NULL,
  fechaalta timestamp without time zone NOT NULL DEFAULT now(),
  estadosubviv character(1) NOT NULL DEFAULT ''::bpchar,
  descripcion character(100) NOT NULL DEFAULT ''::bpchar,
  idaceptado smallint NOT NULL DEFAULT 0
)
WITH (
  OIDS=FALSE
);
ALTER TABLE catestadosubvivrt
  OWNER TO sysserviciosafore;
