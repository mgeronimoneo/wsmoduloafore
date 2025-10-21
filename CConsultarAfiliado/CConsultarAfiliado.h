#ifndef CCONSULTARAFILIADO_H_
#define CCONSULTARAFILIADO_H_

#include "../definiciones.hpp"
#include "CUtileriasAfo.hpp"
#include "clases/CInfoTrabajadorIni.hpp"
#include "clases/CInfoTrabajadorCompleta.hpp"
#include "../clases/funcionescomparar.hpp"
#include "../clases/CBaseDato.hpp"
#include "../clasessql/CTbMemoriaCompartida.hpp"
#include "../clasessql/CCampoTexto.hpp"
#include "../estructuras.hpp"

class CConsultarAfiliado
{
private:
	char cTexto[1024],
		cRutaLog[100],
		cBuffDat[SIZE_BUFF_DAT],
		cIpPg[17],
		cSql[512];
	C_ODBC odbcSafreAf, odbcAdmonAfo;

	SHM_INFOX shmInfoMaestroAfiliado;
	MAESTRO_AFILIADO *shmMaestroAfiliado;
	DATOS_TRABAJADOR_EXP_IDEN stDatosTrabajador;

	short obtenerConexionBaseDato();
	short obtenerFechaCorteShmMaestroAfiliado(char *cOutFechaCorte);
	void formatoFechaConSeparador(char *pcFecha, char *cOutFechaMDY, char pcSeparador);

public:
	CConsultarAfiliado();
	~CConsultarAfiliado();

	short consultarAfiliado(char *cNombres, char *cPaterno, char *cMaterno, short shEstadoNac, char *cFechaNac);
	short obtenerInformacionTrabajador(long lFolioServicio);
	DATOS_TRABAJADOR_EXP_IDEN *getInformacionTrabajador();
};

#endif
