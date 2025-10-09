#ifndef CCONSULTARRESOLUSION_HPP_
#define CCONSULTARRESOLUSION_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CFnConsultarResolucion.hpp"
class CConsultarResolusion
{
	private:
		char	cTexto[1024],
				cOutTexto[768],
				cIpServAfo[17],
				cIpInfx[100],
				cRutaLog[128],
				cSql[356],
				cBuff[SIZE_BUFF_DAT];
		C_ODBC	odbcPg,
				odbcIfx;
		CONSULTA_RESOLUSION	stResolusion;
		short ConsultarResolusionBD(char *cNssx, int iTipoRetirox);
	public:
		CConsultarResolusion();
		~CConsultarResolusion();
		short ConsultarResolusion(char *cNssx, int iTipoRetirox);
		CONSULTA_RESOLUSION* Resolucion();
};

#endif
