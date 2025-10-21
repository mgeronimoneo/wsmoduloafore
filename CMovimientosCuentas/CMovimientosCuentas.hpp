#ifndef CMOVIMIENTOSCUENTAS_HPP_
#define CMOVIMIENTOSCUENTAS_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"

class CMovimientosCuentas
{
private:
	char cTexto[1024],
		cOutTexto[768],
		cIpServAfo[17],
		cIpInfx[100],
		cRutaLog[128],
		cSql[256],
		cBuff[SIZE_BUFF_DAT];
	int iMovimientos;
	C_ODBC odbcPg, odbcInformix;

public:
	CMovimientosCuentas();
	~CMovimientosCuentas();
	short ObtenerMovimientos(char *sNss);
	int movimientos();
	void obtenerIpInformix();
	void abrirConexionInformix();
};

#endif
