#include "CConsultarCtaSaldoVol.hpp"
CConsultarCtaSaldoVol::CConsultarCtaSaldoVol()
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
	memset(&stInfShmSaldoVolCta, 0, sizeof(SHM_INFOX));
	memset(&stSalVol, 0, sizeof(SALDO_VOL));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}

CConsultarCtaSaldoVol::~CConsultarCtaSaldoVol()
{
	odbcPg.Close();
	odbcIfx.Close();
	if (stSalVol.arrCtaSaldoVol != NULL)
		free((void *)stSalVol.arrCtaSaldoVol);
}

short CConsultarCtaSaldoVol::consultarFechaAporte(char *cNssx)
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
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_CTA_SALDO_VOL, stInfShmSaldoVolCta, cOutTexto);
			if (shRet == OK__)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Info shm cta_regimen : id=%i regs=%i  bytesx  = %ld ", __FUNCTION__,
						 stInfShmSaldoVolCta.iIdShm, stInfShmSaldoVolCta.iTotalReg, sizeof(SALDO_VOL) * stInfShmSaldoVolCta.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				shmSalVol = (SALDO_VOL *)CUtileriasAfo::obtenerShm(stInfShmSaldoVolCta.iIdShm, SHM_RDONLY, cOutTexto);
				if (shmSalVol != (void *)-1)
				{
					shRet = this->buscarCtaSaldoVolD();
					if (shRet != OK__)
					{
						shRet = this->ConsultarSaldoVolBD();
					}
				}
			}
			else if (shRet == ERR_NO_HAY_REG_BD)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Error: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarSaldoVolBD(); // Metodo a Informix directo
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar info Shm: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarSaldoVolBD(); // Metodo a Informix directo
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

short CConsultarCtaSaldoVol::buscarCtaSaldoVolD()
{
	short shRet = DEFAULT__;
	SALDO_VOL *stSalVolBuscar = NULL;

	snprintf(stSalVol.cNss, SIZE_NSS, "%s", cNss);

	stSalVolBuscar = (SALDO_VOL *)bsearch(&stSalVol, (void *)shmSalVol, stInfShmSaldoVolCta.iTotalReg, sizeof(SALDO_VOL), compararNssCtaSaldoVol);
	if (stSalVolBuscar != NULL)
	{
		memmove(&stSalVol, stSalVolBuscar, sizeof(SALDO_VOL));
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

short CConsultarCtaSaldoVol::ConsultarSaldoVolBD()
{
	odbcPg.Close();
	short shRet = DEFAULT__;
	// consultar si está cargada la memoria 18001
	short banderaShm = DEFAULT__;
	snprintf(cTexto, sizeof(cTexto), "[ConsultarSaldoVolBD] ------ Inicia [ConsultarMemoria: ]------");
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_ADMONAFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if (shRet == OK__)
	{
		// abrimos conexion a admonafore
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
			strncpy(cIpAdmon, &cBuff[20], sizeof(cIpAdmon) - 1);
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
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_SALDO_CUENTA_DIARIO, stInfShmSaldoVolCta, cOutTexto);
			if (shRet == OK__)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] se encontrÃ³ la memoria 18001 : id=%i regs=%i  bytesx  = %ld ", __FUNCTION__,
						 stInfShmSaldoVolCta.iIdShm, stInfShmSaldoVolCta.iTotalReg, sizeof(SALDO_VOL) * stInfShmSaldoVolCta.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				// bandera en 1
				banderaShm = OK__;
				shRet = ERR_EXEC_SQL;
			}
			else if (shRet == ERR_NO_HAY_REG_BD)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Error no se encontrÃ³ memoria: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				// bandera en 0
				banderaShm = DEFAULT__;
				snprintf(cTexto, sizeof(cTexto), "valor de Bandera: %i", banderaShm);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				snprintf(cTexto, sizeof(cTexto), "[%s] Error al consultar info Shm: %s", __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				banderaShm = DEFAULT__;
				snprintf(cTexto, sizeof(cTexto), "valor de Bandera: %i", banderaShm);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		snprintf(cTexto, sizeof(cTexto), "[%s] Error al leer archivo dat: %s [%s]", __FUNCTION__, cOutTexto, (char *)IP_ADMONAFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	short shCont = 0;
	int iTotalReg = 0;

	snprintf(cTexto, sizeof(cTexto), "[%s] Buscar Cta_saldo_vol en BD: %s", __FUNCTION__, cNss);
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
			memset(cOutTexto, 0, sizeof(cOutTexto));
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			shRet = CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx, (char *)USR_BD_SAFRE_AF, (char *)BD_SAFRE_AF, (char *)PWD_BD_SAFRE_AF, cOutTexto);

			if (shRet == OK__)
			{
				snprintf(cSql, sizeof(cSql), "SELECT count(folio)::int FROM cta_saldo_vol WHERE nss = '%s'", cNss);
				CUtileriasAfo::grabarLogx(cRutaLog, cSql);
				shRet = CBaseDato::consultarNumero(&odbcIfx, cSql, iTotalReg, cOutTexto);
				if (shRet == OK__)
				{
					snprintf(cTexto, sizeof(cTexto), "TotalRegs: %i", iTotalReg);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

					if (iTotalReg > 0)
					{
						stSalVol.arrCtaSaldoVol = static_cast<SALDO_VOL_SUB_EST *>(calloc(static_cast<size_t>(iTotalReg), sizeof(SALDO_VOL_SUB_EST)));
						if (!stSalVol.arrCtaSaldoVol)
						{
							snprintf(cTexto, sizeof(cTexto), "[%s] Error: No se pudo asignar memoria para arrCtaSaldoVol", __FUNCTION__);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						}
					}
					else
					{
						stSalVol.arrCtaSaldoVol = NULL;
					}
					CConsultarSaldoVol xSelSaldoVol(&odbcIfx);
					// validar si mememoria está cargada
					if (banderaShm == OK__)
					{
						snprintf(cTexto, sizeof(cTexto), "Entra aconsultar con fecha de dia anterior: %i", banderaShm);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_obtenerinfo_saldovol('%s',1);", cNss);
						CUtileriasAfo::grabarLogx(cRutaLog, cSql);
					}
					else
					{
						snprintf(cTexto, sizeof(cTexto), "Entra a consultar todos los registro: %i", banderaShm);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_obtenerinfo_saldovol('%s');", cNss);
						CUtileriasAfo::grabarLogx(cRutaLog, cSql);
					}

					if (xSelSaldoVol.Exec(cSql))
					{
						memmove(stSalVol.cNss, cNss, SIZE_NSS);

						xSelSaldoVol.activarCols();

						while (xSelSaldoVol.leer())
						{
							stSalVol.arrCtaSaldoVol[shCont].iFolio = xSelSaldoVol.folio;
							stSalVol.arrCtaSaldoVol[shCont].iConsecutivoLote = xSelSaldoVol.consecutivoLote;
							memmove(stSalVol.arrCtaSaldoVol[shCont].cNss, xSelSaldoVol.nss, SIZE_NSS);
							stSalVol.arrCtaSaldoVol[shCont].iSiefore = xSelSaldoVol.siefore;
							stSalVol.arrCtaSaldoVol[shCont].iSubCuenta = xSelSaldoVol.subCuenta;
							memmove(stSalVol.arrCtaSaldoVol[shCont].cFechaValor, xSelSaldoVol.fechaValor, SIZE_FECHA - 2);
							memmove(stSalVol.arrCtaSaldoVol[shCont].cFechaConversion, xSelSaldoVol.fechaConversion, SIZE_FECHA - 2);
							stSalVol.arrCtaSaldoVol[shCont].iMontoEnPesos = xSelSaldoVol.montoEnPesos;
							stSalVol.arrCtaSaldoVol[shCont].iMontoEnAcciones = xSelSaldoVol.montoEnAcciones;
							stSalVol.arrCtaSaldoVol[shCont].iSaldoAcciones = xSelSaldoVol.saldoAcciones;
							memmove(stSalVol.arrCtaSaldoVol[shCont].cFechaSaldo, xSelSaldoVol.fechaSaldo, SIZE_FECHA - 2);
							memset(stSalVol.arrCtaSaldoVol[shCont].cUsuario, 0, sizeof(stSalVol.arrCtaSaldoVol[shCont].cUsuario));
							memmove(stSalVol.arrCtaSaldoVol[shCont].cUsuario, xSelSaldoVol.usuario, sizeof(xSelSaldoVol.usuario));

							snprintf(cTexto, sizeof(cTexto), "Folio [%i] Saldo acciones [%f]", xSelSaldoVol.folio, xSelSaldoVol.saldoAcciones);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
							snprintf(cTexto, sizeof(cTexto), "fechaSaldo [%s] fechaValor [%s]", xSelSaldoVol.fechaSaldo, xSelSaldoVol.fechaValor);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
							snprintf(cTexto, sizeof(cTexto), "Fecha Conversion [%s]", xSelSaldoVol.fechaConversion);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
							snprintf(cTexto, sizeof(cTexto), "se ejecuto correctamente funcion");
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

							shCont++;
						}
						if (shCont > 0)
						{
							shRet = OK__;
							shRegistros = shCont;
							snprintf(cTexto, sizeof(cTexto), "[%s] Total Reg. leidos: %i", __FUNCTION__, shCont);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						}
						else
							shRet = ERR_NO_HAY_REG_BD;
					}
					else
					{
						shRet = ERR_EXEC_SQL;
						xSelSaldoVol.odbc->GetLastError(xSelSaldoVol.GetHstmt());
						snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelSaldoVol.odbc->LastErrStr());
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					}
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

short CConsultarCtaSaldoVol::abrirConexionServAfo()
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
			cIpServAfo[sizeof(cIpServAfo)] = '\0';
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

// Funcion para retornar los datos obtenidos en instancia de la estructura shmSalVol.
SALDO_VOL *CConsultarCtaSaldoVol::RespCtaSaldoVol()
{
	return &stSalVol;
}

short CConsultarCtaSaldoVol::registrosCtaSaldoVol()
{
	return shRegistros;
}
