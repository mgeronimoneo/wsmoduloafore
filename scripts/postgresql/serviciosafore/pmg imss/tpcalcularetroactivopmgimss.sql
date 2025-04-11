-- DROP TYPE tpcalcularetroactivopmgimss;

CREATE TYPE tpcalcularetroactivopmgimss AS
   (retroactivo double precision,
    devengado double precision,
    estatus integer);
ALTER TYPE tpcalcularetroactivopmgimss
  OWNER TO sysserviciosafore;