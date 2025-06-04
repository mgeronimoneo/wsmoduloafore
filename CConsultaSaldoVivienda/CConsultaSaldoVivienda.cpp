#include "CConsultaSaldoVivienda.hpp"
CConsultaSaldoVivienda::CConsultaSaldoVivienda()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cBuffDat, 0, sizeof(cBuffDat));
	memset(cTextoAux, 0, sizeof(cTextoAux));
	memset(cSql, 0, sizeof(cSql));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}

CConsultaSaldoVivienda::~CConsultaSaldoVivienda()
{
	odbcInformix.Close();
}
short CConsultaSaldoVivienda::consultavivienda(char *cNss, char *fecha_solicitud, char *fecha_ini_pen)
{
	shRet = MSJ_DEFAULT__;
	int iFlagReplica = 0;
	// printf("1\n");

	int iPuerto = 0;
	snprintf(cTexto, sizeof(cTexto), "[%s::%s]=================IniciA=================[%s]", __FILE_MIN__, __FUNCTION__, cSql);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	// obtenerIpInformix();
	//  printf("2\n");
	conexionPostgress();
	if (shRet == MSJ_OK__)
	{
		// Se ejecuta una consulta para verificar si la replica esta actualizada
		snprintf(cSql, sizeof(cSql), "SELECT fnvalidacionreplicas FROM fnvalidacionreplicas(1, 2);");
		printf("%s\n", cSql);
		shRet = CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
		printf("%i\n", iFlagReplica);
		// Si la consulta devuelve un error...
		if (shRet != OK__)
		{
			//... grabamos en el log.
			snprintf(cTexto, sizeof(cTexto), "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
		if (iFlagReplica == 0)
		{
			char fechasolicitud[12],
				fechainipens[12];
			memset(fechasolicitud, 0, sizeof(fechasolicitud));
			memset(fechainipens, 0, sizeof(fechainipens));
			// printf("fechasolicitud %s\n",fechasolicitud);
			// printf("fechainipens %s\n",fechainipens);

			fechasolicitud[0] = fecha_solicitud[6];
			fechasolicitud[1] = fecha_solicitud[7];
			fechasolicitud[2] = fecha_solicitud[8];
			fechasolicitud[3] = fecha_solicitud[9];
			fechasolicitud[4] = fecha_solicitud[5];
			fechasolicitud[5] = fecha_solicitud[0];
			fechasolicitud[6] = fecha_solicitud[1];
			fechasolicitud[7] = fecha_solicitud[2];
			fechasolicitud[8] = fecha_solicitud[3];
			fechasolicitud[9] = fecha_solicitud[4];
			CUtileriasAfo::quitarEspacioDerecha(fechasolicitud);
			// printf("fechasolicitud %s\n",fechasolicitud);
			fechainipens[0] = fecha_ini_pen[6];
			fechainipens[1] = fecha_ini_pen[7];
			fechainipens[2] = fecha_ini_pen[8];
			fechainipens[3] = fecha_ini_pen[9];
			fechainipens[4] = fecha_ini_pen[5];
			fechainipens[5] = fecha_ini_pen[0];
			fechainipens[6] = fecha_ini_pen[1];
			fechainipens[7] = fecha_ini_pen[2];
			fechainipens[8] = fecha_ini_pen[3];
			fechainipens[9] = fecha_ini_pen[4];
			CUtileriasAfo::quitarEspacioDerecha(fechainipens);
			// printf("fechainipens %s\n",fechainipens);

			snprintf(fecha_ini_pen, sizeof(fecha_ini_pen), "%s", fechainipens);
			snprintf(fecha_solicitud, sizeof(fecha_solicitud), "%s", fechasolicitud);

			// Creamos una instancia de la clase CConsultaDataMartIssste para la gestion del resultado de la consulta.
			CConsultadeSaldoVivienda xSelVivienda(&odbcPg);
			snprintf(cSql, sizeof(cSql), "SELECT nss,dmontoaccviv,dmontopesviv FROM fnobtenersaldovivret_e('%s', '%s', '%s');", cNss, fecha_solicitud, fecha_ini_pen);

			// printf("%s  postgresql\n",cSql);

			// Ejecutamos la consulta
			if (xSelVivienda.Exec(cSql))
			{
				// Si es exitosa preparamos pas columnas para leerlas.
				//  printf("entro en xSelVivienda.Exec postgresql\n");
				xSelVivienda.activarCols();
				// printf("activar cools postgresql\n");
				// Comprobamos que podamos leer.
				if (xSelVivienda.leer())
				{
					// Colocamos las variables obtenidas en stResolucion
					// asignar valores a la estructura

					// printf("entro en xSelVivienda.Leer  postgresql\n");

					snprintf(stDMI.sNss, sizeof(stDMI.sNss), "%s", xSelVivienda.cNss);
					// printf("%s  postgresql\n",stDMI.sNss	);
					snprintf(stDMI.dMontoaccviv, sizeof(stDMI.dMontoaccviv), "%s", xSelVivienda.dMontoaccviv);
					// printf("%s  postgresql\n",stDMI.dMontoaccviv);
					snprintf(stDMI.dMontopesviv, sizeof(stDMI.dMontopesviv), "%s", xSelVivienda.dMontopesviv);
					// printf("%s  postgresql\n",stDMI.dMontopesviv);

					return MSJ_OK__;

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
				xSelVivienda.odbc->GetLastError(xSelVivienda.GetHstmt());
				snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelVivienda.odbc->LastErrStr());
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else
		{
			printf("3\n");
			if (shRet == MSJ_OK__)
			{
				printf("antes de llegar a la conexion de Informix\n");
				conexionInformix();
				printf("paso la conexion de Informix\n");
			}

			if (shRet == MSJ_OK__)
			{
				printf("ante de llegar a la funcion\n");
				// snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_obtenerdatosdatamartissste('CURC531120HDFRVR08','B');");
				snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_afop_saldo_viv_ret_e('%s', '%s', '%s');", cNss, fecha_solicitud, fecha_ini_pen);
				snprintf(cTexto, sizeof(cTexto), "[%s::%s] Consulta Vivienda : [%s]", __FILE_MIN__, __FUNCTION__, cSql);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				printf("%s\n", cSql);
				printf("paso la funcion\n");
				CConsultadeSaldoVivienda xSelVivienda(&odbcInformix);
				printf("3.5\n");
				// stDMI->CONSULTA_SALDOS_VIVIENDA();
				if (xSelVivienda.Exec(cSql))
				{
					printf("entro en xSelVivienda.Exec\n");
					xSelVivienda.activarCols();

					while (xSelVivienda.leer())
					{
						// asignar valores a la estructura
						printf("entro en xSelVivienda.Leer\n");

						snprintf(stDMI.sNss, sizeof(stDMI.sNss), "%s", xSelVivienda.cNss);
						printf("%s\n", stDMI.sNss);
						snprintf(stDMI.dMontoaccviv, sizeof(stDMI.dMontoaccviv), "%s", xSelVivienda.dMontoaccviv);
						printf("%s\n", stDMI.dMontoaccviv);
						snprintf(stDMI.dMontopesviv, sizeof(stDMI.dMontopesviv), "%s", xSelVivienda.dMontopesviv);
						printf("%s\n", stDMI.dMontopesviv);
					}
					return MSJ_OK__;
				}
				else
				{
					printf("No entro al else de xSelVivienda.Exec\n");
					// Si no se ejecuta la consulta correctamente se retorna un valor que indique el
					//  error y se graba en log.
					shRet = ERR_EXEC_SQL;
					xSelVivienda.odbc->GetLastError(xSelVivienda.GetHstmt());
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: [%s][%i]", __FUNCTION__, xSelVivienda.odbc->LastErrStr(), shRet);
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

CONSULTA_SALDOS_VIVIENDA *CConsultaSaldoVivienda::getConsultaSaldosVivienda()
{
	// printf("entro en CONSULTA_SALDOS_VIVIENDA");
	return &stDMI;
}
void CConsultaSaldoVivienda::conexionPostgress()
{
	// printf("entro en la conexion de posgress\n");
	// Se intenta leer el archivo de configuración (.dat) que nos dara las IP.
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	// Si lo logra se prosigue.
	if (shRet == OK__)
	{
		// Se pasa la IP obtenida en el .dat a una variable para poder usarse.
		strncpy(cIpServAfo, cBuff, sizeof(cIpServAfo) - 1);
		cIpServAfo[sizeof(cIpServAfo) - 1] = '\0';

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
			strncpy(cIpServAfo, &cBuff[20], sizeof(SIZE_BUFF_DAT - 20));
			cIpServAfo[sizeof(cIpServAfo)] = '\0';
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
void CConsultaSaldoVivienda::conexionInformix()
{

	//   printf("entro a la conexion de Informix\n");
	// Se prepara consulta SQL.
	snprintf(cSql, sizeof(cSql), "select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
	// Se limpia valor de cOutTexto.
	memset(cOutTexto, 0, sizeof(cOutTexto));
	// Se ejecuta consulta SQL para obtener la IP de Informix.
	shRet = CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);

	// Se verifica que la consulta haya sido exitosa.
	if (shRet == OK__)
	{
		// printf("todo bien en la conexion\n");
		// sprintf(cIpInfx, "10.44.172.235");
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