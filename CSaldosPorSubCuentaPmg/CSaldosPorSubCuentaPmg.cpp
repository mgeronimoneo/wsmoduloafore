#include "CSaldosPorSubCuentaPmg.hpp"
CSaldosPorSubCuentaPmg::CSaldosPorSubCuentaPmg()
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
	memset(&stSaldoDiario, 0, sizeof(SALDO_CUENTA_PMG));
	memset(&stSaldosPmg, 0, sizeof(SALDOS_PMG));
	shmSaldoDiario = NULL;
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}

CSaldosPorSubCuentaPmg::~CSaldosPorSubCuentaPmg()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CSaldosPorSubCuentaPmg::consultarSaldoPorSubcuentaPmg(char *cNssx, char *cSecuenciax, int iSubVivx)
{
	short shRet = DEFAULT__;
	snprintf(cNss, sizeof(cNss), "%s", cNssx);
	snprintf(cSecuencia, sizeof(cSecuencia), "%s", cSecuenciax);
	iSubViv = iSubVivx;
	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia [Nss: %s] , [Secuencia: %s], [Estado Vivienda: %i]------", __FUNCTION__, cNss, cSecuenciax, iSubVivx);
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
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_SALDO_CUENTA_DIARIO, stInfShmSaldoDiario, cOutTexto);
			if (shRet == OK__)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Info shm saldo diario: id=%i regs=%i  bytesx  = %ld ", __FUNCTION__,
						 stInfShmSaldoDiario.iIdShm, stInfShmSaldoDiario.iTotalReg, sizeof(SALDO_CUENTA_PMG) * stInfShmSaldoDiario.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				shmSaldoDiario = (SALDO_CUENTA_PMG *)CUtileriasAfo::obtenerShm(stInfShmSaldoDiario.iIdShm, SHM_RDONLY, cOutTexto);
				if (shmSaldoDiario != (void *)-1)
				{
					shRet = this->obtenerRetroactivoPgs();
					if (shRet != OK__)
					{ // si no hay resultados consulta en safre
						shRet = this->obtenerRetroactivoInf();
					}

					if (shRet == OK__)
					{ // si la consulta de retroactivo es exitosa buscas saldo diario
						shRet = this->buscarSaldoDiario();
						if (shRet != OK__)
						{ // si la consulta de saldo diario a pg no es exitosa
							shRet = this->ConsultarSaldoBD();
						}
					}
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
				if (shRet == OK__)
				{
					shRet = this->obtenerRetroactivoInf();
				}
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar info Shm: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarSaldoBD();
				if (shRet == OK__)
				{
					shRet = this->obtenerRetroactivoInf();
				}
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
short CSaldosPorSubCuentaPmg::obtenerRetroactivoPgs()
{
	short shRet = DEFAULT__, iEstatusConsulta = 0;

	snprintf(cTexto, sizeof(cTexto), "[%s] Buscar devengado y retroactivo en BD: %s", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	odbcPg.Close();
	shRet = this->abrirConexionServAfo();
	if (shRet == OK__)
	{
		CConsultarRetroactivoPmg xSelRetro(&odbcPg);
		snprintf(cSql, sizeof(cSql), "SELECT retroactivo, devengado, estatus FROM fncalcularetroactivopmgimss('%s','%s')", cNss, cSecuencia);
		if (xSelRetro.Exec(cSql))
		{
			xSelRetro.activarCols();
			if (xSelRetro.leer())
			{
				stSaldosPmg.dRetroactivo = xSelRetro.dRetroactivo;
				stSaldosPmg.dDevengado = xSelRetro.dDevengado;
				iEstatusConsulta = xSelRetro.iEstatus;
				snprintf(cTexto, sizeof(cTexto), "Valor de retorno -- > %i", iEstatusConsulta);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
			if (iEstatusConsulta > 0)
				shRet = OK__;
			else
				shRet = ERR_NO_HAY_REG_BD;
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			xSelRetro.odbc->GetLastError(xSelRetro.GetHstmt());
			snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelRetro.odbc->LastErrStr());
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	return shRet;
}

short CSaldosPorSubCuentaPmg::obtenerRetroactivoInf()
{
	short shRet = DEFAULT__, shCont = 0, shSubcuenta = 0;
	int iEstatusCons = 0, iSuficiencia = 1, iNumRegistro = 0;
	dRetroactivo = 0;
	dDevengado = 0;

	snprintf(cTexto, sizeof(cTexto), "[%s] Obtener Retroactivo Inf: %s", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	odbcPg.Close();
	shRet = this->abrirConexionServAfo();
	if (shRet == OK__)
	{
		CConsultarRetroactivoPmg xSelSaldo(&odbcIfx);
		snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_calcularetroactivopmgimss('%s', '%s', TODAY)", cNss, cSecuencia);

		if (xSelSaldo.Exec(cSql))
		{
			xSelSaldo.activarCols();
			if (xSelSaldo.leer())
			{
				iEstatusCons = xSelSaldo.iEstatus;
				dRetroactivo = xSelSaldo.dRetroactivo;
				dDevengado = xSelSaldo.dDevengado;
				iNumRegistro = xSelSaldo.iRegistro;

				if (dRetroactivo > dSaldoDia)
				{
					iSuficiencia = 0;
				}

				stSaldosPmg.dRetroactivo = dRetroactivo;
				stSaldosPmg.dDevengado = dDevengado;
				stSaldosPmg.iSuficiencia = iSuficiencia;
				stSaldosPmg.iRegistros = iNumRegistro;
			}
			else
			{
				shRet = ERR_NO_HAY_REG_BD;
				xSelSaldo.odbc->GetLastError(xSelSaldo.GetHstmt());
				snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelSaldo.odbc->LastErrStr());
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
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
	return shRet;
}

short CSaldosPorSubCuentaPmg::buscarSaldoDiario()
{
	snprintf(cTexto, sizeof(cTexto), "[%s] Buscar Saldo Diario BD: %s", __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	short shRet = DEFAULT__, i = 0, shCuenta = 0;
	SALDO_CUENTA_PMG *stSaldoBuscar = NULL;
	dSaldoDia = 0;

	strncpy(stSaldoDiario.cNss, cNss, sizeof(SIZE_NSS) - 1);
	stSaldoDiario.cNss[sizeof(stSaldoDiario.cNss) - 1] = '\0';

	stSaldoBuscar = (SALDO_CUENTA_PMG *)bsearch(&stSaldoDiario, (void *)shmSaldoDiario, stInfShmSaldoDiario.iTotalReg, sizeof(SALDO_CUENTA_PMG), compararNssEnSaldoCuenta);
	if (stSaldoBuscar != NULL)
	{
		for (i = 0; i <= 20; i++)
		{
			shCuenta = shmSaldoDiario->arrSubCtaPmg[i].shSubCuenta;
			if (shCuenta == 1 || shCuenta == 2 || shCuenta == 5 || shCuenta == 6 || shCuenta == 9)
			{
				// stSaldosPmg.dSaldoDia = stSaldosPmg.dSaldoDia + shmSaldoDiario->arrSubCtaPmg[i].dMontoAccion;
				dSaldoDia = dSaldoDia + shmSaldoDiario->arrSubCtaPmg[i].dMontoAccion;
			}
			else
			{
				if (shCuenta == 4 && iSubViv == 1)
				{
					dSaldoDia = dSaldoDia + shmSaldoDiario->arrSubCtaPmg[i].dMontoAccion;
				}
			}
		}

		stSaldosPmg.dSaldoDia = dSaldoDia;
		if (stSaldosPmg.dRetroactivo > dSaldoDia)
		{
			stSaldosPmg.iSuficiencia = 0;
		}

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
SALDOS_PMG *CSaldosPorSubCuentaPmg::SaldoPorSubcuentaPmg()
{
	return &stSaldosPmg;
}
short CSaldosPorSubCuentaPmg::ConsultarSaldoBD()
{
	short shRet = DEFAULT__, shCont = 0, shSubcuenta = 0;
	dSaldoDia = 0;

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
			strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
			shRet = CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx, (char *)USR_BD_SAFRE_AF, (char *)BD_SAFRE_AF, (char *)PWD_BD_SAFRE_AF, cOutTexto);

			if (shRet == OK__)
			{
				CSaldoCuentaPmg xSelSaldo(&odbcIfx);
				snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fnsaldodiario('%s',TODAY)", cNss);
				if (xSelSaldo.Exec(cSql))
				{
					xSelSaldo.activarCols();
					while (xSelSaldo.leer())
					{
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

						shSubcuenta = xSelSaldo.subCuenta;
						if (shSubcuenta == 1 || shSubcuenta == 2 || shSubcuenta == 5 || shSubcuenta == 6 || shSubcuenta == 9)
						{
							dSaldoDia = dSaldoDia + xSelSaldo.montoEnPesos;
						}
						else
						{
							if (shSubcuenta == 4 && iSubViv == 1)
							{
								dSaldoDia = dSaldoDia + xSelSaldo.montoEnPesos;
							}
						}
						shCont++;
					}
					stSaldosPmg.dSaldoDia = dSaldoDia;
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
short CSaldosPorSubCuentaPmg::abrirConexionServAfo()
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

// 418i

short CSaldosPorSubCuentaPmg::obtenerFechaLiquidaPmg(char *cNssx)
{
	short shRet = DEFAULT__;
	strncpy(cIpInfx, cOutTexto, sizeof(cIpInfx) - 1);
	cIpInfx[sizeof(cIpInfx) - 1] = '\0';
	CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
	strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
	shRet = CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx, (char *)USR_BD_SAFRE_AF, (char *)BD_SAFRE_AF, (char *)PWD_BD_SAFRE_AF, cOutTexto);

	if (shRet == OK__)
	{
		CCampoTexto xSelFecha(&odbcIfx);
		snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_obtenerfechaliquidapmg('%s')", cNssx);
		CUtileriasAfo::grabarLogx(cRutaLog, cSql);
		if (xSelFecha.Exec(cSql))
		{
			xSelFecha.activarCols();
			if (xSelFecha.leer())
			{
				snprintf(cFechaLiquida, sizeof(cFechaLiquida), "%s", xSelFecha.texto);
				CUtileriasAfo::grabarLogx(cRutaLog, cFechaLiquida);
				shRet = OK__;
			}
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			xSelFecha.odbc->GetLastError(xSelFecha.GetHstmt());
			snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelFecha.odbc->LastErrStr());
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	return shRet;
}

char *CSaldosPorSubCuentaPmg::RespuestaFecha()
{
	return cFechaLiquida;
}
