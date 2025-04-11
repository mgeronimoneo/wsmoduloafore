-- Table: catagrupasubctaregimen

-- DROP TABLE catagrupasubctaregimen;

CREATE TABLE catagrupasubctaregimen
(
  keyx serial NOT NULL,
  fechaalta timestamp without time zone NOT NULL DEFAULT now(),
  gruporegimen integer NOT NULL DEFAULT 0,
  subcuenta integer NOT NULL DEFAULT 0,
  usuario character(10) DEFAULT ''::bpchar,
  fechaactualiza date DEFAULT '1900-01-01'::date,
  CONSTRAINT catagrupasubctaregimen_pkey PRIMARY KEY (gruporegimen, subcuenta)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE catagrupasubctaregimen
  OWNER TO sysserviciosafore;
