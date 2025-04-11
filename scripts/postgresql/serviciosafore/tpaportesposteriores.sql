-- Type: tpAportesPosteriores

-- DROP TYPE tpAportesPosteriores;

CREATE TYPE tpAportesPosteriores AS
   (iSieforeRec SMALLINT,
    iSubCuenta SMALLINT,
    dSumaAcciones DOUBLE PRECISION);
    
ALTER TYPE tpAportesPosteriores
  OWNER TO sysserviciosafore;