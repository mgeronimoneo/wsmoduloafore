/*
 * CConsultarResolusion.cpp
 *
 *  Created on: Nov 14, 2016
 *      Author: eosunap
 */

#include "CConsultarResolusion.hpp"

CConsultarResolusion::CConsultarResolusion()
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

CConsultarResolusion::~CConsultarResolusion()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CConsultarResolusion::ConsultarResolusion(char *cNssx, int iTipoRetirox)
{
	short shRet = DEFAULT__;
	int iFlagReplica=-1;

	sprintf(cTexto, "[%s] ------ Inicia [Nss: %s tipoRetiro: %i]------",  __FUNCTION__, cNssx, iTipoRetirox);
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
				CFnConsultarResolucion xSelResol(&odbcPg);
				sprintf(cSql, "select nss,fechainiciovigencia,fechafinvigencia,numeroresolucion from fnConsultarResolucion('%s',%i)", cNssx, iTipoRetirox);
				if(xSelResol.Exec(cSql))
				{
					xSelResol.activarCols();
					if(xSelResol.leer())
					{
						 sprintf(stResolusion.cNss, "%s", xSelResol.nss);
						 sprintf(stResolusion.cFechaIniVigencia, "%s", xSelResol.fechainiciovigencia);
						 sprintf(stResolusion.cFechaFinVigencia, "%s", xSelResol.fechafinvigencia);
						 stResolusion.iResolucion = xSelResol.resolusion;
						 shRet=OK__;
					}
					else
						shRet=ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelResol.odbc->GetLastError( xSelResol.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelResol.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet=this->ConsultarResolusionBD(cNssx,iTipoRetirox);
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

	sprintf(cTexto, "[%s] ------ Termina [Nss: %s tipoRetiro: %i]------",  __FUNCTION__, cNssx, iTipoRetirox);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
short CConsultarResolusion::ConsultarResolusionBD(char *cNssx, int iTipoRetirox)
{
	short shRet=DEFAULT__;
	char cAnio[6]={0}, cMes[4]={0},cDia[4]={0};
	sprintf(cTexto, "[%s] validar fecha datamart en BD: %s",  __FUNCTION__, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	sprintf(cSql,"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	memset(cOutTexto,0,sizeof(cOutTexto));
	shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
	if(shRet==OK__)
	{
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx));
		cIpInfx[sizeof(cIpInfx)] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		if(shRet==OK__)
		{
			CFnConsultarResolucion xSelResol(&odbcIfx);
			if(iTipoRetirox==CVE_RET_DESEMPLEO_IMSS)
				sprintf(cSql, "SELECT limit 1 nss, nvl(to_char(fecha_ini_vigencia,'%cY-%cm-%cd'), '1900-01-01'), nvl(to_char(fecha_fin_vigencia,'%cY-%cm-%cd'), '1900-01-01'), num_resolucion "
						" FROM ret_parcial_resol WHERE nss = '%s' AND TODAY BETWEEN fecha_ini_vigencia AND fecha_fin_vigencia AND diag_procesar IN ('400', '106') and tipo_prestacion = 6 ORDER BY fecha_ini_vigencia DESC;", 37,37,37,37,37,37,cNssx);
			else
				sprintf(cSql, "SELECT limit 1 nss, nvl(to_char(fecha_ini_vigencia,'%cY-%cm-%cd'), '1900-01-01'), nvl(to_char(fecha_fin_vigencia,'%cY-%cm-%cd'), '1900-01-01'), num_resolucion "
							" FROM ret_parcial_resol WHERE nss = '%s' AND TODAY BETWEEN fecha_ini_vigencia AND fecha_fin_vigencia AND diag_procesar IN ('400', '106') and tipo_prestacion = 7 ORDER BY fecha_ini_vigencia DESC;", 37,37,37,37,37,37,cNssx);
			if(xSelResol.Exec(cSql))
			{
				xSelResol.activarCols();
				if(xSelResol.leer())
				{
					 sprintf(stResolusion.cNss, "%s", xSelResol.nss);
					 sprintf(stResolusion.cFechaIniVigencia, "%s", xSelResol.fechainiciovigencia);
					 sprintf(stResolusion.cFechaFinVigencia, "%s", xSelResol.fechafinvigencia);
					 stResolusion.iResolucion = xSelResol.resolusion;
					 shRet=OK__;
				}
				else
					shRet=ERR_NO_HAY_REG_BD;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelResol.odbc->GetLastError( xSelResol.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelResol.odbc->LastErrStr() );
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
CONSULTA_RESOLUSION* CConsultarResolusion::Resolucion()
{
	return &stResolusion;
}
