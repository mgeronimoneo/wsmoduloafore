#ifndef CCONSULTAR_CTA_SALDO_VOL_HPP_
#define CCONSULTAR_CTA_SALDO_VOL_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clases/funcionescomparar.hpp"
#include "../clasessql/CConsultarSaldoVol.hpp"
class CConsultarCtaSaldoVol
{
private:
	char	cTexto[1024],
			cOutTexto[768],
			cIpAdmon[17],
			cIpServAfo[17],
			cIpInfx[100],
			cRutaLog[128],
			cSql[1024],
			cBuff[SIZE_BUFF_DAT],
			cNss[SIZE_NSS+2],
			cFechaValor[SIZE_FECHA];
	short 	shRegistros;
			
	SHM_INFOX	stInfShmSaldoVolCta;
	SALDO_VOL	*shmSalVol,
					stSalVol;
	//CBaseDato	objBd;
	C_ODBC		odbcPg,
	odbcIfx;

	short ConsultarSaldoVolBD();
	short abrirConexionServAfo();	
	short buscarCtaSaldoVolD();

public:
		CConsultarCtaSaldoVol();
		~CConsultarCtaSaldoVol();
		short consultarFechaAporte(char *cNssx);
		short registrosCtaSaldoVol();
		SALDO_VOL* RespCtaSaldoVol();
};
#endif
