#include "CMontosFechasDatamart.hpp"
CMontosFechasDatamart::CMontosFechasDatamart()
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

CMontosFechasDatamart::~CMontosFechasDatamart()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CMontosFechasDatamart::MontosFechasDatamart(char *cNssx, int iTipoRetirox)
{
	short shRet = DEFAULT__;
	int iFlagReplica=-1;

	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia [Nss: %s tipoRetiro: %i]------",  __FUNCTION__, cNssx, iTipoRetirox);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[16]={0};
		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
		snprintf(cTexto, sizeof(cTexto), "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		if(shRet != OK__)
		{
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			memcpy(cIpServAfo, &cBuff[20], sizeof(SIZE_BUFF_DAT-20));
			cIpServAfo[16]={0};
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
				CConsMontosFechasDatamart xSelMontos(&odbcPg);
				snprintf(cSql, sizeof(cSql), "select nss,salariobasea,salariobaseb,fechamatrimoniodesempleo,fechainiciovigencia,ultimosalariobasecot,ultimas250semanassbc from fnObtenerMontosFechasDatamart('%s',%i)", cNssx, iTipoRetirox);
				if(xSelMontos.Exec(cSql))
				{
					xSelMontos.activarCols();
					if(xSelMontos.leer())
					{
						 stMontos.dUltimaSbc=xSelMontos.ultimasbc;
						 stMontos.dUltimas250SemanasSbc=xSelMontos.ultimo250semanassbc;
						 stMontos.dSalarioBaseA=xSelMontos.salariobasea;
						 stMontos.DSalarioBaseB=xSelMontos.salariobaseb;
						 snprintf(stMontos.cNss, sizeof(stMontos.cNss), "%s", xSelMontos.nss);
						 snprintf(stMontos.cFechaIniVigencia, sizeof(stMontos.cFechaIniVigencia), "%s", xSelMontos.fechainivigencia);
						 snprintf(stMontos.cFechaMatDes, sizeof(stMontos.cFechaMatDes), "%s", xSelMontos.fechamatdes);
						 shRet=OK__;
					}
					else
						shRet=ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelMontos.odbc->GetLastError( xSelMontos.GetHstmt() );
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelMontos.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet=this->MontosFechasDatamartBD(cNssx,iTipoRetirox);
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
short CMontosFechasDatamart::MontosFechasDatamartBD(char *cNssx, int iTipoRetirox)
{
	short shRet=DEFAULT__;
	char cAnio[6]={0}, cMes[4]={0},cDia[4]={0};
	snprintf(cTexto, sizeof(cTexto), "[%s] validar fecha datamart en BD: %s",  __FUNCTION__, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cSql, sizeof(cSql),"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
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
			CConsMontosFechasDatamart xSelMontos(&odbcIfx);
			if(iTipoRetirox==CVE_RET_DESEMPLEO_IMSS)
				snprintf(cSql, sizeof(cSql), "SELECT limit 1 nss, salario_base_a, salario_base_b,nvl(to_char(fecha_fall_mat_des, '%cY-%cm-%cd'), '1900-01-01'), nvl(to_char(fecha_ini_vigencia, '%cY-%cm-%cd'), '1900-01-01'), 0::double precision, 0::double precision FROM ret_parcial_resol "
							  "WHERE nss = '%s' AND tipo_prestacion = 6  AND salario_base_a is not null AND salario_base_a is not null ORDER BY fecha_fin_vigencia DESC;", 37,37,37,37,37,37,cNssx);
			else
				snprintf(cSql, sizeof(cSql), "SELECT limit 1 nss, salario_base_a, salario_base_b, nvl(to_char(fecha_fall_mat_des, '%cY-%cm-%cd'), '1900-01-01'), nvl(to_char(fecha_ini_vigencia, '%cY-%cm-%cd'), '1900-01-01'), 0::double precision, 0::double precision FROM ret_parcial_resol "
							 "WHERE nss = '%s' AND tipo_prestacion = 7  AND salario_base_a is not null AND salario_base_a is not null ORDER BY fecha_fin_vigencia DESC;",37,37,37,37,37,37,cNssx);

			if(xSelMontos.Exec(cSql))
			{
				xSelMontos.activarCols();
				if(xSelMontos.leer())
				{
					stMontos.dUltimaSbc=0;
					stMontos.dUltimas250SemanasSbc=0;
					if(xSelMontos.salariobasea>0)
						stMontos.dUltimaSbc=xSelMontos.salariobasea;
					if(xSelMontos.salariobaseb>0)
						stMontos.dUltimas250SemanasSbc=xSelMontos.salariobaseb;
					stMontos.dSalarioBaseA=xSelMontos.salariobasea;
					stMontos.DSalarioBaseB=xSelMontos.salariobaseb;
					snprintf(stMontos.cNss, sizeof(stMontos.cNss), "%s", xSelMontos.nss);
					snprintf(stMontos.cFechaIniVigencia, sizeof(stMontos.cFechaIniVigencia), "%s", xSelMontos.fechainivigencia);
					snprintf(stMontos.cFechaMatDes, sizeof(stMontos.cFechaMatDes), "%s", xSelMontos.fechamatdes);
					shRet=OK__;
				}
				else
					shRet=ERR_NO_HAY_REG_BD;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelMontos.odbc->GetLastError( xSelMontos.GetHstmt() );
				snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelMontos.odbc->LastErrStr() );
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
MONTOS_FECHAS_DATAMART* CMontosFechasDatamart::MontosDatamart()
{
	return &stMontos;
}
