#ifndef CSALDOSPORSUBCUENTA_HPP_
#define CSALDOSPORSUBCUENTA_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clases/funcionescomparar.hpp"
#include "../clasessql/CSaldoSubCuenta.hpp"

class CSaldosPorSubCuenta
{
private:
	char	cTexto[1024],
			cOutTexto[768],
			cIpAdmon[17],
			cIpServAfo[17],
			cIpInfx[100],
			cRutaLog[128],
			cSql[256],
			cBuff[SIZE_BUFF_DAT],
			cNss[SIZE_NSS+2];
	SHM_INFOX	stInfShmSaldoDiario;
	SALDO_CUENTA	*shmSaldoDiario,
					stSaldoDiario;
	//CBaseDato	objBd;
	C_ODBC		odbcPg,
				odbcIfx;

	short buscarSaldoDiario();
	short ConsultarSaldoBD();
	short ConsultarSaldoBdPMG();
	short abrirConexionServAfo();
public:
	CSaldosPorSubCuenta();
	~CSaldosPorSubCuenta();
	short consultarSaldoPorSubcuenta(char *cNssx);
	short consultarSaldoPorSubcuentaPMG(char *cNssx);
	SALDO_CUENTA* SaldoPorSubcuenta();
};

#endif
