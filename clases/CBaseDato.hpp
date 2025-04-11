#ifndef CBASEDATO_HPP_
#define CBASEDATO_HPP_

#include "../definiciones.hpp"
#include "../estructuras.hpp"
#include "C_ODBC.hpp"
#include "password.h"
#include "../clasessql/CConsultaInt.hpp"
#include "../clasessql/CCampoTexto.hpp"
#include "../clasessql/CTbMemoriaCompartida.hpp"

class CBaseDato
{
	private:

	public:
		CBaseDato();
		~CBaseDato();

		static short abrirConexion(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cOut);
		static short abrirConexion(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cPwd, char *cOut);
		static short abrirConexionIfx(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cPwd, char *cOut);
		static short consultarNumero(C_ODBC *odbc, char *cSql, int &iOutCont, char *cOut);
		static short consultarTexto(C_ODBC *odbc, char *cSql, char *cOutTexto);
		static short actualizarDatos(C_ODBC *odbc, char *cSql, char *cOut);
		static short consultarInfoShm( C_ODBC *odbcAdmon, char *cFechaCorte, SHM_INFOX &stShminf, char *cTextoOut);
		static short consultarInfoShm( C_ODBC *odbcAdmon, int iIdTablaShm, SHM_INFOX &stShminf, char *cTextoOut);
		static short consultarIpServidor(C_ODBC* odbc, short shId, char *cIp, char* cOutTexto);
		static short abrirConexionInfx(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cPwd, char *cOut);
};

#endif
