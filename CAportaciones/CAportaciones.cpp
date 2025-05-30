#include "CAportaciones.hpp"
CAportaciones::CAportaciones()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(cIpAdmon, 0, sizeof(cIpAdmon));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}
CAportaciones::~CAportaciones()
{
	odbcPg.Close();
}


short CAportaciones::ObtenerAportaciones(char* sNss, char* dFechaIniPension, int opcion)
{
	char auxFechaPension[20]={0};
	char* f1;
	char* f2;
	char* f3;
	short shRet = DEFAULT__;
	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia [Nss: %s], [Inicio Pension: %s]------",  __FUNCTION__, sNss,dFechaIniPension);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		strncpy(cIpAdmon, cBuff, sizeof(cIpAdmon));
		cIpAdmon[sizeof(cIpAdmon)- 1] = '\0';

		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		snprintf(cTexto, sizeof(cTexto), "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		
		snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia ------",  __FUNCTION__);
		
		odbcPg.Close();
		shRet=this->abrirConexionServAfo();
		if(shRet==OK__) 
		{
			snprintf(cSql, sizeof(cSql), "select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);			
			memset(cOutTexto,0,sizeof(cOutTexto));
			shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
			
			if(shRet==OK__)
			{
				strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx));
				cIpInfx[sizeof(cIpInfx)- 1] = '\0';

				CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
				strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
				snprintf(cTexto, sizeof(cTexto), "[%s]IP INFORMIX [%s] ", __FUNCTION__,cIpInfx);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
				if(shRet==OK__)
				{
					f1 = subString(dFechaIniPension, 3, 2);
					f2 = subString(dFechaIniPension, 0, 2 );
					f3 = subString(dFechaIniPension, 6, 4);
					
					snprintf(auxFechaPension, sizeof(auxFechaPension), "MDY(%s,%s,%s)",f1, f2, f3 );
					
					CFnAportaciones xSelAportacion(&odbcIfx);
					snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_obtener_aportaciones_posteriores_rt( '%s', %s, %i);", sNss, auxFechaPension, opcion);
					
					snprintf( cTexto, sizeof(cTexto), "[%s]Se Ejecuto: %s", __FUNCTION__,cSql);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					
					if(xSelAportacion.Exec(cSql))
					{
						xSelAportacion.activarCols();
						if(xSelAportacion.leer()) 
						{
							snprintf( cTexto, sizeof(cTexto), "[%s]Se Retorno la cantidad de [%lf]  aportaciones ", __FUNCTION__,xSelAportacion.iAportaciones);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
							strAportaciones.iAportacion = xSelAportacion.iAportaciones;
						}
					}
					else
					{
						shRet = ERR_EXEC_SQL;
						xSelAportacion.odbc->GetLastError( xSelAportacion.GetHstmt() );
						snprintf( cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelAportacion.odbc->LastErrStr() );
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
			else
			{
				shRet = ERR_EXEC_SQL;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
		
		snprintf(cTexto, sizeof(cTexto), "Termina la ejecucion de la consulta");
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		snprintf(cTexto, sizeof(cTexto), "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_ADMONAFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	snprintf(cTexto, sizeof(cTexto), "[%s] Retorno con caso %i.",  __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Termina [Nss: %s], [Inicio Pension: %s]------",  __FUNCTION__, sNss,dFechaIniPension);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	return shRet;
}

short CAportaciones::abrirConexionServAfo()
{
	short shRet=DEFAULT__;
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
	}
	return shRet;
}

APORTACION_POSTERIORES *CAportaciones::Aportaciones()
{
	return &strAportaciones;
}

char* CAportaciones::subString(char* cadena, int comienzo, int longitud=0)
{
    if (longitud == 0) 
        longitud = strlen(cadena)-comienzo;
    
    char *nuevo = (char*)malloc(sizeof(char) * (longitud+1));
    nuevo[longitud] = '\0';
    strncpy(nuevo, cadena + comienzo, longitud);
    
    return nuevo;
}
