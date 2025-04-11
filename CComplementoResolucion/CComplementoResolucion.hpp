#ifndef CCOMPLEMENTORESOLUCION_HPP_
#define CCOMPLEMENTORESOLUCION_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CFnConsultarComplementoResolucion.hpp"


class CComplementoResolucion
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

		COMPLEMENTO_RESOLUCION	stComplResolucion;
		short ConsultarComplementoResolusionBD(char *cNssx, int iTipoRetirox);
	public:
		CComplementoResolucion();
		~CComplementoResolucion();
		short ConsultarComplementoResolusion(char *cNssx, int iTipoRetirox);
		COMPLEMENTO_RESOLUCION* ResolucionComplemento();
};

#endif
