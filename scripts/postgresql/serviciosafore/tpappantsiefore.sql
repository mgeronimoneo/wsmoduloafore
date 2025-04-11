-- Type: tpAppAntSiefore

-- DROP TYPE tpAppAntSiefore;

CREATE TYPE tpAppAntSiefore AS
   (iSieforeRec SMALLINT,
    iSubCuenta SMALLINT,
    dSumaAcciones DOUBLE PRECISION);
    
ALTER TYPE tpAppAntSiefore
  OWNER TO sysserviciosafore;