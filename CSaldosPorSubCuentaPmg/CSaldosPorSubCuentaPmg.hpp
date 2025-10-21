#ifndef CSALDOSPORSUBCUENTAPMG_HPP_
#define CSALDOSPORSUBCUENTAPMG_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clases/funcionescomparar.hpp"
#include "../clasessql/CSaldoSubCuentaPmg.hpp"
#include "../clasessql/CConsultarRetroactivoPmg.hpp"
#include "../clasessql/CSaldoCuentaPmg.hpp"
#include "../COperacionesPmg/COperacionesPmg.hpp"

#define NUM_SUBCTAS_PMG 20

class CSaldosPorSubCuentaPmg
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
		cNss[SIZE_NSS + 2],
		cSecuencia[4],
		cFechaLiquida[102];
	int iSubViv;
	double dSaldoDia,
		dRetroactivo,
		dDevengado;
	SHM_INFOX stInfShmSaldoDiario;
	SALDO_CUENTA_PMG *shmSaldoDiario,
		stSaldoDiario;
	SALDOS_PMG stSaldosPmg;
	// CBaseDato	objBd;
	C_ODBC odbcPg,
		odbcIfx;

	short buscarSaldoDiario();
	short obtenerRetroactivoPgs();
	short obtenerRetroactivoInf();
	short ConsultarSaldoBD();
	short abrirConexionServAfo();

public:
	CSaldosPorSubCuentaPmg();
	~CSaldosPorSubCuentaPmg();
	short consultarSaldoPorSubcuentaPmg(char *cNssx, char *cSecuenciax, int iSubVivx);
	SALDOS_PMG *SaldoPorSubcuentaPmg();
	short obtenerFechaLiquidaPmg(char *cNssx);
	char *RespuestaFecha();
};

#endif
