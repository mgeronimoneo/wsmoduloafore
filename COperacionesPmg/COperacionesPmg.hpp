#ifndef COPERACIONESPMG_HPP_
#define COPERACIONESPMG_HPP_

#include "../includeStd.hpp"
#include "C_ODBC.hpp"
#include "CUtileriasAfo.hpp"
#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/COperacionesPmgImss.hpp"
#include "../clasessql/CConsultaInt.hpp"

class COperacionesPmg
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
		cSecuencia[4];

	OPERACIONES_PMG_IMSS OperacionesPmgImss;
	C_ODBC odbcPg,
		odbcIfx;
	int iRespuestaSaldo;
	int iRespuestaMensualidad;

	// short buscarSaldoDiario();
	short ConsultaInsuficienciaSaldoInf(char *cNssx, char *cSecuenciax, int lFolioDatamart);
	short ConsultaInsuficienciaSaldo(char *cNssx, char *cSecuenciax, int lFolioDatamart);
	short ValidarSolicitudRetiro(char *cNssx);
	short ValidarSolicitudRetiroInf(char *cNssx);
	short abrirConexionServAfo();
	short ObtenerPagoMensual();
	short ObtenerPagoMensualInfx();
	short ObtenerSaldoPgs(char *cNssx, char *cSecuenciax);
	short ObtenerSaldoInfx(char *cNssx, char *cSecuenciax);
	short ConsultaMensualidadPost(char *cNssx);
	short ConsultaMensualidadInf(char *cNssx);

public:
	COperacionesPmg();
	~COperacionesPmg();
	short ConsultaInsuficiencia(char *cNssx, char *cSecuenciax, int lFolioDatamart, int iOpcion);
	short buscarSaldoaRecuperar(char *cNssx);
	short obtenerNumMensualidad(char *cNssx);
	OPERACIONES_PMG_IMSS *OperacionesPmg();
	int RespuestaSaldo();
	int RespuestaMensualidad();
};

#endif