#ifndef CFOLIADORSERVICIOS_HPP_
#define CFOLIADORSERVICIOS_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"

class CFoliadorServicios
{
private:
	char cTexto[1024],
		cOutTexto[768],
		cIpServAfo[17],
		cIpInfx[100],
		cRutaLog[128],
		cSql[256],
		cBuff[SIZE_BUFF_DAT];
	int iFolio;
	C_ODBC odbcPg;

public:
	CFoliadorServicios();
	~CFoliadorServicios();
	short ObtenerFolio(short shTipoServicio);
	int Folio();
};

#endif
