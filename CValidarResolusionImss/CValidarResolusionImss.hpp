#ifndef CVALIDARRESOLUSIONIMSS_HPP_
#define CVALIDARRESOLUSIONIMSS_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CFnValidarResolucionImss.hpp"

class CValidarResolusionImss
{
private:
	char cTexto[1024],
		cOutTexto[768],
		cIpServAfo[17],
		cIpInfx[100],
		cRutaLog[128],
		cSql[256],
		cBuff[SIZE_BUFF_DAT];
	C_ODBC odbcPg,
		odbcIfx;
	VAL_RESOLUCION_IMSS stValResImss;
	short validarResolucionBd(char *cNss, int iTipoRetirox);

public:
	CValidarResolusionImss();
	~CValidarResolusionImss();
	short ValidarResolucionImss(char *cNssx, int iTipoRetirox);
	VAL_RESOLUCION_IMSS *ResolucionImss();
};

#endif
