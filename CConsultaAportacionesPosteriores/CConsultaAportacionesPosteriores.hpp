#ifndef CCONSULTAAPORTACIONESPOSTERIORES_HPP_
#define CCONSULTAAPORTACIONESPOSTERIORES_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CFnConsultarAportacionesPosteriores.hpp"

class CConsultaAportacionesPosteriores
{
	private:
		char	cTexto[1024],
				cOutTexto[768],
				cIpServAfo[17],
				cIpAdmon[17],
				cIpInfx[100],
				cRutaLog[128],
				cSql[256],
				cBuff[SIZE_BUFF_DAT];
		C_ODBC	odbcPg,
				odbcIfx;
		CONSULTA_APORTACIONES_POSTERIORES	stAportacionesPosteriores;		
		
	public:
		CConsultaAportacionesPosteriores();
		~CConsultaAportacionesPosteriores();
		CONSULTA_APORTACIONES_POSTERIORES* AportacionesPosteriores();
		int CConsultarAportacionesPosteriores(short shOpcion, char *cNssx, char *cFechaIniPension, short shGrupo);
		int CConsultarAportacionesPosterioresBD(short shOpcion, char *cNssx, char *cFechaIniPension, short shGrupo);
		int abrirConexionServAfo();
};

#endif