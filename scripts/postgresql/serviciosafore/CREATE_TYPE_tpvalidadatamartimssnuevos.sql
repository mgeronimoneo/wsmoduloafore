-- Type: tpvalidadatamartimssnuevos

-- DROP TYPE tpvalidadatamartimssnuevos;

CREATE TYPE tpvalidadatamartimssnuevos AS
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
    cdesregimen character(50),
    cdestipopension character(50),
    cdestipopresta character(50),
    cdestiporetiro character(100),
    cdestiposeguro character(50),
    cdescedosubviv character(100),
    iidmovimiento smallint);
ALTER TYPE tpvalidadatamartimssnuevos
  OWNER TO sysserviciosafore;


