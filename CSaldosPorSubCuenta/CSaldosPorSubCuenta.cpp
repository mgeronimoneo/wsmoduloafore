#include "CSaldosPorSubCuenta.hpp"
CSaldosPorSubCuenta::CSaldosPorSubCuenta()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpAdmon, 0, sizeof(cIpAdmon));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	memset(cNss, 0, sizeof(cNss));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(&stInfShmSaldoDiario, 0, sizeof(SHM_INFOX));
	memset(&stSaldoDiario, 0, sizeof(SALDO_CUENTA));
	shmSaldoDiario = NULL;
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}

CSaldosPorSubCuenta::~CSaldosPorSubCuenta()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CSaldosPorSubCuenta::consultarSaldoPorSubcuenta(char *cNssx)
{
	short shRet = DEFAULT__;
	snprintf(cNss, sizeof(cNss), "%s", cNssx);
	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia [Nss: %s]------", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_ADMONAFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if (shRet == OK__)
	{
		strncpy(cIpAdmon, cBuff, sizeof(cIpAdmon) - 1);
		cIpAdmon[sizeof(cIpAdmon) - 1] = '\0';

		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		snprintf(cTexto, sizeof(cTexto), "[%s] ipAdmonAfore: %s", __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
		if (shRet != OK__)
		{
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpAdmon, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpAdmon, 0, sizeof(cIpAdmon));
			strncpy(cIpAdmon, &cBuff[20], sizeof(SIZE_BUFF_DAT - 20) - 1);
			cIpAdmon[sizeof(cIpAdmon) - 1] = '\0';

			CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
			snprintf(cTexto, sizeof(cTexto), "[%s] ipAdmonAfore: %s", __FUNCTION__, cIpAdmon);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
			if (shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpAdmon, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if (shRet == OK__)
		{
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_SALDO_CUENTA_DIARIO, stInfShmSaldoDiario, cOutTexto);
			if (shRet == OK__)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Info shm saldo diario: id=%i regs=%i  bytesx  = %ld ", __FUNCTION__,
						 stInfShmSaldoDiario.iIdShm, stInfShmSaldoDiario.iTotalReg, sizeof(SALDO_CUENTA) * stInfShmSaldoDiario.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				shmSaldoDiario = (SALDO_CUENTA *)CUtileriasAfo::obtenerShm(stInfShmSaldoDiario.iIdShm, SHM_RDONLY, cOutTexto);
				if (shmSaldoDiario != (void *)-1)
				{
					shRet = this->buscarSaldoDiario();
					if (shRet != OK__)
						shRet = this->ConsultarSaldoBD();
				}
				else
				{
					memset(cTexto, 0, sizeof(cTexto));
					snprintf(cTexto, sizeof(cTexto), "[%s]Error accesar shm [idShm: %i] : %s", __FUNCTION__, stInfShmSaldoDiario.iIdShm, cOutTexto);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					shRet = ERR_ACCESAR_SHM;
				}
				CUtileriasAfo::liberarShm((void *)shmSaldoDiario);
			}
			else if (shRet == ERR_NO_HAY_REG_BD)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Error: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarSaldoBD();
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar info Shm: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarSaldoBD();
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		snprintf(cTexto, sizeof(cTexto), "[%s] Error al leer archivo dat: %s [%s]", __FUNCTION__, cOutTexto, (char *)IP_ADMONAFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	snprintf(cTexto, sizeof(cTexto), "[%s] Retorno: %i", __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Termina [Nss: %s]------", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	return shRet;
}
short CSaldosPorSubCuenta::buscarSaldoDiario()
{
	short shRet = DEFAULT__;
	SALDO_CUENTA *stSaldoBuscar = NULL;

	strncpy(stSaldoDiario.cNss, cNss, SIZE_NSS - 1);	
	stSaldoDiario.cNss[SIZE_NSS - 1] = '\0';

	stSaldoBuscar = (SALDO_CUENTA *)bsearch(&stSaldoDiario, (void *)shmSaldoDiario, stInfShmSaldoDiario.iTotalReg, sizeof(SALDO_CUENTA), compararNssEnSaldoCuenta);
	if (stSaldoBuscar != NULL)
	{
		memmove(&stSaldoDiario, stSaldoBuscar, sizeof(SALDO_CUENTA));
		shRet = OK__;
	}
	else
	{
		shRet = ERR_NO_HAY_REG_SHM;
		snprintf(cTexto, sizeof(cTexto), "[%s] Nss: %s - NO_HAY_REG", __FUNCTION__, cNss);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}
SALDO_CUENTA *CSaldosPorSubCuenta::SaldoPorSubcuenta()
{
	return &stSaldoDiario;
}
short CSaldosPorSubCuenta::ConsultarSaldoBD()
{
	short shRet = DEFAULT__, shCont = 0;

	snprintf(cTexto, sizeof(cTexto), "[%s] Buscar saldo en BD: %s", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	odbcPg.Close();
	shRet = this->abrirConexionServAfo();
	if (shRet == OK__)
	{
		snprintf(cSql, sizeof(cSql), "select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
		memset(cOutTexto, 0, sizeof(cOutTexto));
		shRet = CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
		if (shRet == OK__)
		{
			strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
			cIpInfx[sizeof(cIpInfx) - 1] = '\0';
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			snprintf(cIpInfx, sizeof(cIpInfx), "%s;SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;", cIpInfx);
			shRet = CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx, (char *)USR_BD_SAFRE_AF, (char *)BD_SAFRE_AF, (char *)PWD_BD_SAFRE_AF, cOutTexto);
			if (shRet == OK__)
			{
				CSaldoSubCuenta xSelSaldo(&odbcIfx);
				snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fnsaldodiario('%s',TODAY)", cNss);
				if (xSelSaldo.Exec(cSql))
				{
					strncpy(stSaldoDiario.cNss, cNss, SIZE_NSS - 1);
					stSaldoDiario.cNss[SIZE_NSS - 1] = '\0';

					xSelSaldo.activarCols();
					while (xSelSaldo.leer())
					{
						stSaldoDiario.dSaldoEnAccion += xSelSaldo.montoEnAcciones;
						stSaldoDiario.arrSubCta[shCont].shSubCuenta = xSelSaldo.subCuenta;
						stSaldoDiario.arrSubCta[shCont].shSiefore = xSelSaldo.siefore;
						stSaldoDiario.arrSubCta[shCont].dMontoAccion = xSelSaldo.montoEnAcciones;
						stSaldoDiario.arrSubCta[shCont].dMontoPesos = xSelSaldo.montoEnPesos;
						shCont++;
					}
					if (shCont > 0)
						shRet = OK__;
					else
						shRet = ERR_NO_HAY_REG_BD;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelSaldo.odbc->GetLastError(xSelSaldo.GetHstmt());
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelSaldo.odbc->LastErrStr());
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpInfx, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else if (shRet == ERR_NO_HAY_REG_BD)
		{
			snprintf(cTexto, sizeof(cTexto), "[%s] Error: %s", __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar control: %s", __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	return shRet;
}
short CSaldosPorSubCuenta::abrirConexionServAfo()
{
	short shRet = DEFAULT__;
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if (shRet == OK__)
	{
		strncpy(cIpServAfo, cBuff, sizeof(cIpServAfo) - 1);
		cIpServAfo[sizeof(cIpServAfo) - 1] = '\0';

		CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
		snprintf(cTexto, sizeof(cTexto), "[%s] ipServiciosAfore: %s", __FUNCTION__, cIpServAfo);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
		if (shRet != OK__)
		{
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpServAfo, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpServAfo, 0, sizeof(cIpServAfo));
			strncpy(cIpServAfo, &cBuff[20], SIZE_BUFF_DAT - 20);
			cIpServAfo[SIZE_BUFF_DAT - 20 - 1] = '\0';
			CUtileriasAfo::quitarEspacioDerecha(cIpServAfo);
			snprintf(cTexto, sizeof(cTexto), "[%s] ipServiciosAfore: %s", __FUNCTION__, cIpServAfo);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpServAfo, (char *)USR_BD_SYSSERVAFO, (char *)BD_SERV_AFORE, cOutTexto);
			if (shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpServAfo, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
	}
	return shRet;
}

short CSaldosPorSubCuenta::consultarSaldoPorSubcuentaPMG(char *cNssx)
{
	short shRet = DEFAULT__;
	snprintf(cNss, sizeof(cNss), "%s", cNssx);
	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia saldo PMG[Nss: %s]------", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_ADMONAFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if (shRet == OK__)
	{
		strncpy(cIpAdmon, cBuff, sizeof(cIpAdmon) - 1);
		cIpAdmon[sizeof(cIpAdmon) - 1] = '\0';

		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		snprintf(cTexto, sizeof(cTexto), "[%s] ipAdmonAfore: %s", __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
		if (shRet != OK__)
		{
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpAdmon, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpAdmon, 0, sizeof(cIpAdmon));
			strncpy(cIpAdmon, &cBuff[20], sizeof(SIZE_BUFF_DAT - 20) - 1);
			cIpAdmon[sizeof(cIpAdmon) - 1] = '\0';
			
			CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
			snprintf(cTexto, sizeof(cTexto), "[%s] ipAdmonAfore: %s", __FUNCTION__, cIpAdmon);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
			if (shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpAdmon, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if (shRet == OK__)
		{
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_SALDO_CUENTA_DIARIO, stInfShmSaldoDiario, cOutTexto);
			if (shRet == OK__)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Info shm saldo diario: id=%i regs=%i  bytesx  = %ld ", __FUNCTION__,
						 stInfShmSaldoDiario.iIdShm, stInfShmSaldoDiario.iTotalReg, sizeof(SALDO_CUENTA) * stInfShmSaldoDiario.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				shmSaldoDiario = (SALDO_CUENTA *)CUtileriasAfo::obtenerShm(stInfShmSaldoDiario.iIdShm, SHM_RDONLY, cOutTexto);
				if (shmSaldoDiario != (void *)-1)
				{
					shRet = this->buscarSaldoDiario();
					if (shRet != OK__)
						shRet = this->ConsultarSaldoBdPMG();
				}
				else
				{
					memset(cTexto, 0, sizeof(cTexto));
					snprintf(cTexto, sizeof(cTexto), "[%s]Error accesar shm [idShm: %i] : %s", __FUNCTION__, stInfShmSaldoDiario.iIdShm, cOutTexto);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					shRet = ERR_ACCESAR_SHM;
				}
				CUtileriasAfo::liberarShm((void *)shmSaldoDiario);
			}
			else if (shRet == ERR_NO_HAY_REG_BD)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Error: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarSaldoBdPMG();
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar info Shm: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarSaldoBdPMG();
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		snprintf(cTexto, sizeof(cTexto), "[%s] Error al leer archivo dat: %s [%s]", __FUNCTION__, cOutTexto, (char *)IP_ADMONAFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	snprintf(cTexto, sizeof(cTexto), "[%s] Retorno: %i", __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Termina [Nss: %s]------", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	return shRet;
}

short CSaldosPorSubCuenta::ConsultarSaldoBdPMG()
{
	short shRet = DEFAULT__, shCont = 0;

	snprintf(cTexto, sizeof(cTexto), "[%s] Buscar saldo PMG en BD: %s", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	odbcPg.Close();
	shRet = this->abrirConexionServAfo();
	if (shRet == OK__)
	{
		snprintf(cSql, sizeof(cSql), "select valor from controlwsmoduloafore where idcontrol=%i", CTRL_SVR_INFORMIX);
		memset(cOutTexto, 0, sizeof(cOutTexto));
		shRet = CBaseDato::consultarTexto(&odbcPg, cSql, cOutTexto);
		if (shRet == OK__)
		{
			strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
			cIpInfx[sizeof(cIpInfx) - 1] = '\0';
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			snprintf(cIpInfx, sizeof(cIpInfx), "%s;SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;", cIpInfx);
			shRet = CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx, (char *)USR_BD_SAFRE_AF, (char *)BD_SAFRE_AF, (char *)PWD_BD_SAFRE_AF, cOutTexto);
			if (shRet == OK__)
			{
				CSaldoSubCuenta xSelSaldo(&odbcIfx);
				snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fnsaldodiario('%s',TODAY)", cNss);
				if (xSelSaldo.Exec(cSql))
				{
					strncpy(stSaldoDiario.cNss, cNss, SIZE_NSS - 1);
					stSaldoDiario.cNss[SIZE_NSS - 1] = '\0';

					xSelSaldo.activarCols();
					while (xSelSaldo.leer())
					{
						stSaldoDiario.dSaldoEnAccion += xSelSaldo.montoEnAcciones;
						stSaldoDiario.arrSubCta[shCont].shSubCuenta = xSelSaldo.subCuenta;
						stSaldoDiario.arrSubCta[shCont].shSiefore = xSelSaldo.siefore;
						stSaldoDiario.arrSubCta[shCont].dMontoAccion = xSelSaldo.montoEnAcciones;
						stSaldoDiario.arrSubCta[shCont].dMontoPesos = xSelSaldo.montoEnPesos;
						shCont++;
					}
					shRet = OK__;
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelSaldo.odbc->GetLastError(xSelSaldo.GetHstmt());
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelSaldo.odbc->LastErrStr());
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				shRet = ERR_CNX_BASE_DATO;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al abrir cnx[%s]: %s", __FUNCTION__, cIpInfx, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else if (shRet == ERR_NO_HAY_REG_BD)
		{
			snprintf(cTexto, sizeof(cTexto), "[%s] Error: %s", __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar control: %s", __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	return shRet;
}
