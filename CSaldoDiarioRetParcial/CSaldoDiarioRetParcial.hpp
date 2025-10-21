#ifndef CSALDODIARIORETPARCIAL_HPP_
#define CSALDODIARIORETPARCIAL_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clases/funcionescomparar.hpp"
#include "../clasessql/CConsultaDouble.hpp"

class CSaldoDiarioRetParcial
{
private:
	char cTexto[1024],
		cOutTexto[768],
		cIpAdmon[17],
		cIpServAfo[17],
		cIpInfx[100],
		cRutaLog[128],
		cSql[256],
		cBuff[SIZE_BUFF_DAT],
		cNss[SIZE_NSS + 2];
	double dSaldoDiario;
	int iTipoRetiro;
	SHM_INFOX stInfShmSaldoDiario;
	SALDO_CUENTA *shmSaldoDiario,
		stSaldoDiario;

	C_ODBC odbcPg,
		odbcIfx;

	short buscarSaldoDiario();
	short ConsultarSaldoBD();
	short abrirConexionServAfo();

public:
	CSaldoDiarioRetParcial();
	~CSaldoDiarioRetParcial();
	short obtenerSaldoDiarioRetParcial(char *cNssx, int iTipoRetirox);
	double SaldoDiario();
};

#endif
