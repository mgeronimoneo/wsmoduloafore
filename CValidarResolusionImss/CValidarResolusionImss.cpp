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
	sprintf(cRutaLog, "%s", RUTA_LOGX);
}
CValidarResolusionImss::~CValidarResolusionImss()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CValidarResolusionImss::ValidarResolucionImss(char *cNssx,int iTipoRetirox)
{
	short shRet = DEFAULT__;
	int iFlagReplica=-1;

	sprintf(cTexto, "[%s] ------ Inicia [Nss: %s]------",  __FUNCTION__, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[16]={0};
		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
		sprintf(cTexto, "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		if(shRet != OK__)
		{
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			memcpy(cIpServAfo, &cBuff[20], sizeof(SIZE_BUFF_DAT-20));
			cIpServAfo[16]={0};
			CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
			sprintf(cTexto, "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
			if(shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if(shRet == OK__)
		{
			sprintf(cSql, "select fnValidarReplica from fnValidarReplica()");
			shRet=CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
			if(shRet!=OK__)
			{
				sprintf(cTexto, "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
			if( iFlagReplica == 0 )
			{
				CFnValidarResolucionImss xSelResImss(&odbcPg);
				if(iTipoRetirox==CVE_RET_DESEMPLEO_IMSS)
					sprintf(cSql, "select fechaVigencia::char(10),fechaVigenciaAnt::char(10),fechaProximasolicitud::char(10),retorno,mensaje::char(50),diagnosticoprocesar from fnValidarResolucionDesempleo('%s')",cNssx);
				else
					sprintf(cSql, "select '1900-01-01','1900-01-01','1900-01-01',fnValidarUltimoRetParcial,case when fnValidarUltimoRetParcial=0 then 'NO HAY RETIRO' else 'TIENE UN RETIRO' end::char(50),0 from fnValidarUltimoRetParcial(2,'%s')",cNssx);
				if(xSelResImss.Exec(cSql))
				{
					xSelResImss.activarCols();
					if(xSelResImss.leer())
					{
						stValResImss.iRetorno=xSelResImss.retorno;
						stValResImss.iDiagProcesar=xSelResImss.diagnosticoProcesar;
						sprintf(stValResImss.cFechaVigencia, "%s", xSelResImss.fecha_vigencia);
						sprintf(stValResImss.cFechaVigenciaAnterior, "%s", xSelResImss.fecha_vigencia_ant);
						sprintf(stValResImss.cFechaProximaSolicitud, "%s", xSelResImss.fecha_proxima_sol);
						sprintf(stValResImss.cMensaje, "%s", xSelResImss.mensaje);
						CUtileriasAfo::quitarEspacioDerecha(stValResImss.cMensaje);
						shRet=OK__;
					}
					else
						shRet=ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelResImss.odbc->GetLastError( xSelResImss.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelResImss.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet=this->validarResolucionBd(cNssx,iTipoRetirox);
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		sprintf(cTexto, "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_SERV_AFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	sprintf(cTexto, "[%s] Retorno: %i",  __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	sprintf(cTexto, "[%s] ------ Termina [Nss: %s]------",  __FUNCTION__, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
VAL_RESOLUCION_IMSS* CValidarResolusionImss::ResolucionImss()
{
	return &stValResImss;
}
short CValidarResolusionImss::validarResolucionBd(char *cNss, int iTipoRetirox)
{
	short shRet=DEFAULT__;
	char cAnio[6]={0}, cMes[4]={0},cDia[4]={0};
	sprintf(cTexto, "[%s] validar resolucion en BD: %s",  __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	sprintf(cSql,"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	memset(cOutTexto,0,sizeof(cOutTexto));
	shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
	if(shRet==OK__)
	{
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx)] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		if(shRet==OK__)
		{
			CFnValidarResolucionImss xSelValRes(&odbcIfx);
			if(iTipoRetirox==CVE_RET_DESEMPLEO_IMSS)
				sprintf(cSql, "SELECT nvl(unnamed_col_1,'01/01/1900'), nvl(unnamed_col_2,'01/01/1900'), nvl(unnamed_col_3,'01/01/1900'), unnamed_col_4,nvl(unnamed_col_5,''),nvl(unnamed_col_6,0) FROM table(fn_validar_resolucion_desempleo('%s')) ;", cNss);
			else
				sprintf(cSql, "select '01/01/1900','01/01/1900','01/01/1900', unnamed_col_1, case when unnamed_col_1=0 then 'NO HAY RETIRO' else 'TIENE UN RETIRO' end::char(50), 0 from table(fn_ExisteRetiro(2,'%s'))", cNss);

			if(xSelValRes.Exec(cSql))
			{
				xSelValRes.activarCols();
				if(xSelValRes.leer())
				{
					stValResImss.iRetorno=xSelValRes.retorno;
					stValResImss.iDiagProcesar=xSelValRes.diagnosticoProcesar;
					memcpy(cDia, xSelValRes.fecha_vigencia, 2);
					memcpy(cMes, &xSelValRes.fecha_vigencia[3], 2);
					memcpy(cAnio, &xSelValRes.fecha_vigencia[6], 4);
					sprintf(stValResImss.cFechaVigencia, "%s-%s-%s", cAnio, cMes, cDia);

					memcpy(cDia, xSelValRes.fecha_vigencia_ant, 2);
					memcpy(cMes, &xSelValRes.fecha_vigencia_ant[3], 2);
					memcpy(cAnio, &xSelValRes.fecha_vigencia_ant[6], 4);
					sprintf(stValResImss.cFechaVigenciaAnterior, "%s-%s-%s", cAnio, cMes, cDia);

					memcpy(cDia, xSelValRes.fecha_proxima_sol, 2);
					memcpy(cMes, &xSelValRes.fecha_proxima_sol[3], 2);
					memcpy(cAnio, &xSelValRes.fecha_proxima_sol[6], 4);
					sprintf(stValResImss.cFechaProximaSolicitud, "%s-%s-%s", cAnio, cMes, cDia);

					sprintf(stValResImss.cMensaje, "%s", xSelValRes.mensaje);
					CUtileriasAfo::quitarEspacioDerecha(stValResImss.cMensaje);
					shRet=OK__;
				}
				else
					shRet=ERR_NO_HAY_REG_BD;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelValRes.odbc->GetLastError( xSelValRes.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelValRes.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_CNX_BASE_DATO;
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpInfx, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	else if(shRet==ERR_NO_HAY_REG_BD)
	{
		sprintf(cTexto, "[%s] Error: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}
