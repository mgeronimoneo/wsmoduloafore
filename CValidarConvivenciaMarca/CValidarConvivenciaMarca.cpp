#include "CValidarConvivenciaMarca.hpp"
CValidarConvivenciaMarca::CValidarConvivenciaMarca()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
	iFolio=0;
}
CValidarConvivenciaMarca::~CValidarConvivenciaMarca()
{
	odbcPg.Close();
}
short CValidarConvivenciaMarca::ValidarConvivencia( short shOpcion, short shTipoTrabajador, char* cNss, char* cTipoRetiro )
{
	short shRet = DEFAULT__;
	int iFlagReplica=-1;

	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia [Opc: %i TpoTrab: %i Nss: %s TpoRet: %s]------",  __FUNCTION__, shOpcion, shTipoTrabajador, cNss, cTipoRetiro);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		strncpy(cIpServAfo, cBuff, sizeof(cIpServAfo) - 1);
cIpServAfo[sizeof(cIpServAfo)- 1] = '\0';
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
			strncpy(cIpServAfo, &cBuff[20], sizeof(SIZE_BUFF_DAT-20));
			cIpServAfo[sizeof(cIpServAfo)] = '\0';
			
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
			//if( iFlagReplica == 0 )
			{
				CConsultarConvivenciaMarca xSelConvMarca(&odbcPg);
				snprintf(cSql, sizeof(cSql), "select codigo,descripcion,tienemarca from fnConvivenciaMarcaOperativa(%i::SMALLINT, '%s', %i::smallint , '%s');", shOpcion, cNss, shTipoTrabajador, cTipoRetiro);
				
				snprintf(cTexto, sizeof(cTexto), "[%s]ENTRO POSTGRESQL | SQL POSTGRESQL: %s", __FUNCTION__, cSql );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				
				if(xSelConvMarca.Exec(cSql))
				{
				
					xSelConvMarca.activarCols();
					if(xSelConvMarca.leer())
					{
						stConvMarca.shCodigo = (short)xSelConvMarca.codigo;
						stConvMarca.shTieneMarca=(short)xSelConvMarca.tienemarca;
						snprintf(stConvMarca.cMensaje, sizeof(stConvMarca.cMensaje), "%s", xSelConvMarca.descripcion);
						CUtileriasAfo::quitarEspacioDerecha(stConvMarca.cMensaje);
						
						snprintf(cTexto, sizeof(cTexto), "[%s] shCodigo POSTGRESQL: %i | shTieneMarca POSTGRESQL: %i | shTieneMarca POSTGRESQL: %s ", __FUNCTION__, stConvMarca.shCodigo,stConvMarca.shTieneMarca,stConvMarca.cMensaje );
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
												
						if(stConvMarca.shCodigo == 0 && stConvMarca.shTieneMarca == 0) 
						{
							shRet=ERR_NO_HAY_REG_BD;
						}
						else
							shRet=OK__;
					}
					else
						shRet=ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelConvMarca.odbc->GetLastError( xSelConvMarca.GetHstmt() );
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelConvMarca.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			//else
			if(shRet!=OK__)
				shRet=this->ValidarConvivenciaBd(shOpcion, shTipoTrabajador, cNss, cTipoRetiro);
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

	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Termina [Opc: %i TpoTrab: %i Nss: %s TpoRet: %s]------",  __FUNCTION__, shOpcion, shTipoTrabajador, cNss, cTipoRetiro);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
CONVIVENCIA_MARCAS* CValidarConvivenciaMarca::ConviveMarcas()
{
	return &stConvMarca;
}
short CValidarConvivenciaMarca::ValidarConvivenciaBd(short shOpcion, short shTipoTrabajador, char* cNss, char* cTipoRetiro)
{
	short shRet=DEFAULT__;
	char cAnio[6]={0}, cMes[4]={0},cDia[4]={0};
	snprintf(cTexto, sizeof(cTexto), "[%s] validar convivencia en BD: %s",  __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cSql, sizeof(cSql),"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	memset(cOutTexto,0,sizeof(cOutTexto));
	shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
	if(shRet==OK__)
	{
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx) - 1] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		if(shRet==OK__)
		{
		
			CConsultarConvivenciaMarca xSelValConv(&odbcIfx);
			snprintf(cSql, sizeof(cSql), "select unnamed_col_2, unnamed_col_3::char(200), unnamed_col_1 from table(fn_verificaConvivenciaMarcasServicios(%i, %i, '%s', '%s'))", shOpcion, shTipoTrabajador, cNss, cTipoRetiro);
			
			snprintf(cTexto, sizeof(cTexto), "[%s] ENTRO SAFRE SQL SAFRE: %s", __FUNCTION__, cSql );
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					
			if(xSelValConv.Exec(cSql))
			{
				xSelValConv.activarCols();
				if(xSelValConv.leer())
				{
					stConvMarca.shCodigo = (short)xSelValConv.codigo;
					stConvMarca.shTieneMarca=(short)xSelValConv.tienemarca;
					snprintf(stConvMarca.cMensaje, sizeof(stConvMarca.cMensaje), "%s", xSelValConv.descripcion);
					CUtileriasAfo::quitarEspacioDerecha(stConvMarca.cMensaje);
					shRet=OK__;
					
					
					snprintf(cTexto, sizeof(cTexto), "[%s] shCodigo SAFRE: %i | shTieneMarca SAFRE: %i | shTieneMarca SAFRE: %s ", __FUNCTION__, stConvMarca.shCodigo,stConvMarca.shTieneMarca,stConvMarca.cMensaje );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					
				}
				else
					shRet=ERR_NO_HAY_REG_BD;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelValConv.odbc->GetLastError( xSelValConv.GetHstmt() );
				snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelValConv.odbc->LastErrStr() );
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
