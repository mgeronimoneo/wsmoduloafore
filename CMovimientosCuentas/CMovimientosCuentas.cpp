#include "CMovimientosCuentas.hpp"
CMovimientosCuentas::CMovimientosCuentas()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	sprintf(cRutaLog, "%s", RUTA_LOGX);
	iMovimientos=-1;
		
}
CMovimientosCuentas::~CMovimientosCuentas()
{
	odbcPg.Close();
}
short CMovimientosCuentas::ObtenerMovimientos(char* sNss)
{
	short shRet = DEFAULT__;
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[16]={0};
		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
		sprintf(cTexto, "[%s] ipAforeGlobal: %s",  __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_AFOREGLOBAL, (char *)BD_AFOREGLOBAL, cOutTexto);
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
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_AFOREGLOBAL, (char *)BD_AFOREGLOBAL, cOutTexto);
			if(shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if(shRet == OK__)
		{
			obtenerIpInformix();
			if(shRet == OK__)
			{
				abrirConexionInformix();
				if(shRet == OK__)
				{
					sprintf(cSql, "SELECT COUNT (tipo_movimiento) FROM dis_cuenta WHERE  subcuenta  in (1, 2, 3, 4 , 5,6) and nss = '%s';", sNss);
					shRet = CBaseDato::CBaseDato::consultarNumero(&odbcInformix, cSql, iMovimientos, cOutTexto);
					//consultarTexto(C_ODBC *odbc, char *cSql, char *cOutTexto)
					printf ("la variable bandera vale : %i despues de ejecutar la consulta...", shRet);
					if(shRet == OK__)
					{
						sprintf(cTexto, "[%s] Movimientos Cunsultados Para en NSS: %s Con un numro de movimientos en las cuentas 1 a 6 = %i", __FUNCTION__, sNss, iMovimientos);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					}
					else
					{
						sprintf(cTexto, "[%s] Excep. al obtener el numero de movimientos en las cuentas 1 a 6: %i %s", __FUNCTION__, shRet, cOutTexto);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					}
				}
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

	sprintf(cTexto, "[%s] ------ Termina [NSS: %s]------",  __FUNCTION__, sNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
int CMovimientosCuentas::movimientos()
{
	return iMovimientos;
}
void CMovimientosCuentas::abrirConexionInformix()
{
	short shRet = DEFAULT__;
	char cLog[ 200 ] = {0};
				   
	strcat( cIpInfx, ";SERVER=safre_tcp");
	
	//if( odbcInformix.Open("Informix", cIpInfx, "syscoppel", "545f78b743", "safre_af" ) == 1  )//Produccion
	if( odbcInformix.Open("Informix", cIpInfx, "safre", "safre123", "safre_af" ) == 1  )//Pruebas
	{
	   
	   //sprintf( cTexto, "SE ESTABLECIO LA CONEXION AL SVR INFORMIX [%s]", cIpInfx );
		sprintf(cTexto, "[%s] SE ESTABLECIO LA CONEXION AL SVR INFORMIX [%s]", __FUNCTION__, cIpInfx);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);	   
	    shRet=OK__;
	}
	else
	{
	   sprintf( cLog, "ERROR AL ABRIR CONEXION AL SVR INFORMIX [%s]", cIpInfx );
	   CUtileriasAfo::grabarLogx( cLog );
	   shRet=DEFAULT__;
	}  
	
}

void CMovimientosCuentas::obtenerIpInformix()
{
    short shRet = DEFAULT__;
    char cSqlText[ 100 ] = {0};
    
	sprintf(cSqlText, "SELECT TRIM(concepto) FROM pafhardcode WHERE tipo1 = 1 AND tipo2 = 0 AND tipo3 = 1");
	sprintf(cTexto, "[%s] Funcion : %s",  __FUNCTION__, cSqlText);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	shRet = CBaseDato::consultarTexto(&odbcPg,cSqlText,cOutTexto);	    
	if( shRet==OK__)
	{
		memset( cIpInfx, 0, sizeof(cIpInfx) );
		//sprintf( cIpInfx, "%s", cOutTexto ); 
		sprintf( cIpInfx, "%s", "10.44.172.243" ); //SIN SERVIDOR 230 Y 235
		shRet=OK__;
    }
    else
    {
		sprintf( cTexto, "[%s] ERROR AL OBTENER LA IP DEL SERVIDOR DE INFORMIX: %s",  __FUNCTION__, cOutTexto );
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet=DEFAULT__;
    }                                                                                                        
    
	
}

