-- Table: catdependenciasreplicabinario

-- DROP TABLE catdependenciasreplicabinario;

CREATE TABLE catdependenciasreplicabinario
(
  keyx serial NOT NULL,
  fechaalta TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT timenow(),
  idbinario INTEGER NOT NULL DEFAULT 0,
  descripcion CHARACTER(100) NOT NULL DEFAULT ''::bpchar,
  dependencias CHARACTER(100) NOT NULL DEFAULT ''::bpchar,
  CONSTRAINT pk_catdependenciasreplicabinario_idbinario PRIMARY KEY (idbinario)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE catdependenciasreplicabinario
  OWNER TO sysserviciosafore;
