#include "CConsultarAfiliado.h"
/*int main(int argc, char* argv[])
{
	char	cTexto[1024] = {0};
	CConsultarAfiliado obj;
	//obj.consultarAfiliado( argv[1], argv[2], argv[3], (short)atoi(argv[4]), argv[5] );
	obj.obtenerInformacionTrabajador(atol(argv[1]));

	DATOS_TRABAJADOR_EXP_IDEN  * st;
	st = obj.getInformacionTrabajador();

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cNss, sizeof(st->cNss));
	printf("%s\n", cTexto);

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cCurp, sizeof(st->cCurp));
	printf("%s\n", cTexto);

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cNombre, sizeof(st->cNombre));
	printf("%s\n", cTexto);

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cApellidoPaterno, sizeof(st->cApellidoPaterno));
	printf("%s\n", cTexto);

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cApellidoMaterno, sizeof(st->cApellidoMaterno));
	printf("%s\n", cTexto);

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cFechaServicio, sizeof(st->cFechaServicio));
	printf("%s\n", cTexto);

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cEntidadNacimiento, sizeof(st->cEntidadNacimiento));
	printf("%s\n", cTexto);

	memset(cTexto,0,sizeof(cTexto));
	memcpy(cTexto, &st->cFechaNacimiento, sizeof(st->cFechaNacimiento));
	printf("%s\n", cTexto);

	return 0;
}*/
CConsultarAfiliado::CConsultarAfiliado()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
	memset(cSql, 0, sizeof(cSql));
	memset(cBuffDat, 0, sizeof(cBuffDat));
	memset(cIpPg, 0, sizeof(cIpPg));
	memset(&shmInfoMaestroAfiliado, 0, sizeof(shmInfoMaestroAfiliado));
	memset(&stDatosTrabajador, 0, sizeof(stDatosTrabajador));
	shmMaestroAfiliado = NULL;
}
CConsultarAfiliado::~CConsultarAfiliado()
{
	odbcAdmonAfo.Close();
	odbcSafreAf.Close();
}
short CConsultarAfiliado::consultarAfiliado(char *cNombres, char *cPaterno, char *cMaterno, short shEstadoNac, char *cFechaNac)
{
	short shRet = DEFAULT__;
	snprintf(cTexto, sizeof(cTexto), "[%s::%s] ===== Inicia =====", __FILE__, __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cTexto, sizeof(cTexto), "[%s] Nom: %s Pat: %s Mat: %s EdoNac: %i FechaNac: %s", __FUNCTION__, cNombres, cPaterno, cMaterno, shEstadoNac, cFechaNac);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = this->obtenerConexionBaseDato();
	if (shRet == OK__)
	{
	}
	snprintf(cTexto, sizeof(cTexto), "[%s::%s] ===== Termina =====", __FILE__, __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
short CConsultarAfiliado::obtenerInformacionTrabajador(long lFolioServicio)
{
	short shRet = DEFAULT__;
	char cSql[512] = {0}, cMensaje[1024] = {0}, cNss[13] = {0}, cEstadoAux[3] = {0}, cFechaCorteMaestroAfiliado[12] = {0}, cFechaNacConSeparador[12] = {0}, cFechaNac[12] = {0}, cSep = 0;
	bool bBuscarEnBD = false;
	MAESTRO_AFILIADO *ptrMaeAfilEnc = NULL, maeAfilBuscar;

	snprintf(cTexto, sizeof(cTexto), "[%s::%s] ===== Inicia =====", __FILE__, __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	shRet = this->obtenerConexionBaseDato();
	if (shRet == OK__)
	{
		CInfoTrabajadorIni xSel(&odbcSafreAf);
		// Aqui se consulta en rec_solicitud el nss y otros datos para (Nss necesario para buscar en memoria el resto de datos del cliente)
		snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fnobtenerinformacionpreviatrabajadorei(%ld)", lFolioServicio);
		if (xSel.Exec(cSql))
		{
			xSel.activarCols();
			if (xSel.Leer())
			{
				// LLenar los pirmeros datos de la estructura que se regresara
				memset(&stDatosTrabajador, 0, sizeof(DATOS_TRABAJADOR_EXP_IDEN));
				strncpy(stDatosTrabajador.cCurp, xSel.curp, sizeof(stDatosTrabajador.cCurp));
				strncpy(stDatosTrabajador.cNss, xSel.nss, sizeof(stDatosTrabajador.cNss));
				strncpy(stDatosTrabajador.cFechaServicio, xSel.fechaservicio, sizeof(stDatosTrabajador.cFechaServicio));

				// Llenar estructura para buscar en memoria
				memset(&maeAfilBuscar, 0, sizeof(MAESTRO_AFILIADO));
				strncpy(maeAfilBuscar.cNss, xSel.nss, SIZE_NSS - 1);
				maeAfilBuscar.cNss[SIZE_NSS - 1] = '\0';
				xSel.ClearResults();
			}
			else
			{
				shRet = ERR_NO_HAY_REG_BD;
			}
		}
		else
		{
			xSel.odbc->GetLastError(xSel.GetHstmt());
			snprintf(cTexto, sizeof(cTexto), "[%s][%s]Query fallido[%s] Error[%s]", __FILE__, __FUNCTION__, cSql, xSel.odbc->LastErrStr());
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = ERR_EXEC_SQL;
		}
		if (shRet == OK__)
		{
			CUtileriasAfo::grabarLogx(cRutaLog, "Buscar datos del trabajador en memoria");
			shRet = this->obtenerFechaCorteShmMaestroAfiliado(cFechaCorteMaestroAfiliado);
			if (shRet == OK__)
			{
				CUtileriasAfo::quitarEspacioDerecha(cFechaCorteMaestroAfiliado);
				shRet = CBaseDato::consultarInfoShm(&odbcAdmonAfo, cFechaCorteMaestroAfiliado /*"2015-04-24"*/, shmInfoMaestroAfiliado, cMensaje);
				// shRet = DEFAULT__;
				if (shRet == OK__)
				{
					shmMaestroAfiliado = (MAESTRO_AFILIADO *)CUtileriasAfo::obtenerShm(shmInfoMaestroAfiliado.iIdShm, SHM_RDONLY, cMensaje);
					if (shmMaestroAfiliado != NULL)
					{

						memset(cTexto, 0, sizeof(cTexto));
						snprintf(cTexto, sizeof(cTexto), "Se acceso con exito [shmMaestroAfiliado] Total Reg: [%i] ID_SHM: [%i]", shmInfoMaestroAfiliado.iTotalReg, shmInfoMaestroAfiliado.iIdShm);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

						ptrMaeAfilEnc = (MAESTRO_AFILIADO *)bsearch(&maeAfilBuscar, (void *)shmMaestroAfiliado, shmInfoMaestroAfiliado.iTotalReg, sizeof(MAESTRO_AFILIADO), compararNssMaestroAfiliado);
						if (ptrMaeAfilEnc != NULL)
						{
							memmove(stDatosTrabajador.cApellidoPaterno, ptrMaeAfilEnc->cPaterno, sizeof(stDatosTrabajador.cApellidoPaterno));
							memmove(stDatosTrabajador.cApellidoMaterno, ptrMaeAfilEnc->cMaterno, sizeof(stDatosTrabajador.cApellidoMaterno));
							memmove(stDatosTrabajador.cNombre, ptrMaeAfilEnc->cNombres, sizeof(stDatosTrabajador.cNombre));
							snprintf(cEstadoAux, sizeof(cEstadoAux), "%02i", ptrMaeAfilEnc->shEstadoNacimiento);
							memmove(stDatosTrabajador.cEntidadNacimiento, cEstadoAux, sizeof(stDatosTrabajador.cEntidadNacimiento));
							memmove(stDatosTrabajador.cFechaNacimiento, ptrMaeAfilEnc->cFechaNac, sizeof(ptrMaeAfilEnc->cFechaNac));
							stDatosTrabajador.iSexo = (int)ptrMaeAfilEnc->shSexo;

							// Dar formato a la fecha de nacimiento , viene sin separador a 8 posiciones
							memset(cFechaNac, 0, sizeof(cFechaNac));
							memmove(cFechaNac, &stDatosTrabajador.cFechaNacimiento, sizeof(stDatosTrabajador.cFechaNacimiento));
							if (strlen(cFechaNac) == 8)
							{
								this->formatoFechaConSeparador(cFechaNac, cFechaNacConSeparador, 45); // 45 equivale al guion medio
								memmove(stDatosTrabajador.cFechaNacimiento, cFechaNacConSeparador, sizeof(stDatosTrabajador.cFechaNacimiento));
							}

							snprintf(cTexto, sizeof(cTexto), "Se encontraron datos en memoria maestro_afiliado");
							shRet = OK__;
						}
						else
						{
							snprintf(cTexto, sizeof(cTexto), "No encontraron datos en memoria maestro_afiliado");
							bBuscarEnBD = true;
						}
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						CUtileriasAfo::liberarShm(shmMaestroAfiliado);
					}
					else
					{
						bBuscarEnBD = true;
						CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);
					}
				}
				else
				{
					CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);
					bBuscarEnBD = true;
				}
			}
			else
			{
				bBuscarEnBD = true;
				CUtileriasAfo::grabarLogx(cRutaLog, "ocurrio un error al consultar la fecha de corte de memoria maestro_afiliado");
			}

			if (bBuscarEnBD)
			{
				// En caso de no encontrar informacion en memoria se ejecuta la funcion que actualmente ejecuta el expediente de identificacion
				CUtileriasAfo::grabarLogx(cRutaLog, "Buscar datos del trabajador en DB");

				memset(cSql, 0, sizeof(cSql));
				snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fnobtenerinformaciontrabajadorei(%ld);", lFolioServicio);
				CInfoTrabajadorCompleta xSelCom(&odbcSafreAf);
				if (xSelCom.Exec(cSql))
				{
					xSelCom.activarCols();
					if (xSelCom.Leer())
					{
						memmove(stDatosTrabajador.cApellidoPaterno, xSelCom.capellidopaterno, sizeof(stDatosTrabajador.cApellidoPaterno));
						memmove(stDatosTrabajador.cApellidoMaterno, xSelCom.capellidomaterno, sizeof(stDatosTrabajador.cApellidoMaterno));
						memmove(stDatosTrabajador.cNombre, xSelCom.cnombre, sizeof(stDatosTrabajador.cNombre));
						memmove(stDatosTrabajador.cEntidadNacimiento, xSelCom.centidadnacimiento, sizeof(stDatosTrabajador.cEntidadNacimiento));
						memmove(stDatosTrabajador.cFechaNacimiento, xSelCom.cfechanacimiento, sizeof(stDatosTrabajador.cFechaNacimiento));
						stDatosTrabajador.iSexo = (int)xSelCom.shsexo;
						shRet = OK__;
					}
					else
					{
						shRet = ERR_NO_HAY_REG_BD;
						CUtileriasAfo::grabarLogx(cRutaLog, "No se leyeron datos de Maestro_Afiliado en BD");
					}
				}
				else
				{
					xSelCom.odbc->GetLastError(xSelCom.GetHstmt());
					snprintf(cTexto, sizeof(cTexto), "[%s][%s]Query fallido[%s] Error[%s]", __FILE__, __FUNCTION__, cSql, xSelCom.odbc->LastErrStr());
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					shRet = ERR_EXEC_SQL;
				}
			}
		}
	}

	snprintf(cTexto, sizeof(cTexto), "[%s::%s] ===== Termina =====", __FILE__, __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
DATOS_TRABAJADOR_EXP_IDEN *CConsultarAfiliado::getInformacionTrabajador()
{
	return &stDatosTrabajador;
}

short CConsultarAfiliado::obtenerConexionBaseDato()
{
	short shRet = DEFAULT__;
	char cBuff[SIZE_BUFF_DAT] = {0},
		 cIpAdmon[17] = {0},
		 cIpSafreAf[50] = {0},
		 cTexto[200] = {0},
		 cMensaje[1024] = {0};

	snprintf(cMensaje, sizeof(cMensaje), "[%s][%s] Leyendo archivo de configuracion", __FILE__, __FUNCTION__);
	CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);

	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_ADMONAFO_DAT, cBuff, SIZE_BUFF_DAT, cTexto);

	if (shRet == OK__)
	{
		strncpy(cIpAdmon, cBuff, sizeof(cIpAdmon) - 1);
		cIpAdmon[sizeof(cIpAdmon) - 1] = '\0';
		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		// Abre conexión con el servidor

		shRet = CBaseDato::abrirConexion(&odbcAdmonAfo, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cTexto);

		if (shRet == OK__)
		{
			// Obtiene el ip del servidor de informix
			shRet = CBaseDato::consultarIpServidor(&odbcAdmonAfo, SERV_DATABASE, cIpSafreAf, cMensaje);

			if (shRet == OK__)
			{
				// Abre la conexion al servidor de informix
				snprintf(cIpSafreAf, sizeof(cIpSafreAf), ";SERVER=safre_tcp");
				shRet = CBaseDato::abrirConexionInfx(&odbcSafreAf, cIpSafreAf, USR_BD_SAFRE_AF, BD_SAFRE_AF, PWD_BD_SAFRE_AF, cMensaje);

				if (shRet == OK__)
				{
					// Graba en el log que abrio conexion
					snprintf(cMensaje, sizeof(cMensaje), "[%s][%s] Conexión a Base de Datos abierta", __FILE__, __FUNCTION__);
					CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);
				}
				else
				{
					// Graba el log en caso de no abrir conexion al servidor
					CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);
					shRet = ERR_CNX_BASE_DATO;
				}
			}
			else
			{
				// Graba el log en caso de no abrir conexion al servidor
				CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);
				shRet = ERR_CNX_BASE_DATO;
			}
		}
		else
		{
			// Graba el log en caso de no abrir conexion al servidor
			snprintf(cMensaje, sizeof(cMensaje), "[%s][%s] Error al abrir conexión Postgresql [%s]", __FILE__, __FUNCTION__, cTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);
			shRet = ERR_CNX_BASE_DATO;
		}
	}
	else
	{
		snprintf(cMensaje, sizeof(cMensaje), "[%s][%s] Error al abrir el archivo de configuracion", __FILE__, __FUNCTION__);
		CUtileriasAfo::grabarLogx(cRutaLog, cMensaje);
		shRet = ERR_LEER_ARCHIVO_CNF;
	}

	return shRet;
}
short CConsultarAfiliado::obtenerFechaCorteShmMaestroAfiliado(char *cOutFechaCorte)
{
	short shRet = DEFAULT__;
	memset(cSql, 0, sizeof(cSql));
	snprintf(cSql, sizeof(cSql), "SELECT to_char(current_date - interval '1 day','YYYYMMDD')");
	CCampoTexto xSel(&odbcAdmonAfo);

	if (xSel.Exec(cSql))
	{
		xSel.activarCols();
		if (xSel.leer())
		{
			snprintf(cOutFechaCorte, sizeof(cOutFechaCorte), "%s", xSel.texto);
			shRet = OK__;
		}
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		xSel.odbc->GetLastError(xSel.GetHstmt());
		snprintf(cTexto, sizeof(cTexto), "[%s][%s][obtenerFechaCorteShmMaestroAfiliado] Err: %s", __FILE__, __FUNCTION__, xSel.odbc->LastErrStr());
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}
void CConsultarAfiliado::formatoFechaConSeparador(char *pcFecha, char *cOutFechaMDY, char pcSeparador)
{
	char cAnio[5] = {0}, cMes[3] = {0}, cDia[3] = {0}, cFecha[17] = {0};

	memset(cAnio, 0, sizeof(cAnio));
	memset(cMes, 0, sizeof(cMes));
	memset(cDia, 0, sizeof(cDia));
	memset(cFecha, 0, sizeof(cFecha));

	snprintf(cAnio, sizeof(cAnio), "%s", CUtileriasAfo::Substring(0, 4, pcFecha));
	snprintf(cMes, sizeof(cMes), "%s", CUtileriasAfo::Substring(4, 2, pcFecha));
	snprintf(cDia, sizeof(cDia), "%s", CUtileriasAfo::Substring(6, 2, pcFecha));
	snprintf(cFecha, sizeof(cFecha), "%s%c%s%c%s", cAnio, pcSeparador, cMes, pcSeparador, cDia);
	CUtileriasAfo::quitarEspacioDerecha(cFecha);
	strncpy(cOutFechaMDY, cFecha, sizeof(cFecha) - 1);
	cOutFechaMDY[sizeof(cFecha) - 1] = '\0';
}
