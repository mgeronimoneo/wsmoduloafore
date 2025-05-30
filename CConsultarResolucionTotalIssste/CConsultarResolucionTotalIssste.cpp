#include "CConsultarResolucionTotalIssste.hpp"
CConsultarResolucionTotalIssste::CConsultarResolucionTotalIssste()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cBuffDat, 0, sizeof(cBuffDat));
	memset(cTextoAux, 0, sizeof(cTextoAux));
	memset(cSql, 0, sizeof(cSql));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}

CConsultarResolucionTotalIssste::~CConsultarResolucionTotalIssste()
{
	odbcInformix.Close();
}
short CConsultarResolucionTotalIssste::consultaDatamart(char *cCurp, char *cRetiro)
{
	shRet = MSJ_DEFAULT__;
	int iFlagReplica = 0;

	int iPuerto = 0;
	snprintf(cTexto, sizeof(cTexto), "[%s::%s]=================IniciA=================[%s]", __FILE_MIN__, __FUNCTION__, cSql);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	// obtenerIpInformix();

	conexionPostgress();
	if (shRet == MSJ_OK__)
	{
		// Se ejecuta una consulta para verificar si la replica esta actualizada
		snprintf(cSql, sizeof(cSql), "select fnValidarReplica from fnValidarReplica()");

		shRet = CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);

		// Si la consulta devuelve un error...
		if (shRet != OK__)
		{
			//... grabamos en el log.
			snprintf(cTexto, sizeof(cTexto), "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
		if (iFlagReplica == 0)
		{
			// Creamos una instancia de la clase CConsultaDataMartIssste para la gestion del resultado de la consulta.
			CConsultaDataMartIssste xSelIssstep(&odbcPg);
			snprintf(cSql, sizeof(cSql), "select folio,nss,nssissste,curp,secpension,nombredatamart,paternodatamart,maternodatamart,numconcesion,delegacion,tipomovimiento,tiporetiro,regimen,tiposeguro,tipopension,cvepension,tipoprestacion,fechainiciopen,fecharesolucion,semanascotizadas,diagdatamart,estadoregistro from fnobtenerdatamartissste('%s', '%s')", cCurp, cRetiro);

			// Ejecutamos la consulta
			if (xSelIssstep.Exec(cSql))
			{
				// Si es exitosa preparamos pas columnas para leerlas.
				xSelIssstep.activarCols();

				// Comprobamos que podamos leer.
				if (xSelIssstep.leer())
				{
					// Colocamos las variables obtenidas en stResolucion
					// asignar valores a la estructura

					snprintf(stDMI.cNss, sizeof(stDMI.cNss), "%s", xSelIssstep.cNss);
					snprintf(stDMI.cNssissste, sizeof(stDMI.cNssissste), "%s", xSelIssstep.cNssissste);
					snprintf(stDMI.cCurp, sizeof(stDMI.cCurp), "%s", xSelIssstep.cCurp);
					snprintf(stDMI.cSecpension, sizeof(stDMI.cSecpension), "%s", xSelIssstep.cSecpension);
					snprintf(stDMI.cNombreDatamart, sizeof(stDMI.cNombreDatamart), "%s", xSelIssstep.cNombreDatamart);
					snprintf(stDMI.cPaternoDatamart, sizeof(stDMI.cPaternoDatamart), "%s", xSelIssstep.cPaternoDatamart);
					snprintf(stDMI.cMaternoDatamart, sizeof(stDMI.cMaternoDatamart), "%s", xSelIssstep.cMaternoDatamart);
					snprintf(stDMI.cNumconcesion, sizeof(stDMI.cNumconcesion), "%s", xSelIssstep.cNumconcesion);
					snprintf(stDMI.cTiporetiro, sizeof(stDMI.cTiporetiro), "%s", xSelIssstep.cTiporetiro);
					snprintf(stDMI.cRegimen, sizeof(stDMI.cRegimen), "%s", xSelIssstep.cRegimen);
					snprintf(stDMI.cTiposeguro, sizeof(stDMI.cTiposeguro), "%s", xSelIssstep.cTiposeguro);
					snprintf(stDMI.cTipopension, sizeof(stDMI.cTipopension), "%s", xSelIssstep.cTipopension);
					snprintf(stDMI.cCvepension, sizeof(stDMI.cCvepension), "%s", xSelIssstep.cCvepension);
					snprintf(stDMI.cFechainiciopen, sizeof(stDMI.cFechainiciopen), "%s", xSelIssstep.cFechainiciopen);
					snprintf(stDMI.cFecharesolucion, sizeof(stDMI.cFecharesolucion), "%s", xSelIssstep.cFecharesolucion);

					stDMI.shDelegacion = xSelIssstep.shDelegacion;
					stDMI.shTipomovimiento = xSelIssstep.shTipomovimiento;
					stDMI.shTipoprestacion = xSelIssstep.shTipoprestacion;
					stDMI.shSemanascotizadas = xSelIssstep.shSemanascotizadas;
					stDMI.shDiagDatamart = xSelIssstep.shDiagDatamart;
					stDMI.shEstadoregistro = xSelIssstep.shEstadoregistro;
					stDMI.iFolio = xSelIssstep.iFolio;
					// quitar espacios en blanco
					/*
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cNss				);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cNssissste		);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cCurp				);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cSecpension		);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cNombreDatamart	);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cPaternoDatamart	);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cMaternoDatamart	);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cNumconcesion		);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cTiporetiro		);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cRegimen			);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cTiposeguro		);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cTipopension		);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cCvepension	  	);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cFechainiciopen 	);
					CUtileriasAfo::quitarEspacioDerecha(stDMI.cFecharesolucion	);


					printf("iFolio					[%i]\n",stDMI.iFolio				);
					printf("cNss					[%s]\n",stDMI.cNss					);
					printf("cNssissste				[%s]\n",stDMI.cNssissste			);
					printf("cCurp					[%s]\n",stDMI.cCurp					);
					printf("cSecpension				[%s]\n",stDMI.cSecpension			);
					printf("cNombreDatamart			[%s]\n",stDMI.cNombreDatamart		);
					printf("cPaternoDatamart		[%s]\n",stDMI.cPaternoDatamart		);
					printf("cMaternoDatamart		[%s]\n",stDMI.cMaternoDatamart		);
					printf("cNumconcesion			[%s]\n",stDMI.cNumconcesion			);
					printf("cTiporetiro				[%s]\n",stDMI.cTiporetiro			);
					printf("cRegimen				[%s]\n",stDMI.cRegimen				);
					printf("cTiposeguro				[%s]\n",stDMI.cTiposeguro			);
					printf("cTipopension			[%s]\n",stDMI.cTipopension			);
					printf("cCvepension	  			[%s]\n",stDMI.cCvepension	  		);
					printf("cFechainiciopen 		[%s]\n",stDMI.cFechainiciopen 		);
					printf("cFecharesolucion		[%s]\n",stDMI.cFecharesolucion		);
					printf("shDelegacion			[%i]\n",stDMI.shDelegacion			);
					printf("shTipomovimiento		[%i]\n",stDMI.shTipomovimiento		);
					printf("shTipoprestacion		[%i]\n",stDMI.shTipoprestacion		);
					printf("shSemanascotizadas		[%i]\n",stDMI.shSemanascotizadas	);
					printf("shDiagDatamart			[%i]\n",stDMI.shDiagDatamart		);
					printf("shEstadoregistro		[%i]\n",stDMI.shEstadoregistro		);*/

					// Indicamos que la consulta y obtenci�n de datos fueron exitosas.
					shRet = OK__;
				}
				else
				{
					// Si no hay registros se retorna un valor indicandolo.
					shRet = ERR_NO_HAY_REG_BD;
				}
				return MSJ_OK__;
			}
			else
			{
				// Si no se realiza la consulta correctamente se graba en el log y
				// se retorna un valor que indique el error.
				shRet = ERR_EXEC_SQL;
				xSelIssstep.odbc->GetLastError(xSelIssstep.GetHstmt());
				snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelIssstep.odbc->LastErrStr());
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			if (shRet == MSJ_OK__)
			{
				conexionInformix();
			}

			if (shRet == MSJ_OK__)
			{
				// snprintf(cSql, "EXECUTE FUNCTION fn_obtenerdatosdatamartissste('CURC531120HDFRVR08','B');");
				snprintf(cSql, sizeof(cSql), "SELECT folio,nss,nss_issste,curp,sec_pension,nombre_datamart,paterno_datamart,materno_datamart,num_concesion,delegacion,tipo_movimiento,tipo_retiro,regimen,tipo_seguro,tipo_pension,cve_pension,tipo_prestacion,fecha_ini_pen,fecha_resolucion,semanas_cotizadas,diag_datamart,estado_registro FROM ret_datamart_issste WHERE curp = '%s' and tipo_retiro='%s' and sec_pension = (SELECT max(sec_pension)  FROM ret_datamart_issste where curp = '%s' )", cCurp, cRetiro, cCurp);
				snprintf(cTexto, sizeof(cTexto), "[%s::%s] Consulta DataMart : [%s]", __FILE_MIN__, __FUNCTION__, cSql);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				CConsultaDataMartIssste xSelIssste(&odbcInformix);
				// stDMI->DATOS_DATAMART_ISSSTE();
				if (xSelIssste.Exec(cSql))
				{
					xSelIssste.activarCols();

					while (xSelIssste.leer())
					{
						// asignar valores a la estructura

						snprintf(stDMI.cNss, sizeof(stDMI.cNss), "%s", xSelIssste.cNss);
						snprintf(stDMI.cNssissste, sizeof(stDMI.cNssissste), "%s", xSelIssste.cNssissste);
						snprintf(stDMI.cCurp, sizeof(stDMI.cCurp), "%s", xSelIssste.cCurp);
						snprintf(stDMI.cSecpension, sizeof(stDMI.cSecpension), "%s", xSelIssste.cSecpension);
						snprintf(stDMI.cNombreDatamart, sizeof(stDMI.cNombreDatamart), "%s", xSelIssste.cNombreDatamart);
						snprintf(stDMI.cPaternoDatamart, sizeof(stDMI.cPaternoDatamart), "%s", xSelIssste.cPaternoDatamart);
						snprintf(stDMI.cMaternoDatamart, sizeof(stDMI.cMaternoDatamart), "%s", xSelIssste.cMaternoDatamart);
						snprintf(stDMI.cNumconcesion, sizeof(stDMI.cNumconcesion), "%s", xSelIssste.cNumconcesion);
						snprintf(stDMI.cTiporetiro, sizeof(stDMI.cTiporetiro), "%s", xSelIssste.cTiporetiro);
						snprintf(stDMI.cRegimen, sizeof(stDMI.cRegimen), "%s", xSelIssste.cRegimen);
						snprintf(stDMI.cTiposeguro, sizeof(stDMI.cTiposeguro), "%s", xSelIssste.cTiposeguro);
						snprintf(stDMI.cTipopension, sizeof(stDMI.cTipopension), "%s", xSelIssste.cTipopension);
						snprintf(stDMI.cCvepension, sizeof(stDMI.cCvepension), "%s", xSelIssste.cCvepension);
						snprintf(stDMI.cFechainiciopen, sizeof(stDMI.cFechainiciopen), "%s", xSelIssste.cFechainiciopen);
						snprintf(stDMI.cFecharesolucion, sizeof(stDMI.cFecharesolucion), "%s", xSelIssste.cFecharesolucion);

						stDMI.shDelegacion = xSelIssste.shDelegacion;
						stDMI.shTipomovimiento = xSelIssste.shTipomovimiento;
						stDMI.shTipoprestacion = xSelIssste.shTipoprestacion;
						stDMI.shSemanascotizadas = xSelIssste.shSemanascotizadas;
						stDMI.shDiagDatamart = xSelIssste.shDiagDatamart;
						stDMI.shEstadoregistro = xSelIssste.shEstadoregistro;
						stDMI.iFolio = xSelIssste.iFolio;
						// quitar espacios en blanco
						/*
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cNss				);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cNssissste		);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cCurp				);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cSecpension		);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cNombreDatamart	);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cPaternoDatamart	);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cMaternoDatamart	);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cNumconcesion		);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cTiporetiro		);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cRegimen			);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cTiposeguro		);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cTipopension		);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cCvepension	  	);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cFechainiciopen 	);
						CUtileriasAfo::quitarEspacioDerecha(stDMI.cFecharesolucion	);


						printf("iFolio					[%i]\n",stDMI.iFolio				);
						printf("cNss					[%s]\n",stDMI.cNss					);
						printf("cNssissste				[%s]\n",stDMI.cNssissste			);
						printf("cCurp					[%s]\n",stDMI.cCurp					);
						printf("cSecpension				[%s]\n",stDMI.cSecpension			);
						printf("cNombreDatamart			[%s]\n",stDMI.cNombreDatamart		);
						printf("cPaternoDatamart		[%s]\n",stDMI.cPaternoDatamart		);
						printf("cMaternoDatamart		[%s]\n",stDMI.cMaternoDatamart		);
						printf("cNumconcesion			[%s]\n",stDMI.cNumconcesion			);
						printf("cTiporetiro				[%s]\n",stDMI.cTiporetiro			);
						printf("cRegimen				[%s]\n",stDMI.cRegimen				);
						printf("cTiposeguro				[%s]\n",stDMI.cTiposeguro			);
						printf("cTipopension			[%s]\n",stDMI.cTipopension			);
						printf("cCvepension	  			[%s]\n",stDMI.cCvepension	  		);
						printf("cFechainiciopen 		[%s]\n",stDMI.cFechainiciopen 		);
						printf("cFecharesolucion		[%s]\n",stDMI.cFecharesolucion		);
						printf("shDelegacion			[%i]\n",stDMI.shDelegacion			);
						printf("shTipomovimiento		[%i]\n",stDMI.shTipomovimiento		);
						printf("shTipoprestacion		[%i]\n",stDMI.shTipoprestacion		);
						printf("shSemanascotizadas		[%i]\n",stDMI.shSemanascotizadas	);
						printf("shDiagDatamart			[%i]\n",stDMI.shDiagDatamart		);
						printf("shEstadoregistro		[%i]\n",stDMI.shEstadoregistro		);*/
					}
					return MSJ_OK__;
				}
				else
				{
					// Si no se ejecuta la consulta correctamente se retorna un valor que indique el
					//  error y se graba en log.
					shRet = ERR_EXEC_SQL;
					xSelIssste.odbc->GetLastError(xSelIssste.GetHstmt());
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: [%s][%i]", __FUNCTION__, xSelIssste.odbc->LastErrStr(), shRet);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					return MSJ_DEFAULT__;
				}
			}
			else
			{
				return MSJ_DEFAULT__;
			}
		}
	}
}

DATOS_DATAMART_ISSSTE *CConsultarResolucionTotalIssste::getInformacionDataMart()
{
	return &stDMI;
}
void CConsultarResolucionTotalIssste::conexionPostgress()
{
	// Se intenta leer el archivo de configuración (.dat) que nos dara las IP.
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	// Si lo logra se prosigue.
	if (shRet == OK__)
	{
		// Se pasa la IP obtenida en el .dat a una variable para poder usarse.
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[16] = {0};
		// Se limpia de espacios la IP.
		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);

		// Se graba en log la obtención de la IP.
		snprintf(cTexto, sizeof(cTexto), "[%s] ipServiciosAfore: %s", __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		// Se intenta abrir la conexion a BD.
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		// Si falla se reintenta abrir la conexion.
		if (shRet != OK__)
		{
			// Se graba en log el primer falló al abrir la conexión a BD.
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

			// Se vacia y re-obtiene la IP considerando un fallo en la obtención del cBuff.
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			memcpy(cIpServAfo, &cBuff[20], sizeof(SIZE_BUFF_DAT - 20));
			cIpServAfo[16] = {0};
			// Se limpia de espacios la IP.
			CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);

			// Se graba en log la obtención de la IP.
			snprintf(cTexto, sizeof(cTexto), "[%s] ipServiciosAfore: %s", __FUNCTION__, cIpServAfo);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

			// Se reintenta abrir la conexión a BD.
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);

			// Si falla se graba en log.
			if (shRet != OK__)
			{
				// Se graba en log el falló definitivo al abrir la conexión a BD.
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
	}
}
void CConsultarResolucionTotalIssste::conexionInformix()
{

	// Se prepara consulta SQL.
	snprintf(cSql, sizeof(cSql), "select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	// Se limpia valor de cOutTexto.
	memset(cOutTexto, 0, sizeof(cOutTexto));
	// Se ejecuta consulta SQL para obtener la IP de Informix.
	shRet = CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);

	// Se verifica que la consulta haya sido exitosa.
	if (shRet == OK__)
	{

		// snprintf(cIpInfx, "10.44.172.235");
		// Pasamos la IP de Informix obtenida en la consulta anterior a la variable cIpInfx.
		strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
		cIpInfx[sizeof(cIpInfx) - 1] = '\0';
		// Se limpia de espacios.
		CUtileriasAfo::quitarEspacioDerecha(cIpInfx);

		// Se graba en log la obtención de la IP.
		snprintf(cTexto, sizeof(cTexto), "[%s] ipSafre: %s", __FUNCTION__, cIpInfx);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		// Se prepara y se intenta abrir la conexión a Informix.
		strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
		shRet = CBaseDato::abrirConexionInfx(&odbcInformix, cIpInfx, (char *)USR_BD_SAFRE_AF, (char *)BD_SAFRE_AF, (char *)PWD_BD_SAFRE_AF, cOutTexto);
	}
}