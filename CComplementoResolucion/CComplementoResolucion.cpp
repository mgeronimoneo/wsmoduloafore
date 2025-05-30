#include "CComplementoResolucion.hpp"
CComplementoResolucion::CComplementoResolucion()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
	memset(&stComplResolucion, 0, sizeof(COMPLEMENTO_RESOLUCION));
}

CComplementoResolucion::~CComplementoResolucion()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CComplementoResolucion::ConsultarComplementoResolusion(char *cNssx, int iTipoRetirox)
{
	short shRet = DEFAULT__;
	int iFlagReplica=-1;

	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia [Nss: %s tipoRetiro: %i]------",  __FUNCTION__, cNssx, iTipoRetirox);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		strncpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[sizeof(cIpServAfo)- 1] = '\0';

		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
		snprintf(cTexto, sizeof(cTexto), "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		if(shRet != OK__)
		{
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			strncpy(cIpServAfo, &cBuff[20], sizeof(SIZE_BUFF_DAT-20));
			cIpServAfo[sizeof(cIpServAfo)- 1] = '\0';
			
			CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
			snprintf(cTexto, sizeof(cTexto), "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
			if(shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if(shRet == OK__)
		{
			snprintf(cSql, sizeof(cSql), "select fnValidarReplica from fnValidarReplica()");
			shRet=CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
			if(shRet!=OK__)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
			if( iFlagReplica == 0 )
			{
				CFnConsultarComplementoResolucion xSelResolCompl(&odbcPg);
				snprintf(cSql, sizeof(cSql), "select numeroResolucion,idcomplemento,montopagado,montopagadocomplemento,saldorcv,saldorcvanterior,respuestaotrodatamart from fnConsultarComplementoResolucion('%s',%i)", cNssx, iTipoRetirox);
				if(xSelResolCompl.Exec(cSql))
				{
					xSelResolCompl.activarCols();
					if(xSelResolCompl.leer())
					{
						stComplResolucion.iNumResolucion=xSelResolCompl.numeroResolucion;
						stComplResolucion.iIdComplemento=xSelResolCompl.idcomplemento;
						stComplResolucion.dMontoPagado=xSelResolCompl.montopagado;
						stComplResolucion.dMontoPagadoComplemento=xSelResolCompl.montopagadocomplemento;
						stComplResolucion.dSaldoRcv=xSelResolCompl.saldorcv;
						stComplResolucion.dSaldoRcvAnterior=xSelResolCompl.saldorcvanterior;
						shRet=OK__;
					}
					else
						shRet=ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelResolCompl.odbc->GetLastError( xSelResolCompl.GetHstmt() );
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelResolCompl.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet=this->ConsultarComplementoResolusionBD(cNssx,iTipoRetirox);
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		snprintf(cTexto, sizeof(cTexto), "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_SERV_AFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	snprintf(cTexto, sizeof(cTexto), "[%s] Retorno: %i",  __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Termina [Nss: %s tipoRetiro: %i]------",  __FUNCTION__, cNssx, iTipoRetirox);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
short CComplementoResolucion::ConsultarComplementoResolusionBD(char *cNssx, int iTipoRetirox)
{

	///----- falta implementar esta funcion ------
	short shRet=DEFAULT__;
	char cAnio[6]={0}, cMes[4]={0},cDia[4]={0};
	snprintf(cTexto, sizeof(cTexto), "[%s] consultar resol compl en BD: %s",  __FUNCTION__, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cSql, sizeof(cSql), "select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	memset(cOutTexto,0,sizeof(cOutTexto));
	shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
	if(shRet==OK__)
	{
		strncpy (cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx)]='\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		if(shRet==OK__)
		{
			CFnConsultarComplementoResolucion xSelResolCompl(&odbcIfx);
			snprintf(cSql, sizeof(cSql), "execute function fnConsultarComplResolucion('%s', %i)", cNssx, iTipoRetirox);
			if(xSelResolCompl.Exec(cSql))
			{
				xSelResolCompl.activarCols();
				if(xSelResolCompl.leer())
				{
					stComplResolucion.iNumResolucion=xSelResolCompl.numeroResolucion;
					stComplResolucion.iIdComplemento=xSelResolCompl.idcomplemento;
					stComplResolucion.dMontoPagado=xSelResolCompl.montopagado;
					stComplResolucion.dMontoPagadoComplemento=xSelResolCompl.montopagadocomplemento;
					stComplResolucion.dSaldoRcv=xSelResolCompl.saldorcv;
					stComplResolucion.dSaldoRcvAnterior=xSelResolCompl.saldorcvanterior;
					 shRet=OK__;
				}
				else
					shRet=ERR_NO_HAY_REG_BD;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelResolCompl.odbc->GetLastError( xSelResolCompl.GetHstmt() );
				snprintf( cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelResolCompl.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_CNX_BASE_DATO;
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpInfx, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	else if(shRet==ERR_NO_HAY_REG_BD)
	{
		snprintf(cTexto, sizeof(cTexto), "[%s] Error: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}
COMPLEMENTO_RESOLUCION* CComplementoResolucion::ResolucionComplemento()
{
	return &stComplResolucion;
}
