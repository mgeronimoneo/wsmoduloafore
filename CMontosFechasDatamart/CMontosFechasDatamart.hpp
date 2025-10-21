#ifndef CMONTOSFECHASDATAMART_HPP_
#define CMONTOSFECHASDATAMART_HPP_
#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CConsMontosFechasDatamart.hpp"

class CMontosFechasDatamart
{
private:
	char cTexto[1024],
		cOutTexto[768],
		cIpServAfo[17],
		cIpInfx[100],
		cRutaLog[128],
		cSql[1024],
		cBuff[SIZE_BUFF_DAT];
	C_ODBC odbcPg,
		odbcIfx;
	MONTOS_FECHAS_DATAMART stMontos;
	short MontosFechasDatamartBD(char *cNssx, int iTipoRetirox);

public:
	CMontosFechasDatamart();
	~CMontosFechasDatamart();
	short MontosFechasDatamart(char *cNssx, int iTipoRetirox);
	MONTOS_FECHAS_DATAMART *MontosDatamart();
};

#endif
