-- Type: tpvalidardatamartimssrt

-- DROP TYPE tpvalidardatamartimssrt;

CREATE TYPE tpvalidardatamartimssrt AS
   (iexisteresolucion smallint,
    iafiliado integer,
    cregimen character(2),
    ctiposeguro character(2),
    ctipopension character(2),
    stipoprestacion smallint,
    cnombredatamart character(50),
    ifolio integer,
    csecpension character(2),
    dfechaini date,
    dfechapago date,
    ssemanascotizadas smallint,
    sdiagdatamart smallint,
    sestadosubviv smallint,
    dfecharesol date,
    cdesregimen character(40),
    cdestipopension character(40),
    cdestipopresta character(40),
    cdestiporetiro character(40),
    cdestiposeguro character(40),
    cdescedosubviv character(40),
    iidmovimiento smallint);
ALTER TYPE tpvalidardatamartimssrt
  OWNER TO sysserviciosafore;
