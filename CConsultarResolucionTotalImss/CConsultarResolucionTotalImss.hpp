#ifndef CCONSULTARRESOLUSIONTOTALIMSS_HPP_
#define CCONSULTARRESOLUSIONTOTALIMSS_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CFnConsultarResolucionTotalImss.hpp"

class CConsultarResolucionTotalImss
{
	private:
		char	cTexto[1024],
				cOutTexto[768],
				cIpServAfo[17],
				cIpInfx[100],
				cRutaLog[128],
				cSql[256],
				cBuff[SIZE_BUFF_DAT];
		C_ODBC	odbcPg,
				odbcIfx;
		CONSULTA_RESOLUCION_TOTAL_IMSS	stResolucionTotalImss;
		short ConsultarResolucionTotalImssBD(short shOpcion, char *cNssx, char *cCurpx);
		short ConsultarResolucionTotalImssBDNuevos(short shOpcion, char *cNssx, char *cCurpx,int idocumento);
	public:
		CConsultarResolucionTotalImss();
		~CConsultarResolucionTotalImss();
		short ConsultarResolucionTotalImss(short shOpcion, char *cNssx, char *cCurpx);
		short ConsultarResolucionTotalImssNuevos(short shOpcion, char *cNssx, char *cCurpx,int idocumento);
		CONSULTA_RESOLUCION_TOTAL_IMSS* ResolucionTotalImss();
};

#endif