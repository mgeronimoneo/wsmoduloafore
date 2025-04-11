-- Type: tpAfopObtieneApPosteriores

-- DROP TYPE tpAfopObtieneApPosteriores;

CREATE TYPE tpAfopObtieneApPosteriores AS
   (cMontoAportacionPosterior CHARACTER(25),
    cMensaje CHARACTER(200));
    
ALTER TYPE tpAfopObtieneApPosteriores
  OWNER TO sysserviciosafore;