#ifndef CCONSULTASALDOVIVIENDA_HPP_
#define CCONSULTASALDOVIVIENDA_HPP_

#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "CUtileriasAfo.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CConsultadeSaldoVivienda.hpp"
#include "../includeStd.hpp"

class CConsultaSaldoVivienda
{
private:
	char cTexto[1950],
		cRutaLog[200],
		cBuffDat[SIZE_BUFF_DAT + 2],
		cTextoAux[700],
		cSql[256],
		cIpAfoGlobal[17],
		cProspecto[120],
		cIpInformix[40],
		cBuff[SIZE_BUFF_DAT],
		cOutTexto[768],
		cIpServAfo[17],
		cIpInfx[100];

	short shRet;

	C_ODBC odbcPg, odbcInformix;
	CBaseDato objBd;
	CONSULTA_SALDOS_VIVIENDA stDMI;

	// short enviarPropecto(RECIBE_SKT_PROSPECTO *stRec, char *cIpModulo, int iPuerto);
	// void enviarSms(int iTienda);
	// void abrirConexionInformix();
	// void obtenerIpInformix();
	void conexionInformix();
	void conexionPostgress();

public:
	CConsultaSaldoVivienda();
	~CConsultaSaldoVivienda();
	CONSULTA_SALDOS_VIVIENDA *getConsultaSaldosVivienda();
	short consultavivienda(char *cNss, char *fecha_solicitud, char *fecha_ini_pen);
};

#endif
