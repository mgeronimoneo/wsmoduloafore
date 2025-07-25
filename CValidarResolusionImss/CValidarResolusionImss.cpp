#include "CValidarResolusionImss.hpp"
CValidarResolusionImss::CValidarResolusionImss()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}
CValidarResolusionImss::~CValidarResolusionImss()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CValidarResolusionImss::ValidarResolucionImss(char *cNssx, int iTipoRetirox)
{
	short shRet = DEFAULT__;
	int iFlagReplica = -1;

	snprintf(cTexto, sizeof(cTexto) "[%s] ------ Inicia [Nss: %s]------", __FUNCTION__, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if (shRet == OK__)
	{
		strncpy(cIpServAfo, cBuff, sizeof(cIpServAfo) - 1);
		cIpServAfo[sizeof(cIpServAfo) - 1] = '\0';
		
		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
		snprintf(cTexto, sizeof(cTexto) "[%s] ipServiciosAfore: %s", __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		if (shRet != OK__)
		{
			snprintf(cTexto, sizeof(cTexto) "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			strncpy(cIpServAfo, &cBuff[20], SIZE_BUFF_DAT - 20);
			cIpServAfo[SIZE_BUFF_DAT - 20 - 1] = '\0';
			CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
			snprintf(cTexto, sizeof(cTexto) "[%s] ipServiciosAfore: %s", __FUNCTION__, cIpServAfo);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
			if (shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto) "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if (shRet == OK__)
		{
			snprintf(cSql, sizeof(cSql), "select fnValidarReplica from fnValidarReplica()");
			shRet = CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
			if (shRet != OK__)
			{
				snprintf(cTexto, sizeof(cTexto) "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
			if (iFlagReplica == 0)
			{
				CFnValidarResolucionImss xSelResImss(&odbcPg);
				if (iTipoRetirox == CVE_RET_DESEMPLEO_IMSS)
					snprintf(cSql, sizeof(cSql), "select fechaVigencia::char(10),fechaVigenciaAnt::char(10),fechaProximasolicitud::char(10),retorno,mensaje::char(50),diagnosticoprocesar from fnValidarResolucionDesempleo('%s')", cNssx);
				else
					snprintf(cSql, sizeof(cSql), "select '1900-01-01','1900-01-01','1900-01-01',fnValidarUltimoRetParcial,case when fnValidarUltimoRetParcial=0 then 'NO HAY RETIRO' else 'TIENE UN RETIRO' end::char(50),0 from fnValidarUltimoRetParcial(2,'%s')", cNssx);
				if (xSelResImss.Exec(cSql))
				{
					xSelResImss.activarCols();
					if (xSelResImss.leer())
					{
						stValResImss.iRetorno = xSelResImss.retorno;
						stValResImss.iDiagProcesar = xSelResImss.diagnosticoProcesar;
						snprintf(stValResImss.cFechaVigencia, sizeof(stValResImss.cFechaVigencia), "%s", xSelResImss.fecha_vigencia);
						snprintf(stValResImss.cFechaVigenciaAnterior, sizeof(stValResImss.cFechaVigenciaAnterior), "%s", xSelResImss.fecha_vigencia_ant);
						snprintf(stValResImss.cFechaProximaSolicitud, sizeof(stValResImss.cFechaProximaSolicitud), "%s", xSelResImss.fecha_proxima_sol);
						snprintf(stValResImss.cMensaje, sizeof(stValResImss.cMensaje), "%s", xSelResImss.mensaje);
						CUtileriasAfo::quitarEspacioDerecha(stValResImss.cMensaje);
						shRet = OK__;
					}
					else
						shRet = ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelResImss.odbc->GetLastError(xSelResImss.GetHstmt());
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelResImss.odbc->LastErrStr());
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet = this->validarResolucionBd(cNssx, iTipoRetirox);
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		snprintf(cTexto, sizeof(cTexto) "[%s] Error al leer archivo dat: %s [%s]", __FUNCTION__, cOutTexto, (char *)IP_SERV_AFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	snprintf(cTexto, sizeof(cTexto) "[%s] Retorno: %i", __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	snprintf(cTexto, sizeof(cTexto) "[%s] ------ Termina [Nss: %s]------", __FUNCTION__, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
VAL_RESOLUCION_IMSS *CValidarResolusionImss::ResolucionImss()
{
	return &stValResImss;
}
short CValidarResolusionImss::validarResolucionBd(char *cNss, int iTipoRetirox)
{
	short shRet = DEFAULT__;
	char cAnio[6] = {0}, cMes[4] = {0}, cDia[4] = {0};
	snprintf(cTexto, sizeof(cTexto) "[%s] validar resolucion en BD: %s", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cSql, sizeof(cSql), "select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	memset(cOutTexto, 0, sizeof(cOutTexto));
	shRet = CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
	if (shRet == OK__)
	{
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx) - 1] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
		snprintf(cIpInfx, sizeof(cIpInfx), "%s;SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;", cIpInfx);
		shRet = CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx, (char *)USR_BD_SAFRE_AF, (char *)BD_SAFRE_AF, (char *)PWD_BD_SAFRE_AF, cOutTexto);
		if (shRet == OK__)
		{
			CFnValidarResolucionImss xSelValRes(&odbcIfx);
			if (iTipoRetirox == CVE_RET_DESEMPLEO_IMSS)
				snprintf(cSql, sizeof(cSql), "SELECT nvl(unnamed_col_1,'01/01/1900'), nvl(unnamed_col_2,'01/01/1900'), nvl(unnamed_col_3,'01/01/1900'), unnamed_col_4,nvl(unnamed_col_5,''),nvl(unnamed_col_6,0) FROM table(fn_validar_resolucion_desempleo('%s')) ;", cNss);
			else
				snprintf(cSql, sizeof(cSql), "select '01/01/1900','01/01/1900','01/01/1900', unnamed_col_1, case when unnamed_col_1=0 then 'NO HAY RETIRO' else 'TIENE UN RETIRO' end::char(50), 0 from table(fn_ExisteRetiro(2,'%s'))", cNss);

			if (xSelValRes.Exec(cSql))
			{
				xSelValRes.activarCols();
				if (xSelValRes.leer())
				{
					stValResImss.iRetorno = xSelValRes.retorno;
					stValResImss.iDiagProcesar = xSelValRes.diagnosticoProcesar;
					memcpy(cDia, xSelValRes.fecha_vigencia, 2);
					memcpy(cMes, &xSelValRes.fecha_vigencia[3], 2);
					memcpy(cAnio, &xSelValRes.fecha_vigencia[6], 4);
					snprintf(stValResImss.cFechaVigencia, sizeof(stValResImss.cFechaVigencia), "%s-%s-%s", cAnio, cMes, cDia);

					memcpy(cDia, xSelValRes.fecha_vigencia_ant, 2);
					memcpy(cMes, &xSelValRes.fecha_vigencia_ant[3], 2);
					memcpy(cAnio, &xSelValRes.fecha_vigencia_ant[6], 4);
					snprintf(stValResImss.cFechaVigenciaAnterior, sizeof(stValResImss.cFechaVigenciaAnterior), "%s-%s-%s", cAnio, cMes, cDia);

					memcpy(cDia, xSelValRes.fecha_proxima_sol, 2);
					memcpy(cMes, &xSelValRes.fecha_proxima_sol[3], 2);
					memcpy(cAnio, &xSelValRes.fecha_proxima_sol[6], 4);
					snprintf(stValResImss.cFechaProximaSolicitud, sizeof(stValResImss.cFechaProximaSolicitud), "%s-%s-%s", cAnio, cMes, cDia);

					snprintf(stValResImss.cMensaje, sizeof(stValResImss.cMensaje), "%s", xSelValRes.mensaje);
					CUtileriasAfo::quitarEspacioDerecha(stValResImss.cMensaje);
					shRet = OK__;
				}
				else
					shRet = ERR_NO_HAY_REG_BD;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelValRes.odbc->GetLastError(xSelValRes.GetHstmt());
				snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelValRes.odbc->LastErrStr());
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_CNX_BASE_DATO;
			snprintf(cTexto, sizeof(cTexto) "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpInfx, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	else if (shRet == ERR_NO_HAY_REG_BD)
	{
		snprintf(cTexto, sizeof(cTexto) "[%s] Error: %s", __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		snprintf(cTexto, sizeof(cTexto) "[%s] Error al consultar control: %s", __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}
