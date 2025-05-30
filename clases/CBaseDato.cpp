#include "CBaseDato.hpp"
CBaseDato::CBaseDato()
{
	/*memset(cTexto, 0, sizeof(cTexto));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	sprintf(cRutaLog, "%s", RUTA_LOG);*/
}
CBaseDato::~CBaseDato()
{
}
short CBaseDato::abrirConexion(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cOut)
{
	short shRet = DEFAULT__;
	char cPwdBd[40] = {0};
	generarPasswordDB(cUsrBd, cBaseDato, cPwdBd);
	if (odbc->Open("PostgreSQL", cIpServ, cUsrBd, cPwdBd, cBaseDato) == 1)
		shRet = OK__;
	else
	{
		shRet = ERR_CNX_BASE_DATO;
		odbc->GetLastError();
		snprintf(cOut, sizeof(cOut), "[abrirConexion] Err en cnx bd: %s", odbc->LastErrStr());
	}
	return shRet;
}
short CBaseDato::abrirConexion(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cPwd, char *cOut)
{
	short shRet = DEFAULT__;
	char cPwdBd[40] = {0};
	if (strcmp(cPwd, "") == 0)
		generarPasswordDB(cUsrBd, cBaseDato, cPwdBd);
	else
		snprintf(cPwdBd, sizeof(cPwdBd), "%s", cPwd);
	if (odbc->Open("PostgreSQL", cIpServ, cUsrBd, cPwdBd, cBaseDato) == 1)
		shRet = OK__;
	else
	{
		shRet = ERR_CNX_BASE_DATO;
		odbc->GetLastError();
		snprintf(cOut, sizeof(cOut), "[abrirConexion] Err en cnx bd: %s", odbc->LastErrStr());
	}
	return shRet;
}
short CBaseDato::consultarNumero(C_ODBC *odbc, char *cSql, int &iOutCont, char *cOut)
{
	short shRet = DEFAULT__;
	CConsultaInt xSel(odbc);
	if (xSel.Exec(cSql))
	{
		xSel.activarCols();
		if (xSel.leer())
		{
			shRet = OK__;
			iOutCont = xSel.campoint;
		}
		else
		{
			snprintf(cOut, sizeof(cOut), "[CBaseDato::consultarNumero] No existen registros para la consulta: %s", cSql);
			shRet = ERR_NO_HAY_REG_BD;
		}
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		xSel.odbc->GetLastError(xSel.GetHstmt());
		snprintf(cOut, sizeof(cOut), "[CBaseDato::consultarNumero] Err[%s]: %s", cSql, xSel.odbc->LastErrStr());
	}
	xSel.ClearResults();
	return shRet;
}
short CBaseDato::consultarTexto(C_ODBC *odbc, char *cSql, char *cOutTexto)
{
	short shRet = DEFAULT__;
	CCampoTexto xSel(odbc);
	if (xSel.Exec(cSql))
	{
		xSel.activarCols();
		if (xSel.leer())
		{
			shRet = OK__;
			snprintf(cOutTexto, sizeof(cOutTexto), "%s", xSel.texto);
		}
		else
		{
			snprintf(cOutTexto, sizeof(cOutTexto), "[CBaseDato::consultarTexto] No existen registros para la consulta: %s", cSql);
			shRet = ERR_NO_HAY_REG_BD;
		}
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		xSel.odbc->GetLastError(xSel.GetHstmt());
		snprintf(cOutTexto, sizeof(cOutTexto), "[CBaseDato::consultarTexto] Err[%s]: %s", cSql, xSel.odbc->LastErrStr());
	}
	return shRet;
}
short CBaseDato::actualizarDatos(C_ODBC *odbc, char *cSql, char *cOut)
{
	short shRet = DEFAULT__;
	CCampoTexto xSel(odbc);
	if (xSel.Exec(cSql))
	{
		xSel.activarCols();
		xSel.leer();
		shRet = OK__;
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		xSel.odbc->GetLastError(xSel.GetHstmt());
		snprintf(cOut, sizeof(cOut), "[CBaseDato::actualizarDatos] Err[%s]: %s", cSql, xSel.odbc->LastErrStr());
	}
	return shRet;
}
short CBaseDato::abrirConexionIfx(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cPwd, char *cOut)
{
	short shRet = DEFAULT__;
	if (odbc->Open("Informix", cIpServ, cUsrBd, cPwd, cBaseDato) == 1)
		shRet = OK__;
	else
	{
		odbc->GetLastError();
		snprintf(cOut, sizeof(cOut), "[CBaseDato::abrirConexionInformix] error cnx[ %s ]: %s", cIpServ, odbc->LastErrStr());
		shRet = ERR_CNX_BASE_DATO;
	}

	return shRet;
}
short CBaseDato::consultarInfoShm(C_ODBC *odbcAdmon, char *cFechaCorte, SHM_INFOX &stShminf, char *cTextoOut)
{
	char cSql[256] = {0};
	short shRet = DEFAULT__;
	CTbMemoriaCompartida xSel(odbcAdmon);
	snprintf(cSql, sizeof(cSql), "SELECT idShm, totalreg FROM tbmemoriacompartida WHERE idTabla = %i AND fechaCorte >= '%s'", ID_CTRL_MAE_AFIL, cFechaCorte);
	if (xSel.Exec(cSql))
	{
		xSel.activarCols();
		if (xSel.leer())
		{
			stShminf.iIdShm = xSel.iIdShm;
			stShminf.iTotalReg = xSel.iTotalReg;
			shRet = OK__;
		}
		else
		{
			shRet = ERR_NO_HAY_REG_BD;
			snprintf(cTextoOut, sizeof(cTextoOut), "no se encontro inf shm mae afil");
		}
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		xSel.odbc->GetLastError(xSel.GetHstmt());
		snprintf(cTextoOut, sizeof(cTextoOut), "Err en consulta a inf shm mae afil: %s", xSel.odbc->LastErrStr());
	}
	return shRet;
}
short CBaseDato::consultarInfoShm(C_ODBC *odbcAdmon, int iIdTablaShm, SHM_INFOX &stShminf, char *cTextoOut)
{
	char cSql[256] = {0};
	short shRet = DEFAULT__;
	CTbMemoriaCompartida xSel(odbcAdmon);
	snprintf(cSql, sizeof(cSql), "SELECT idShm, totalreg FROM tbmemoriacompartida WHERE idTabla = %i AND fechaCorte=CURRENT_DATE-1", iIdTablaShm);
	if (xSel.Exec(cSql))
	{
		xSel.activarCols();
		if (xSel.leer())
		{
			stShminf.iIdShm = xSel.iIdShm;
			stShminf.iTotalReg = xSel.iTotalReg;
			shRet = OK__;
		}
		else
		{
			shRet = ERR_NO_HAY_REG_BD;
			snprintf(cTextoOut, sizeof(cTextoOut), "no se encontro inf shm");
		}
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		xSel.odbc->GetLastError(xSel.GetHstmt());
		snprintf(cTextoOut, sizeof(cTextoOut), "Err en consulta a inf shm: %s", xSel.odbc->LastErrStr());
	}
	return shRet;
}
short CBaseDato::consultarIpServidor(C_ODBC *odbc, short shId, char *cIp, char *cOutTexto)
{
	short shRet = DEFAULT__;
	char cSql[256] = {0};
	// Ejecuta la consulta en el servidor
	snprintf(cSql, sizeof(cSql), "SELECT TRIM(valor) FROM aforecontrol WHERE Tipo = 1 AND activo = 1 AND id = %i", shId);
	CCampoTexto xSel(odbc);
	if (xSel.Exec(cSql))
	{
		// Lee el resultado de la consulta
		xSel.activarCols();
		if (xSel.leer())
		{
			snprintf(cIp, sizeof(cIp), "%s", xSel.texto);
			shRet = OK__;
		}
		else
		{
			// Graba en el log que no encontro informacion
			shRet = ERR_NO_HAY_REG_BD;
			snprintf(cOutTexto, sizeof(cOutTexto), "[CBaseDato::consultarIpServidor] No se encontró el IP del servidor");
		}
	}
	else
	{
		// Graba en el log el error del odbc
		shRet = ERR_EXEC_SQL;
		xSel.odbc->GetLastError(xSel.GetHstmt());
		snprintf(cOutTexto, sizeof(cOutTexto), "[CBaseDato::consultarIpServidor] [shId: %i] %s Err: %s", shId, cSql, xSel.odbc->LastErrStr());
	}

	return shRet;
}
short CBaseDato::abrirConexionInfx(C_ODBC *odbc, char *cIpServ, char *cUsrBd, char *cBaseDato, char *cPwd, char *cOut)
{
	short shRet = DEFAULT__;
	char cPwdBd[40] = {0};
	if (strcmp(cPwd, "") == 0)
		generarPasswordDB(cUsrBd, cBaseDato, cPwdBd);
	else
		snprintf(cPwdBd, sizeof(cPwdBd), "%s", cPwd);
	if (odbc->Open("Informix", cIpServ, cUsrBd, cPwdBd, cBaseDato) == 1)
		shRet = OK__;
	else
	{
		shRet = ERR_CNX_BASE_DATO;
		odbc->GetLastError();
		snprintf(cOut, sizeof(cOut), "[abrirConexionInfx] Err en cnx bd: %s", odbc->LastErrStr());
	}
	return shRet;
}