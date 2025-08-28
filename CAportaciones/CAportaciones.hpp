#ifndef CAPORTACIONES_HPP_
#define CAPORTACIONES_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CFnAportaciones.hpp"
#include <string>

class CAportaciones
{
private:
	char cTexto[1024],
		cOutTexto[768],
		cIpServAfo[17],
		cIpAdmon[17],
		cIpInfx[100],
		cRutaLog[128],
		cSql[256],
		cBuff[SIZE_BUFF_DAT];
	double iAportaciones;
	APORTACION_POSTERIORES strAportaciones;
	C_ODBC odbcPg,
		odbcIfx;

public:
	CAportaciones();
	~CAportaciones();
	short ObtenerAportaciones(char *sNss, char *dFechaIniPension, int opcion);
	std::string subString(const std::string &cadena, int comienzo, int longitud);
	APORTACION_POSTERIORES *Aportaciones();
	short abrirConexionServAfo();
};

#endif
