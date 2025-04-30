#include "CConsultarCtaRegimen.hpp"
CConsultarCtaRegimen::CConsultarCtaRegimen()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpAdmon, 0, sizeof(cIpAdmon));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	memset(cNss, 0, sizeof(cNss));
	memset(cIpServAfo,0,sizeof(cIpServAfo));
	memset(&stInfShmSiefore, 0, sizeof(SHM_INFOX));
	memset(&stSiefore, 0, sizeof(SIEFORE));
	//shmSiefore = NULL;
	sprintf(cRutaLog, "%s", RUTA_LOGX);
}

CConsultarCtaRegimen::~CConsultarCtaRegimen()
{
	odbcPg.Close();
	odbcIfx.Close();
}

short CConsultarCtaRegimen::consultarSieforeCtaRegimen(char *cNssx)
{
	short shRet = DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cTexto, "[%s] ------ Inicia [Nss: %s]------",  __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_ADMONAFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	
	if(shRet == OK__)
	{
		memcpy(cIpAdmon, cBuff, sizeof(cIpAdmon));
		cIpAdmon[16]={0};
		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
		if(shRet != OK__)
		{
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpAdmon, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpAdmon, 0, sizeof(cIpAdmon));
			memcpy(cIpAdmon, &cBuff[20], SIZE_BUFF_DAT-20);
			cIpAdmon[16]={0};
			CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
			sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
			if(shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpAdmon, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if(shRet == OK__)
		{
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_CTA_REGIMEN, stInfShmSiefore, cOutTexto);	
			if(shRet == OK__)
			{
				sprintf(cTexto, "[%s] Info shm cta_regimen : id=%i regs=%i  bytesx  = %ld ",  __FUNCTION__,
						stInfShmSiefore.iIdShm, stInfShmSiefore.iTotalReg, sizeof(SIEFORE) * stInfShmSiefore.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				shmSiefore = (SIEFORE *) CUtileriasAfo::obtenerShm(stInfShmSiefore.iIdShm, SHM_RDONLY, cOutTexto);
				if( shmSiefore != (void *)-1 )
				{
					shRet = this->buscarSieforeCta();
					if(shRet!=OK__)
					{
						shRet= this->ConsultarSieforeBD();
					}
				}
			}
			else if(shRet==ERR_NO_HAY_REG_BD)
			{
				sprintf(cTexto, "[%s] Error: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet= this->ConsultarSieforeBD(); //Metodo a Informix directo
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				sprintf(cTexto, "[%s] Error al consultar info Shm: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet= this->ConsultarSieforeBD(); // Metodo a Informix directo
			}
		}
	}	
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		sprintf(cTexto, "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_ADMONAFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	sprintf(cTexto, "[%s] Retorno: %i",  __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	sprintf(cTexto, "[%s] ------ Termina [Nss: %s]------",  __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	return shRet;
}
short CConsultarCtaRegimen::buscarSieforeCta()
{
	short shRet = DEFAULT__;
	SIEFORE *stSieforeBuscar = NULL;

	memcpy(stSiefore.cNss, cNss, SIZE_NSS);
	stSieforeBuscar = (SIEFORE *)bsearch(&stSiefore, (void *)shmSiefore, stInfShmSiefore.iTotalReg, sizeof(SIEFORE), compararNssCtaRegimen);
	if(stSieforeBuscar != NULL)
	{
		memcpy(&stSiefore, stSieforeBuscar, sizeof(SIEFORE));
		shRet = OK__;
	}
	else
	{
		shRet = ERR_NO_HAY_REG_SHM;
		sprintf(cTexto, "[%s] Nss: %s - NO_HAY_REG",  __FUNCTION__, cNss);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}

short CConsultarCtaRegimen::ConsultarSieforeBD()
{
	short shRet=DEFAULT__, shCont=0;

	sprintf(cTexto, "[%s] Buscar Siefore en BD: %s",  __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	odbcPg.Close();
	
	shRet=this->abrirConexionServAfo();
	if(shRet==OK__)
	{
		sprintf(cSql,"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
		memset(cOutTexto,0,sizeof(cOutTexto));
		shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
		if(shRet==OK__)
		{
			strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);			
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
			shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
			
			if(shRet==OK__)
			{	
				CConsultarSiefore xSelSiefore(&odbcIfx);
				sprintf(cSql, "SELECT nss,subcuenta,codigo_siefore FROM cta_regimen WHERE nss = '%s';", cNss);
				CUtileriasAfo::grabarLogx(cRutaLog, cSql);
				
				if(xSelSiefore.Exec(cSql))
				{
					memcpy(stSiefore.cNss, cNss, SIZE_NSS);
					xSelSiefore.activarCols();
					while(xSelSiefore.leer())
					{
						memcpy(stSiefore.arrCtaRegimen[shCont].cNss, xSelSiefore.nss, sizeof(stSiefore.arrCtaRegimen[shCont].cNss));
						stSiefore.arrCtaRegimen[shCont].cSubCuenta=xSelSiefore.subCuenta;
						stSiefore.arrCtaRegimen[shCont].cSiefore=xSelSiefore.siefore;
						
						shCont++;
					}
					if(shCont>0)
					{
						shRet=OK__;
						shRegistros = shCont;
					}
					else
						shRet=ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelSiefore.odbc->GetLastError( xSelSiefore.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelSiefore.odbc->LastErrStr() );
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
	}
	return shRet;
}
short CConsultarCtaRegimen::abrirConexionServAfo()
{
	short shRet=DEFAULT__;
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
			memcpy(cIpServAfo, &cBuff[20], SIZE_BUFF_DAT-20);
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
	}
	return shRet;
}

//Funcion para retornar los datos obtenidos en instancia de la estructura shmSiefore.
SIEFORE *CConsultarCtaRegimen::RespSiefore()
{
	return &stSiefore;
}

short CConsultarCtaRegimen::registrosctaregimen()
{
	return shRegistros;
}