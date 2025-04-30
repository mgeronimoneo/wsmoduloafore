/*
 * CConsultaAportacionesPosteriores.cpp
 */

#include "CConsultaAportacionesPosteriores.hpp"

//Constructor de la clase.
CConsultaAportacionesPosteriores::CConsultaAportacionesPosteriores()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cIpAdmon, 0, sizeof(cIpAdmon));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	sprintf(cRutaLog, "%s", RUTA_LOGX);
}

//Destructor de la clase.
CConsultaAportacionesPosteriores::~CConsultaAportacionesPosteriores()
{
	odbcPg.Close();
	odbcIfx.Close();
}

//Funcion 
int CConsultaAportacionesPosteriores::CConsultarAportacionesPosterioresBD(short shOpcion, char *cNssx, char *cFechaIniPension, short shGrupo)
{
	//Se inicializan variables.
	short shRet=DEFAULT__;
	int iFlagReplica;
	int iIdBinario;
	
	//Se prepara y graba en el log.
	sprintf(cTexto, "[%s] ------ Inicia consulta de Aportaciones Posteriores en PostgreSQL ------ [Opcion: %i Nss: %s, FechaInicioPension: %s, Grupo: %i]",  __FUNCTION__, shOpcion, cNssx, cFechaIniPension, shGrupo);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	//Se abre la conexion a ServiciosAfore.
	shRet=this->abrirConexionServAfo();
	
	if(shRet==OK__) 
	{
		//Se prepara consulta SQL.
		iIdBinario = 1;
		sprintf(cSql, "SELECT fnvalidacionreplicas FROM fnvalidacionreplicas(1, %i);", iIdBinario);
		sprintf(cTexto, "[%s] ------ Se validan las replicas ------ [SQL: %s]",  __FUNCTION__, cSql);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		
		//Se ejecuta una consulta para verificar si la replica esta actualizada
		shRet = CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
		sprintf(cTexto, "[%s] ------ La validacion retorno: %i y FlagReplica: %i------ ",  __FUNCTION__, shRet, iFlagReplica);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		
		//Si la consulta devuelve un error...
		if(shRet == OK__)
		{
			//Se graba en log lo que retorno iFlagReplica.
			sprintf(cTexto, "[%s] El valor de iFlagReplica: %i",  __FUNCTION__, iFlagReplica);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

			//Si la validacion nos retorna 0 la replica esta completa y podemos usar las tablas.
			if( iFlagReplica == 0 )
			{
				//Creamos una instancia de la clase CFnConsultarResolucion para la gestion del resultado de la consulta.
				CFnConsultarAportacionesPosteriores xSelAportPost(&odbcPg);

				//Se prepara la consulta...
				sprintf(cSql, "SELECT cmontoaportacionposterior, cmensaje FROM fnAfopObtieneApPosteriores(%i::SMALLINT, '%s',  TO_DATE('%s', 'MM-DD-YYYY'), %i::SMALLINT);", shOpcion, cNssx, cFechaIniPension, shGrupo);
				//...y se graba en el log.
				sprintf(cTexto, "Se consultan las aportaciones con la siguiente funcion: %s", cSql);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				
				//Se ejecuta la consulta y se verifica que haya sido exitosa.
				if(xSelAportPost.Exec(cSql))
				{
					//Se preparan los registros para poder leerlos.
					xSelAportPost.activarCols();
					
					//Verificamos que podamos leer algun registro
					if(xSelAportPost.leer())
					{
						sprintf(cTexto, "Se obtuvieron AportacionesPosteriores: %lf", xSelAportPost.montoaportacionposterior);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						
						//Colocamos las variables obtenidas en stResolucion
						stAportacionesPosteriores.iAportacion = xSelAportPost.montoaportacionposterior;
						
						if (stAportacionesPosteriores.iAportacion > 0)
						{
							//Indicamos que la consulta y obtencion de datos fueron exitosas.
							shRet=OK__;
						}
						else
						{
							shRet=ERR_NO_HAY_REG_BD;
						}
					}
					else
					{
						//Se retorna un valor que indique que no hay registros.
						shRet=ERR_NO_HAY_REG_BD;
					}
				}
				else
				{
					//Si no se ejecuta la consulta correctamente se retorna un valor que indique el
					// error y se graba en log.
					shRet = ERR_EXEC_SQL;
					xSelAportPost.odbc->GetLastError( xSelAportPost.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelAportPost.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			//Si la replica esta incompleta se consulta con safre.
			else
			{
				//Metodo para obtener los datos a traves de Safre.
				shRet = this->CConsultarAportacionesPosteriores(shOpcion, cNssx, cFechaIniPension, shGrupo);
			}
		}
		else
		{
			//... grabamos en el log.
			sprintf(cTexto, "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	else
	{
		//Si la consulta regresa un error general se retorna un valor que indique
		//el error y se guarda en el log.
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	
	sprintf(cTexto, "Termina la ejecucion de la consulta de aportaciones posteriores");
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	//Retornamos un indicador del resultado del proceso.
	return shRet;
}

//Funcion 
int CConsultaAportacionesPosteriores::CConsultarAportacionesPosteriores(short shOpcion, char *cNssx, char *cFechaIniPension, short shGrupo)
{
	//Se inicializan variables.
	short shRet=DEFAULT__;
	
	//Se prepara y graba en el log.
	sprintf(cTexto, "[%s] ------ Inicia consulta de Aportaciones Posteriores en Safre ------ [Opcion: %i Nss: %s, FechaInicioPension: %s, Grupo: %i]",  __FUNCTION__, shOpcion, cNssx, cFechaIniPension, shGrupo);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	//Se prepara consulta SQL.
	sprintf(cSql,"SELECT valor FROM controlwsmoduloafore WHERE idcontrol=%i", CTRL_SVR_INFORMIX);
	
	//Se limpia valor de cOutTexto.
	memset(cOutTexto,0,sizeof(cOutTexto));
	
	//Se ejecuta consulta SQL para obtener la IP de Informix.
	shRet = CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);

	//Se verifica que la consulta haya sido exitosa.
	if(shRet==OK__)
	{
		//Pasamos la IP de Informix obtenida en la consulta anterior a la variable cIpInfx.
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx));
		cIpInfx[sizeof(cIpInfx)] = '\0';
		//Se limpia de espacios.
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);

		//Se graba en log la obtenci�n de la IP.
		sprintf(cTexto, "[%s] ipSafre: %s",  __FUNCTION__, cIpInfx);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		//Se prepara y se intenta abrir la conexi�n a Informix.
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet = CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		
		//Si la conexi�n se realiza correctamente se prosigue.
		if(shRet==OK__)
		{
			//Creamos una instancia de la clase CFnConsultarResolucion para la gestion del resultado de la consulta.
			CFnConsultarAportacionesPosteriores xSelAporPostIfx(&odbcIfx);

			//Se prepara la consulta.
			sprintf(cSql, "EXECUTE FUNCTION fn_afop_obtiene_ap_posteriores(%i, '%s', '%s', %i);", shOpcion, cNssx, cFechaIniPension, shGrupo);
			sprintf(cTexto, "Se consultan las aportaciones con la siguiente funcion: %s", cSql);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			
			//Se ejecuta la consulta y se verifica que haya sido exitosa.
			if(xSelAporPostIfx.Exec(cSql))
			{
				//Se preparan los registros para poder leerlos.
				xSelAporPostIfx.activarCols();
				
				//Verificamos que podamos leer algun registro
				if(xSelAporPostIfx.leer())
				{
					sprintf(cTexto, "Se obtuvieron AportacionesPosteriores: %lf", xSelAporPostIfx.montoaportacionposterior);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					//Colocamos las variables obtenidas en stResolucion
					stAportacionesPosteriores.iAportacion	= xSelAporPostIfx.montoaportacionposterior;
					if (stAportacionesPosteriores.iAportacion > 0)
					{
						//Indicamos que la consulta y obtencion de datos fueron exitosas.
						shRet=OK__;
					}
					else
					{
						shRet=ERR_NO_HAY_REG_BD;
					}
				}
				else
				{
					//Se retorna un valor que indique que no hay registros.
					shRet=ERR_NO_HAY_REG_BD;
				}
			}
			else
			{
				//Si no se ejecuta la consulta correctamente se retorna un valor que indique el
				// error y se graba en log.
				shRet = ERR_EXEC_SQL;
				xSelAporPostIfx.odbc->GetLastError( xSelAporPostIfx.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelAporPostIfx.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			//Si no se conecta a Informix se retorna un valor que indique el error y se graba en log.
			shRet = ERR_CNX_BASE_DATO;
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpInfx, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	else if(shRet==ERR_NO_HAY_REG_BD)
	{
		//Si la consulta regresa un error por falta de registros se retorna un valor que indique
		//el error y grabamos en el log.
		sprintf(cTexto, "[%s] Error: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	else
	{
		//Si la consulta regresa un error general se retorna un valor que indique
		//el error y se guarda en el log.
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	
	sprintf(cTexto, "Termina la ejecucion de la consulta de aportaciones posteriores");
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	//Retornamos un indicador del resultado del proceso.
	return shRet;
}

//Funcion para retornar los datos obtenidos en instancia de la estructura stAportacionesPosteriores.
CONSULTA_APORTACIONES_POSTERIORES* CConsultaAportacionesPosteriores::AportacionesPosteriores()
{
	return &stAportacionesPosteriores;
}

int CConsultaAportacionesPosteriores::abrirConexionServAfo()
{
	//Se inicializan variables.
	short shRet=DEFAULT__;
	
	//Se prepara y graba en el log.
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	
	//Se intenta leer el archivo de configuracion (.dat) que nos dara las IP.
	if(shRet == OK__)
	{
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[sizeof(cIpServAfo)] = '\0';
		
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
			cIpServAfo[sizeof(cIpServAfo)] = '\0';
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
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		sprintf(cTexto, "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_ADMONAFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}