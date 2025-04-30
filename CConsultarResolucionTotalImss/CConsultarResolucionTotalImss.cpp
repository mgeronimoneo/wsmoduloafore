/*
 * CConsultarResolusionTotalImss.cpp
 *
 *  Creado el: 02 de Enero de 2017
 *  Autor: Jos� Antonio Corona Olivas
 */

#include "CConsultarResolucionTotalImss.hpp"

//Constructor de la clase.
CConsultarResolucionTotalImss::CConsultarResolucionTotalImss()
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

//Destructor de la clase.
CConsultarResolucionTotalImss::~CConsultarResolucionTotalImss()
{
	odbcPg.Close();
	odbcIfx.Close();
}

//Funci�n  
short CConsultarResolucionTotalImss::ConsultarResolucionTotalImss(short shOpcion, char *cNssx, char *cCurpx)
{
	//Se inicializan variables a utilizar en la funci�n.
	short shRet = DEFAULT__;

	//Grabamos en el log.
	sprintf(cTexto, "[%s] ------ Inicia [Opcion: %i Nss: %s Curp: %s]------",  __FUNCTION__, shOpcion, cNssx, cCurpx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	//Se intenta leer el archivo de configuraci�n (.dat) que nos dara las IP.
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	//Si lo logra se prosigue.
	if(shRet == OK__)
	{
		//Se pasa la IP obtenida en el .dat a una variable para poder usarse.
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[16]={0};
		//Se limpia de espacios la IP.
		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
		//Se graba en log la obtenci�n de la IP.
		sprintf(cTexto, "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		//Se intenta abrir la conexion a BD.
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		//Si falla se reintenta abrir la conexion.
		if(shRet != OK__)
		{
			//Se graba en log el primer fall� al abrir la conexi�n a BD.
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			//Se vacia y re-obtiene la IP considerando un fallo en la obtenci�n del cBuff. 
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			memcpy(cIpServAfo, &cBuff[20], SIZE_BUFF_DAT-20);
			cIpServAfo[16]={0};
			//Se limpia de espacios la IP.
			CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);

			//Se graba en log la obtenci�n de la IP.
			sprintf(cTexto, "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

			//Se reintenta abrir la conexi�n a BD.
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);

			//Si falla se graba en log.
			if(shRet != OK__)
			{
				//Se graba en log el fall� definitivo al abrir la conexi�n a BD.
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		//Si nos conectamos proseguimos.
		if(shRet == OK__)
		{
			int iFlagReplica;
			//Se ejecuta una consulta para verificar si la replica esta actualizada
			sprintf(cSql, "select fnValidarReplica from fnValidarReplica()");
			shRet=CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
			//Si la consulta devuelve un error...
			if(shRet!=OK__)
			{
				//... grabamos en el log.
				sprintf(cTexto, "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}

			//Si la validacion nos retorna 0 la replica esta completa y podemos usar las tablas.
			if( iFlagReplica == 0 )
			{
				//Creamos una instancia de la clase CFnConsultarResolucion para la gestion del resultado de la consulta.
				CFnConsultarResolucionTotalImss xSelResolTotalImss(&odbcPg);
				sprintf(cSql, "SELECT iExisteresolucion,iAfiliado,cRegimen,cTipoSeguro,cTipoPension,sTipoPrestacion,cNombreDatamart,iFolio,cSecPension,dFechaIni,dFechaPago,sSemanasCotizadas,sDiagDatamart,sEstadoSubViv,dFechaResol,cDesRegimen,cDesTipoPension,cDesTipoPresta,cDesTipoRetiro,cDesTipoSeguro,cDescEdoSubviv,iIdMovimiento FROM  fnvalidadatamartimssnuevos(%i::SMALLINT,'%s','%s');",shOpcion, cNssx, cCurpx);

				//Ejecutamos la consulta
				if(xSelResolTotalImss.Exec(cSql))
				{
					//Se preparan los registros para poder leerlos.
					xSelResolTotalImss.activarCols();
					//Verificamos que podamos leer algun registro
					if(xSelResolTotalImss.leer())
					{
						//Colocamos las variables obtenidas en stResolucion
						stResolucionTotalImss.iExisteResolucion = xSelResolTotalImss.iExisteResolucion;
						stResolucionTotalImss.iExisteAfiliado = xSelResolTotalImss.iExisteAfiliado;

						sprintf(stResolucionTotalImss.cRegimen, "%s", xSelResolTotalImss.cRegimen);
						sprintf(stResolucionTotalImss.cTipoSeguro, "%s", xSelResolTotalImss.cTipoSeguro);
						sprintf(stResolucionTotalImss.cTipoPension, "%s", xSelResolTotalImss.cTipoPension);

						stResolucionTotalImss.iTipoPrestacion = xSelResolTotalImss.iTipoPrestacion;
						sprintf(stResolucionTotalImss.cNombreDatamart, "%s", xSelResolTotalImss.cNombreDatamart);
						stResolucionTotalImss.iFolio = xSelResolTotalImss.iFolio;

						//Se prepara y graba en el log.
						sprintf(cTexto, "iTipoPrestacion BD: %i, iTipoPrestacion: %i", stResolucionTotalImss.iTipoPrestacion, xSelResolTotalImss.iTipoPrestacion);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						
						sprintf(stResolucionTotalImss.cSecPension, "%s", xSelResolTotalImss.cSecPension);
						sprintf(stResolucionTotalImss.dFechaIni, "%s", xSelResolTotalImss.dFechaIni);
						sprintf(stResolucionTotalImss.dFechaPago, "%s", xSelResolTotalImss.dFechaPago);

						stResolucionTotalImss.iSemanasCotizadas = xSelResolTotalImss.iSemanasCotizadas;
						stResolucionTotalImss.iDiagDatamart = xSelResolTotalImss.iDiagDatamart;
						stResolucionTotalImss.iEstadoSubViv = xSelResolTotalImss.iEstadoSubViv;

						sprintf(stResolucionTotalImss.dFechaResolucion, "%s", xSelResolTotalImss.dFechaResolucion);
						sprintf(stResolucionTotalImss.cDescripRegimen, "%s", xSelResolTotalImss.cDescripRegimen);
						sprintf(stResolucionTotalImss.cDescripTPension, "%s", xSelResolTotalImss.cDescripTPension);
						sprintf(stResolucionTotalImss.cDescripTPresta, "%s", xSelResolTotalImss.cDescripTPresta);
						sprintf(stResolucionTotalImss.cDescripTRetiro, "%s", xSelResolTotalImss.cDescripTRetiro);
						sprintf(stResolucionTotalImss.cDescripTSeguro, "%s", xSelResolTotalImss.cDescripTSeguro);
						sprintf(stResolucionTotalImss.cDescripEdoSubViv, "%s", xSelResolTotalImss.cDescripEdoSubViv);

						stResolucionTotalImss.iIdMovimiento = xSelResolTotalImss.iIdMovimiento;
						
						//Indicamos que la consulta y obtenci�n de datos fueron exitosas.
						shRet=OK__;
					}
					else
						//Se retorna un valor que indique que no hay registros.
						shRet=ERR_NO_HAY_REG_BD;
     			}
				else
				{
					//Si no se ejecuta la consulta correctamente se retorna un valor que indique el
					// error y se graba en log.
					shRet = ERR_EXEC_SQL;
					xSelResolTotalImss.odbc->GetLastError( xSelResolTotalImss.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelResolTotalImss.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			//Si la replica esta incompleta se consulta con safre.
			else
			{
				//M�todo para obtener los datos a traves de Safre.
				shRet=this->ConsultarResolucionTotalImssBD(shOpcion, cNssx, cCurpx);
			}
		}
	}
	else
	{
		//Si no se logra leer el archivo de configuraci�n se manda error.
		shRet = ERR_LEER_ARCHIVO_CNF;
		sprintf(cTexto, "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_SERV_AFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	//Grabamos en el log el retorno de la funci�n.
	sprintf(cTexto, "[%s] Retorno: %i",  __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	//Grabamos en el log el termino de la funci�n con los parametros utilizados.
	sprintf(cTexto, "[%s] ------ Termina [Opcion: %i Nss: %s Curp: %s]------",  __FUNCTION__, shOpcion, cNssx, cCurpx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}

//Funci�n 
short CConsultarResolucionTotalImss::ConsultarResolucionTotalImssBD(short shOpcion, char *cNssx, char *cCurpx)
{
	//Se inicializan variables.
	short shRet=DEFAULT__;
	//char cAnio[6]={0}, cMes[4]={0},cDia[4]={0};
	//Se prepara y graba en el log.
	sprintf(cTexto, "[%s] Se consulta la resolucion en Safre: Opcion: %i Nss: %s, Curp: %s",  __FUNCTION__, shOpcion, cNssx, cCurpx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	//Se prepara consulta SQL.
	sprintf(cSql,"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	//Se limpia valor de cOutTexto.
	memset(cOutTexto,0,sizeof(cOutTexto));
	//Se ejecuta consulta SQL para obtener la IP de Informix.
	shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);

	//Se verifica que la consulta haya sido exitosa.
	if(shRet==OK__)
	{
		//Pasamos la IP de Informix obtenida en la consulta anterior a la variable cIpInfx.
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx)]= '\0';
		//Se limpia de espacios.
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);

		//Se graba en log la obtenci�n de la IP.
		sprintf(cTexto, "[%s] ipSafre: %s",  __FUNCTION__, cIpInfx);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		//Se prepara y se intenta abrir la conexi�n a Informix.
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		
		//Si la conexi�n se realiza correctamente se prosigue.
		if(shRet==OK__)
		{
			//Creamos una instancia de la clase CFnConsultarResolucion para la gestion del resultado de la consulta.
			CFnConsultarResolucionTotalImss xSelResolTotalImss(&odbcIfx);

			//Se prepara la consulta.
			sprintf(cSql, "EXECUTE FUNCTION fn_obtenerdatosretdetdatamrtrt(%i, '%s', '%s');", shOpcion, cNssx, cCurpx);
			//Se ejecuta la consulta y se verifica que haya sido exitosa.
			if(xSelResolTotalImss.Exec(cSql))
			{
				//Se preparan los registros para poder leerlos.
				xSelResolTotalImss.activarCols();
				//Verificamos que podamos leer algun registro
				if(xSelResolTotalImss.leer())
				{
					//Colocamos las variables obtenidas en stResolucion
					stResolucionTotalImss.iExisteResolucion = xSelResolTotalImss.iExisteResolucion;
					stResolucionTotalImss.iExisteAfiliado = xSelResolTotalImss.iExisteAfiliado;

					sprintf(stResolucionTotalImss.cRegimen, "%s", xSelResolTotalImss.cRegimen);
					sprintf(stResolucionTotalImss.cTipoSeguro, "%s", xSelResolTotalImss.cTipoSeguro);
					sprintf(stResolucionTotalImss.cTipoPension, "%s", xSelResolTotalImss.cTipoPension);

					stResolucionTotalImss.iTipoPrestacion = xSelResolTotalImss.iTipoPrestacion;
					sprintf(stResolucionTotalImss.cNombreDatamart, "%s", xSelResolTotalImss.cNombreDatamart);
					stResolucionTotalImss.iFolio = xSelResolTotalImss.iFolio;

					//Se prepara y graba en el log.
					sprintf(cTexto, "iTipoPrestacion BD: %i, iTipoPrestacion: %i", stResolucionTotalImss.iTipoPrestacion, xSelResolTotalImss.iTipoPrestacion);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					
					sprintf(stResolucionTotalImss.cSecPension, "%s", xSelResolTotalImss.cSecPension);
					sprintf(stResolucionTotalImss.dFechaIni, "%s", xSelResolTotalImss.dFechaIni);
					sprintf(stResolucionTotalImss.dFechaPago, "%s", xSelResolTotalImss.dFechaPago);

					stResolucionTotalImss.iSemanasCotizadas = xSelResolTotalImss.iSemanasCotizadas;
					stResolucionTotalImss.iDiagDatamart = xSelResolTotalImss.iDiagDatamart;
					stResolucionTotalImss.iEstadoSubViv = xSelResolTotalImss.iEstadoSubViv;

					sprintf(stResolucionTotalImss.dFechaResolucion, "%s", xSelResolTotalImss.dFechaResolucion);
					sprintf(stResolucionTotalImss.cDescripRegimen, "%s", xSelResolTotalImss.cDescripRegimen);
					sprintf(stResolucionTotalImss.cDescripTPension, "%s", xSelResolTotalImss.cDescripTPension);
					sprintf(stResolucionTotalImss.cDescripTPresta, "%s", xSelResolTotalImss.cDescripTPresta);
					sprintf(stResolucionTotalImss.cDescripTRetiro, "%s", xSelResolTotalImss.cDescripTRetiro);
					sprintf(stResolucionTotalImss.cDescripTSeguro, "%s", xSelResolTotalImss.cDescripTSeguro);
					sprintf(stResolucionTotalImss.cDescripEdoSubViv, "%s", xSelResolTotalImss.cDescripEdoSubViv);

					stResolucionTotalImss.iIdMovimiento = xSelResolTotalImss.iIdMovimiento;
					
					//Indicamos que la consulta y obtenci�n de datos fueron exitosas.
					shRet=OK__;
				}
				else
					//Se retorna un valor que indique que no hay registros.
					shRet=ERR_NO_HAY_REG_BD;
			}
			else
			{
				//Si no se ejecuta la consulta correctamente se retorna un valor que indique el
				// error y se graba en log.
				shRet = ERR_EXEC_SQL;
				xSelResolTotalImss.odbc->GetLastError( xSelResolTotalImss.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelResolTotalImss.odbc->LastErrStr() );
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

	//Retornamos un indicador del resultado del proceso.
	return shRet;
}


//Funci�n  
short CConsultarResolucionTotalImss::ConsultarResolucionTotalImssNuevos(short shOpcion, char *cNssx, char *cCurpx ,int idocumento)
{
	//Se inicializan variables a utilizar en la funci�n.
	short shRet = DEFAULT__;
//	int iFlagReplica=-1;
//
	//Grabamos en el log.
	sprintf(cTexto, "[%s] ------ Inicia [Opcion: %i Nss: %s Curp: %s Documento: %i]------",  __FUNCTION__, shOpcion, cNssx, cCurpx, idocumento );
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	//Se intenta leer el archivo de configuraci�n (.dat) que nos dara las IP.
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	//Si lo logra se prosigue.
	if(shRet == OK__)
	{
		//Se pasa la IP obtenida en el .dat a una variable para poder usarse.
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[16]={0};
		//Se limpia de espacios la IP.
		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);

		//Se graba en log la obtenci�n de la IP.
		sprintf(cTexto, "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		//Se intenta abrir la conexion a BD.
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		//Si falla se reintenta abrir la conexion.
		if(shRet != OK__)
		{
			//Se graba en log el primer fall� al abrir la conexi�n a BD.
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			//Se vacia y re-obtiene la IP considerando un fallo en la obtenci�n del cBuff. 
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			memcpy(cIpServAfo, &cBuff[20], SIZE_BUFF_DAT-20);
			cIpServAfo[16]={0};
			//Se limpia de espacios la IP.
			CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);

			//Se graba en log la obtenci�n de la IP.
			sprintf(cTexto, "[%s] ipServiciosAfore: %s",  __FUNCTION__, cIpServAfo);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

			//Se reintenta abrir la conexi�n a BD.
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);

			//Si falla se graba en log.
			if(shRet != OK__)
			{
				//Se graba en log el fall� definitivo al abrir la conexi�n a BD.
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		//Si nos conectamos proseguimos.
		if(shRet == OK__)
		{
			int iFlagReplica;
			//Se ejecuta una consulta para verificar si la replica esta actualizada
			sprintf(cSql, "select fnValidarReplica from fnValidarReplica()");
			shRet=CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
			//Si la consulta devuelve un error...
			if(shRet!=OK__)
			{
				//... grabamos en el log.
				sprintf(cTexto, "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}

			//Si la validacion nos retorna 0 la replica esta completa y podemos usar las tablas.
			if( iFlagReplica == 0 )
			{
				//Creamos una instancia de la clase CFnConsultarResolucion para la gestion del resultado de la consulta.
				CFnConsultarResolucionTotalImss xSelResolTotalImss(&odbcPg);
				sprintf(cSql, "SELECT iExisteresolucion,iAfiliado,cRegimen,cTipoSeguro,cTipoPension,sTipoPrestacion,cNombreDatamart,iFolio,cSecPension,dFechaIni,dFechaPago,sSemanasCotizadas,sDiagDatamart,sEstadoSubViv,dFechaResol,cDesRegimen,cDesTipoPension,cDesTipoPresta,cDesTipoRetiro,cDesTipoSeguro,cDescEdoSubviv,iIdMovimiento FROM  fnvalidardatamartimssrt(%i,'%s','%s',%i::SMALLINT)",shOpcion, cNssx, cCurpx,idocumento);

				//Ejecutamos la consulta
				if(xSelResolTotalImss.Exec(cSql))
				{
					//Se preparan los registros para poder leerlos.
					xSelResolTotalImss.activarCols();
					//Verificamos que podamos leer algun registro
					if(xSelResolTotalImss.leer())
					{
						//Colocamos las variables obtenidas en stResolucion
						stResolucionTotalImss.iExisteResolucion = xSelResolTotalImss.iExisteResolucion;
						stResolucionTotalImss.iExisteAfiliado = xSelResolTotalImss.iExisteAfiliado;

						sprintf(stResolucionTotalImss.cRegimen, "%s", xSelResolTotalImss.cRegimen);
						sprintf(stResolucionTotalImss.cTipoSeguro, "%s", xSelResolTotalImss.cTipoSeguro);
						sprintf(stResolucionTotalImss.cTipoPension, "%s", xSelResolTotalImss.cTipoPension);

						stResolucionTotalImss.iTipoPrestacion = xSelResolTotalImss.iTipoPrestacion;
						sprintf(stResolucionTotalImss.cNombreDatamart, "%s", xSelResolTotalImss.cNombreDatamart);
						stResolucionTotalImss.iFolio = xSelResolTotalImss.iFolio;

						//Se prepara y graba en el log.
						sprintf(cTexto, "iTipoPrestacion BD: %i, iTipoPrestacion: %i", stResolucionTotalImss.iTipoPrestacion, xSelResolTotalImss.iTipoPrestacion);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						
						sprintf(stResolucionTotalImss.cSecPension, "%s", xSelResolTotalImss.cSecPension);
						sprintf(stResolucionTotalImss.dFechaIni, "%s", xSelResolTotalImss.dFechaIni);
						sprintf(stResolucionTotalImss.dFechaPago, "%s", xSelResolTotalImss.dFechaPago);

						stResolucionTotalImss.iSemanasCotizadas = xSelResolTotalImss.iSemanasCotizadas;
						stResolucionTotalImss.iDiagDatamart = xSelResolTotalImss.iDiagDatamart;
						stResolucionTotalImss.iEstadoSubViv = xSelResolTotalImss.iEstadoSubViv;

						sprintf(stResolucionTotalImss.dFechaResolucion, "%s", xSelResolTotalImss.dFechaResolucion);
						sprintf(stResolucionTotalImss.cDescripRegimen, "%s", xSelResolTotalImss.cDescripRegimen);
						sprintf(stResolucionTotalImss.cDescripTPension, "%s", xSelResolTotalImss.cDescripTPension);
						sprintf(stResolucionTotalImss.cDescripTPresta, "%s", xSelResolTotalImss.cDescripTPresta);
						sprintf(stResolucionTotalImss.cDescripTRetiro, "%s", xSelResolTotalImss.cDescripTRetiro);
						sprintf(stResolucionTotalImss.cDescripTSeguro, "%s", xSelResolTotalImss.cDescripTSeguro);
						sprintf(stResolucionTotalImss.cDescripEdoSubViv, "%s", xSelResolTotalImss.cDescripEdoSubViv);

						stResolucionTotalImss.iIdMovimiento = xSelResolTotalImss.iIdMovimiento;
						
						//Indicamos que la consulta y obtenci�n de datos fueron exitosas.
						shRet=OK__;
					}
					else
						//Se retorna un valor que indique que no hay registros.
						shRet=ERR_NO_HAY_REG_BD;
     			}
				else
				{
					//Si no se ejecuta la consulta correctamente se retorna un valor que indique el
					// error y se graba en log.
					shRet = ERR_EXEC_SQL;
					xSelResolTotalImss.odbc->GetLastError( xSelResolTotalImss.GetHstmt() );
					sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelResolTotalImss.odbc->LastErrStr() );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			//Si la replica esta incompleta se consulta con safre.
			else
			{
				//M�todo para obtener los datos a traves de Safre.
				shRet=this->ConsultarResolucionTotalImssBDNuevos(shOpcion, cNssx, cCurpx,idocumento);
			}
		}
	}
	else
	{
		//Si no se logra leer el archivo de configuraci�n se manda error.
		shRet = ERR_LEER_ARCHIVO_CNF;
		sprintf(cTexto, "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_SERV_AFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	//Grabamos en el log el retorno de la funci�n.
	sprintf(cTexto, "[%s] Retorno: %i",  __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	//Grabamos en el log el termino de la funci�n con los parametros utilizados.
	sprintf(cTexto, "[%s] ------ Termina [Opcion: %i Nss: %s Curp: %s]------",  __FUNCTION__, shOpcion, cNssx, cCurpx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}

//Funci�n 
short CConsultarResolucionTotalImss::ConsultarResolucionTotalImssBDNuevos(short shOpcion, char *cNssx, char *cCurpx,int idocumento)
{
	//Se inicializan variables.
	short shRet=DEFAULT__;
	//char cAnio[6]={0}, cMes[4]={0},cDia[4]={0};
	//Se prepara y graba en el log.
	sprintf(cTexto, "[%s] Se consulta la resolucion en Safre: Opcion: %i Nss: %s, Curp: %s, Documento: %i",  __FUNCTION__, shOpcion, cNssx, cCurpx,idocumento);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	
	//Se prepara consulta SQL.
	sprintf(cSql,"select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	//Se limpia valor de cOutTexto.
	memset(cOutTexto,0,sizeof(cOutTexto));
	//Se ejecuta consulta SQL para obtener la IP de Informix.
	shRet=CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);

	//Se verifica que la consulta haya sido exitosa.
	if(shRet==OK__)
	{
		//Pasamos la IP de Informix obtenida en la consulta anterior a la variable cIpInfx.
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx)]= '\0';
		//Se limpia de espacios.
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);

		//Se graba en log la obtenci�n de la IP.
		sprintf(cTexto, "[%s] ipSafre: %s",  __FUNCTION__, cIpInfx);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		//Se prepara y se intenta abrir la conexi�n a Informix.
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
		
		//Si la conexi�n se realiza correctamente se prosigue.
		if(shRet==OK__)
		{
			//Creamos una instancia de la clase CFnConsultarResolucion para la gestion del resultado de la consulta.
			CFnConsultarResolucionTotalImss xSelResolTotalImss(&odbcIfx);

			//Se prepara la consulta.
			sprintf(cSql, "EXECUTE FUNCTION fn_validaciones_datamart_imssrt(%i, '%s', '%s',%i);", shOpcion, cNssx, cCurpx,idocumento);
			//Se ejecuta la consulta y se verifica que haya sido exitosa.
			if(xSelResolTotalImss.Exec(cSql))
			{
				//Se preparan los registros para poder leerlos.
				xSelResolTotalImss.activarCols();
				//Verificamos que podamos leer algun registro
				if(xSelResolTotalImss.leer())
				{
					//Colocamos las variables obtenidas en stResolucion
					stResolucionTotalImss.iExisteResolucion = xSelResolTotalImss.iExisteResolucion;
					stResolucionTotalImss.iExisteAfiliado = xSelResolTotalImss.iExisteAfiliado;

					sprintf(stResolucionTotalImss.cRegimen, "%s", xSelResolTotalImss.cRegimen);
					sprintf(stResolucionTotalImss.cTipoSeguro, "%s", xSelResolTotalImss.cTipoSeguro);
					sprintf(stResolucionTotalImss.cTipoPension, "%s", xSelResolTotalImss.cTipoPension);


					stResolucionTotalImss.iTipoPrestacion = xSelResolTotalImss.iTipoPrestacion;
					sprintf(stResolucionTotalImss.cNombreDatamart, "%s", xSelResolTotalImss.cNombreDatamart);
					stResolucionTotalImss.iFolio = xSelResolTotalImss.iFolio;

					//Se prepara y graba en el log.
					sprintf(cTexto, "iTipoPrestacion BD: %i, iTipoPrestacion: %i", stResolucionTotalImss.iTipoPrestacion, xSelResolTotalImss.iTipoPrestacion);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					
					sprintf(stResolucionTotalImss.cSecPension, "%s", xSelResolTotalImss.cSecPension);
					sprintf(stResolucionTotalImss.dFechaIni, "%s", xSelResolTotalImss.dFechaIni);
					sprintf(stResolucionTotalImss.dFechaPago, "%s", xSelResolTotalImss.dFechaPago);

					stResolucionTotalImss.iSemanasCotizadas = xSelResolTotalImss.iSemanasCotizadas;
					stResolucionTotalImss.iDiagDatamart = xSelResolTotalImss.iDiagDatamart;
					stResolucionTotalImss.iEstadoSubViv = xSelResolTotalImss.iEstadoSubViv;

					sprintf(stResolucionTotalImss.dFechaResolucion, "%s", xSelResolTotalImss.dFechaResolucion);
					sprintf(stResolucionTotalImss.cDescripRegimen, "%s", xSelResolTotalImss.cDescripRegimen);
					sprintf(stResolucionTotalImss.cDescripTPension, "%s", xSelResolTotalImss.cDescripTPension);
					sprintf(stResolucionTotalImss.cDescripTPresta, "%s", xSelResolTotalImss.cDescripTPresta);
					sprintf(stResolucionTotalImss.cDescripTRetiro, "%s", xSelResolTotalImss.cDescripTRetiro);
					sprintf(stResolucionTotalImss.cDescripTSeguro, "%s", xSelResolTotalImss.cDescripTSeguro);
					sprintf(stResolucionTotalImss.cDescripEdoSubViv, "%s", xSelResolTotalImss.cDescripEdoSubViv);

					stResolucionTotalImss.iIdMovimiento = xSelResolTotalImss.iIdMovimiento;
					
					//Indicamos que la consulta y obtenci�n de datos fueron exitosas.
					shRet=OK__;
				}
				else
					//Se retorna un valor que indique que no hay registros.
					shRet=ERR_NO_HAY_REG_BD;
			}
			else
			{
				//Si no se ejecuta la consulta correctamente se retorna un valor que indique el
				// error y se graba en log.
				shRet = ERR_EXEC_SQL;
				xSelResolTotalImss.odbc->GetLastError( xSelResolTotalImss.GetHstmt() );
				sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelResolTotalImss.odbc->LastErrStr() );
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

	//Retornamos un indicador del resultado del proceso.
	return shRet;
}

//Funci�n para retornar los datos obtenidos en instancia de la estructura stResolucionTotalImss.
CONSULTA_RESOLUCION_TOTAL_IMSS* CConsultarResolucionTotalImss::ResolucionTotalImss()
{
	return &stResolucionTotalImss;
}