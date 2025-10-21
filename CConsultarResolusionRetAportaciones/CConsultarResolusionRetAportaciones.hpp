#ifndef CCONSULTARRESOLUSIONRETAPORTACIONES_HPP_
#define CCONSULTARRESOLUSIONRETAPORTACIONES_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "C_Ayuda.cpp"
#include "C_SumaRegs.cpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CFnConsultarResolucionRetAportaciones.hpp"
class CConsultarResolusionRetAportaciones
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
	int iResolusion;
	short ConsultarResolusionBD(char *cCurpx, char *cNssx);
	short obtenerTipoSolicitante(int iTipoSeguro, char *cTipoPension);

public:
	CConsultarResolusionRetAportaciones();
	~CConsultarResolusionRetAportaciones();
	short ConsultarResolusion(char *cCurpx, char *cNssx);
	int Resolucion();
};

#endif
