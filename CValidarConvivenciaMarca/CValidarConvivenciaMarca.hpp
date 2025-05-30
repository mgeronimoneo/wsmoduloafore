#ifndef CValidarConvivenciaMarca_HPP_
#define CValidarConvivenciaMarca_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CConsultarConvivenciaMarca.hpp"

class CValidarConvivenciaMarca
{
	private:
		char	cTexto[1024],
				cOutTexto[768],
				cIpServAfo[17],
				cIpInfx[100],
				cRutaLog[128],
				cSql[256],
				cBuff[SIZE_BUFF_DAT];
		int 	iFolio;
		C_ODBC	odbcPg,
				odbcIfx;
		CONVIVENCIA_MARCAS	stConvMarca;
		short ValidarConvivenciaBd(short shOpcion, short shTipoTrabajador, char* cNss, char* cTipoRetiro);
	public:
		CValidarConvivenciaMarca();
		~CValidarConvivenciaMarca();
		short ValidarConvivencia( short shOpcion, short shTipoTrabajador, char* cNss, char* cTipoRetiro );
		CONVIVENCIA_MARCAS* ConviveMarcas();
};

#endif
