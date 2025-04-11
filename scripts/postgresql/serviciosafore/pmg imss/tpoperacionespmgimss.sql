-- DROP TYPE tpoperacionespmgimss;

CREATE TYPE tpoperacionespmgimss AS (
	Mensaje 	CHARACTER(100),
	Codigo		SMALLINT,
	MontoPmg	DOUBLE PRECISION
);
ALTER TYPE tpoperacionespmgimss OWNER TO postgres;