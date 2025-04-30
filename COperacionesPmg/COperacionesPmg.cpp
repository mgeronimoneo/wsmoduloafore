/*
	Fecha: 12-01-2017
	Empleado: Andres Guadalupe Figueroa Rodriguez
*/
#include "COperacionesPmg.hpp"

COperacionesPmg::COperacionesPmg()
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
	memset(cSecuencia, 0, sizeof(cSecuencia));
	sprintf(cRutaLog, "%s", RUTA_LOGX);
}

COperacionesPmg::~COperacionesPmg()
{
	odbcPg.Close();
	odbcIfx.Close();
}

short COperacionesPmg::ConsultaInsuficiencia(char *cNssx, char *cSecuenciax, int lFolioDatamart, int iOpcion)
{
	short shRet = DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cSecuencia, "%s", cSecuenciax);
	sprintf(cTexto, "[%s] ------ Inicia [Nss: %s], [Secuencia: %s], [Folio Datamart: %i], [Opcion:  %i]------",  __FUNCTION__, cNss,cSecuencia,lFolioDatamart, iOpcion);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		memcpy(cIpAdmon, cBuff, sizeof(cIpAdmon));
		cIpAdmon[sizeof(cIpAdmon)] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		
		if (iOpcion == 1) {
			//Se llama al metodo que realiza la consulta a serviciosafore
			shRet = this->ConsultaInsuficienciaSaldo(cNss, cSecuencia, lFolioDatamart);
			if(shRet != OK__) {
				shRet = this->ConsultaInsuficienciaSaldoInf(cNss, cSecuencia, lFolioDatamart);
			}
		}
		else if (iOpcion == 2) {
			shRet= this->ValidarSolicitudRetiro(cNss);
			if(shRet != OK__) {
				shRet= this->ValidarSolicitudRetiroInf(cNss);
			}
		}
		else if ( iOpcion == 3 ) {
			shRet= this->ObtenerPagoMensual();
			if(shRet != OK__) {
				shRet= this->ObtenerPagoMensualInfx();
			}
		}
		/*else if ( iOpcion == 4 ) {
			shRet= this->ObtenerSaldoPgs(cNss, cSecuencia);
			if(shRet != OK__) {
				shRet= this->ObtenerSaldoInfx(cNss, cSecuencia);
			}
		}*/
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

OPERACIONES_PMG_IMSS *COperacionesPmg::OperacionesPmg()
{
	return &OperacionesPmgImss;
}

short COperacionesPmg::ObtenerPagoMensual()
{
	short shRet=DEFAULT__;
	odbcPg.Close();
	sprintf(cTexto, "[%s] ------ Inicia obtener pago mensual ------",  __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	shRet=this->abrirConexionServAfo();
	if(shRet==OK__) 
	{
		COperacionesPmgImss xSelMonto(&odbcPg);
		sprintf(cSql, "SELECT Trim(Mensaje) AS Mensaje, Codigo, MontoPmg FROM fnobtenerpagomensualpmg()");
		
		if(xSelMonto.Exec(cSql))
		{
			xSelMonto.activarCols();
			if(xSelMonto.leer()) 
			{
				sprintf(OperacionesPmgImss.cMensaje	,"%s", xSelMonto.cMensaje);							
				OperacionesPmgImss.sCodigo	 = xSelMonto.sCodigo;
				OperacionesPmgImss.dImporte	 = xSelMonto.dMonto;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelMonto.odbc->GetLastError( xSelMonto.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelMonto.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			xSelMonto.odbc->GetLastError( xSelMonto.GetHstmt() );
			sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelMonto.odbc->LastErrStr() );
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	sprintf(cTexto, "[%s] Termina la ejecucion de la consulta", __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	return shRet;
}

short COperacionesPmg::ObtenerPagoMensualInfx()
{
	short shRet=DEFAULT__;
	sprintf(cTexto, "[%s] ------ Inicia obtener pago mensual ------",  __FUNCTION__);
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
			cIpInfx[sizeof(cIpInfx) - 1] = '\0';
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
			shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
			if(shRet==OK__)
			{
				
				COperacionesPmgImss xSelMonto(&odbcIfx);
				sprintf(cSql, "EXECUTE FUNCTION fn_obtenerpagomensualpmg();");
				if(xSelMonto.Exec(cSql))
				{
					xSelMonto.activarCols();
					if(xSelMonto.leer()) 
					{
						sprintf(OperacionesPmgImss.cMensaje	,"%s", xSelMonto.cMensaje);							
						OperacionesPmgImss.sCodigo	 = xSelMonto.sCodigo;
						OperacionesPmgImss.dImporte	 = xSelMonto.dMonto;
					}
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelMonto.odbc->GetLastError( xSelMonto.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelMonto.odbc->LastErrStr() );
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
		else
		{
			shRet = ERR_EXEC_SQL;
			sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	else //Else de la conexion a serviciosafore
	{
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	
	sprintf(cTexto, "Termina la ejecucion de la consulta");
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	return shRet;
}

short COperacionesPmg::ConsultaInsuficienciaSaldo(char *cNssx, char *cSecuenciax, int lFolioDatamart)
{
	short shRet=DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cSecuencia, "%s", cSecuenciax);
	sprintf(cTexto, "[%s] ------ Insuficiencia de saldo ------ ",  __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	odbcPg.Close();
	shRet=this->abrirConexionServAfo();
	if(shRet==OK__) 
	{
		//Realizamos la consulta a servicios afore para saber si existen registros
		COperacionesPmgImss xSelSaldo(&odbcPg);
		sprintf(cSql, "SELECT TRIM(Mensaje) AS Mensaje, Codigo, MontoPmg FROM fnconsultarsaldopmg('%s','%s',%i)", cNss, cSecuencia, lFolioDatamart);
		if(xSelSaldo.Exec(cSql))
		{
			xSelSaldo.activarCols();
			if(xSelSaldo.leer())
			{
				sprintf(OperacionesPmgImss.cMensaje	,"%s", xSelSaldo.cMensaje);							
				OperacionesPmgImss.sCodigo	 = xSelSaldo.sCodigo;
				OperacionesPmgImss.dImporte	 = xSelSaldo.dMonto;				
			}
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			xSelSaldo.odbc->GetLastError( xSelSaldo.GetHstmt() );
			sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelSaldo.odbc->LastErrStr() );
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	} 
	else
	{
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	
	sprintf(cTexto, "[%s] Termina la ejecucion de la consulta", __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	return shRet;
}

short COperacionesPmg::ConsultaInsuficienciaSaldoInf(char *cNssx, char *cSecuenciax, int lFolioDatamart)
{
	short shRet=DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cSecuencia, "%s", cSecuenciax);
	sprintf(cTexto, "[%s] ------ Insuficiencia de saldo ------ ",  __FUNCTION__);
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
			strcpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
			cIpInfx[sizeof(cIpInfx) - 1] = '\0';
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
			shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
			if(shRet==OK__)
			{
				
				COperacionesPmgImss xSelSaldo(&odbcIfx);
				sprintf(cSql, "EXECUTE FUNCTION fn_Consultar_Saldo_Pmg('%s','%s',%i)", cNss, cSecuencia, lFolioDatamart);
				if(xSelSaldo.Exec(cSql))
				{
					xSelSaldo.activarCols();
					if(xSelSaldo.leer()) 
					{
						sprintf(OperacionesPmgImss.cMensaje	,"%s", xSelSaldo.cMensaje);							
						OperacionesPmgImss.sCodigo	 = xSelSaldo.sCodigo;
						OperacionesPmgImss.dImporte	 = xSelSaldo.dMonto;
					}
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelSaldo.odbc->GetLastError( xSelSaldo.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelSaldo.odbc->LastErrStr() );
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
		else
		{
			shRet = ERR_EXEC_SQL;
			sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	} 
	else
	{
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	
	sprintf(cTexto, "[%s] Termina la ejecucion de la consulta", __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	return shRet;
}

short COperacionesPmg::ValidarSolicitudRetiro(char *cNssx)
{
	short shRet=DEFAULT__;
	sprintf(cTexto, "[%s] ------ Solicitud Retiro ------ ",  __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	odbcPg.Close();
	shRet=this->abrirConexionServAfo();
	if(shRet==OK__) 
	{
		COperacionesPmgImss xSelOperacion(&odbcPg);
		sprintf(cSql, "SELECT TRIM(Mensaje), Codigo, MontoPmg FROM fnvalidarsolicitudretiropmgimss('%s')", cNssx);
		if(xSelOperacion.Exec(cSql))
		{
			xSelOperacion.activarCols();
			if(xSelOperacion.leer()) 
			{
				sprintf(OperacionesPmgImss.cMensaje	,"%s", xSelOperacion.cMensaje);
				OperacionesPmgImss.sCodigo = xSelOperacion.sCodigo;
				OperacionesPmgImss.dImporte	 = xSelOperacion.dMonto;
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelOperacion.odbc->GetLastError( xSelOperacion.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelOperacion.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			xSelOperacion.odbc->GetLastError( xSelOperacion.GetHstmt() );
			sprintf( cTexto, "[%s] [ServiciosAfore] Err: %s", __FUNCTION__, xSelOperacion.odbc->LastErrStr() );
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	return shRet;
}

short COperacionesPmg::ValidarSolicitudRetiroInf(char *cNssx)
{
	short shRet=DEFAULT__;
	sprintf(cTexto, "[%s] ------ Solicitud Retiro ------ ",  __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	odbcPg.Close();
	
	shRet=this->abrirConexionServAfo();
	if (shRet==OK__)
	{
		sprintf(cSql,"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
		
		memset(cOutTexto,0,sizeof(cOutTexto));
		shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
		
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx) - 1] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		if(shRet==OK__)
		{
			
			COperacionesPmgImss xSelOperacionInf(&odbcIfx);
			sprintf(cSql, "EXECUTE FUNCTION fn_validar_solicitud_retiro_pmgimss('%s')", cNssx);
			if(xSelOperacionInf.Exec(cSql))
			{
				xSelOperacionInf.activarCols();
				if(xSelOperacionInf.leer()) 
				{
					sprintf(OperacionesPmgImss.cMensaje	,"%s", xSelOperacionInf.cMensaje);
					OperacionesPmgImss.sCodigo = xSelOperacionInf.sCodigo;
					OperacionesPmgImss.dImporte	 = xSelOperacionInf.dMonto;
				}
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelOperacionInf.odbc->GetLastError( xSelOperacionInf.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelOperacionInf.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_CNX_BASE_DATO;
			sprintf(cTexto, "[%s] Error al abrir cnx[Informix][%s]: %s",  __FUNCTION__, cIpInfx, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	return shRet;
}

short COperacionesPmg::abrirConexionServAfo()
{
	short shRet=DEFAULT__;
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
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
	return shRet;
}

//418i
short COperacionesPmg::buscarSaldoaRecuperar(char *cNssx)
{
	short shRet=DEFAULT__;
	sprintf(cTexto, "[%s] ------ Saldos a Recuperar ------ ",  __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	odbcPg.Close();
	
	shRet=this->abrirConexionServAfo();
	if (shRet==OK__)
	{
		sprintf(cSql,"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
		memset(cOutTexto,0,sizeof(cOutTexto));
		shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
		
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx)] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		if(shRet==OK__)
		{
			
			CConsultaInt xSelOperacionInf(&odbcIfx);
			sprintf(cSql, "EXECUTE FUNCTION fn_buscarSaldoaRecuperar('%s')", cNssx);
			CUtileriasAfo::grabarLogx(cRutaLog, cSql);
			if(xSelOperacionInf.Exec(cSql))
			{
				xSelOperacionInf.activarCols();
				
				if(xSelOperacionInf.leer()) 
				{
					iRespuestaSaldo = xSelOperacionInf.campoint;
					sprintf(cTexto,"Resultado: %i" , iRespuestaSaldo);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					shRet=OK__;
				}
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelOperacionInf.odbc->GetLastError( xSelOperacionInf.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelOperacionInf.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_CNX_BASE_DATO;
			sprintf(cTexto, "[%s] Error al abrir cnx[Informix][%s]: %s",  __FUNCTION__, cIpInfx, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	return shRet;
}

short COperacionesPmg::obtenerNumMensualidad(char *cNssx)
{
	short shRet = DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cTexto, "[%s] ------ Inicia [Nss: %s]------",  __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		memcpy(cIpAdmon, cBuff, sizeof(cIpAdmon));
		cIpAdmon[sizeof(cIpAdmon)] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
		//Se llama al metodo que realiza la consulta a serviciosafore
		shRet = this->ConsultaMensualidadPost(cNss);
		sprintf(cTexto, "SHRET antes del if: %i",  shRet);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		if(shRet != OK__) 
		{
			sprintf(cTexto, "SHRET del if: %i",  shRet);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = this->ConsultaMensualidadInf(cNss);
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

short COperacionesPmg::ConsultaMensualidadPost(char *cNssx)
{
	short shRet=DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cTexto, "[%s] ------ Numero de Mensualidad ------ ",  __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	odbcPg.Close();
	shRet=this->abrirConexionServAfo();
	if(shRet==OK__) 
	{
		//Realizamos la consulta a servicios afore para saber si existen registros
		CConsultaInt xSelMensualidad(&odbcPg);
		sprintf(cSql, "SELECT fnObtenerMensualidadActual as mensualidad from fnObtenerMensualidadActual('%s')", cNss);
		CUtileriasAfo::grabarLogx(cRutaLog, cSql);
		if(xSelMensualidad.Exec(cSql))
		{
			xSelMensualidad.activarCols();
			
			if(xSelMensualidad.leer())
			{
				iRespuestaMensualidad = xSelMensualidad.campoint;
				sprintf(cTexto,"Resultado: %i" , iRespuestaMensualidad);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				if(iRespuestaMensualidad == 1)
				{
					shRet = DEFAULT__;
					sprintf(cTexto,"SHRET IF: %i" , shRet);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelMensualidad.odbc->GetLastError( xSelMensualidad.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelMensualidad.odbc->LastErrStr() );
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			xSelMensualidad.odbc->GetLastError( xSelMensualidad.GetHstmt() );
			sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelMensualidad.odbc->LastErrStr() );
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	} 
	else
	{
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	
	sprintf(cTexto, "[%s] Termina la ejecucion de la consulta", __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	return shRet;
}

short COperacionesPmg::ConsultaMensualidadInf(char *cNssx)
{
	short shRet=DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cTexto, "[%s] ------ Numero de Mensualidad ------ ",  __FUNCTION__);
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
			cIpInfx[sizeof(cIpInfx)] = '\0';
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
			shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
			if(shRet==OK__)
			{
				
				CConsultaInt xSelMensualidadInf(&odbcIfx);
				sprintf(cSql, "EXECUTE FUNCTION fn_obtener_mensualidad_actual('%s')", cNss);
				CUtileriasAfo::grabarLogx(cRutaLog, cSql);
				if(xSelMensualidadInf.Exec(cSql))
				{
					xSelMensualidadInf.activarCols();
					if(xSelMensualidadInf.leer()) 
					{
						iRespuestaMensualidad = xSelMensualidadInf.campoint;
						sprintf(cTexto,"Resultado: %i" , iRespuestaMensualidad);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						shRet=OK__;
					}
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelMensualidadInf.odbc->GetLastError( xSelMensualidadInf.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelMensualidadInf.odbc->LastErrStr() );
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
		else
		{
			shRet = ERR_EXEC_SQL;
			sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	} 
	else
	{
		shRet = ERR_EXEC_SQL;
		sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	
	sprintf(cTexto, "[%s] Termina la ejecucion de la consulta", __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	return shRet;
}

int COperacionesPmg::RespuestaSaldo()
{
	return iRespuestaSaldo;
}

int COperacionesPmg::RespuestaMensualidad()
{
	return iRespuestaMensualidad;
}
