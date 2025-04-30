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
	
	memset(cIpServAfo,0,sizeof(cIpServAfo));
	memset(&stInfShmSaldoVolCta, 0, sizeof(SHM_INFOX));
	memset(&stSalVol, 0, sizeof(SALDO_VOL));
	sprintf(cRutaLog, "%s", RUTA_LOGX);
}

CConsultarCtaSaldoVol::~CConsultarCtaSaldoVol()
{
	odbcPg.Close();
	odbcIfx.Close();
	if(stSalVol.arrCtaSaldoVol != NULL)
		free((void*)stSalVol.arrCtaSaldoVol);
}

short CConsultarCtaSaldoVol::consultarFechaAporte(char *cNssx)
{
	short shRet = DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	sprintf(cTexto, "[%s] ------ Inicia [Nss: %s]------",  __FUNCTION__, cNss);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_ADMONAFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	
	if(shRet == OK__)
	{
		memcpy(cIpAdmon, cBuff, sizeof(cIpAdmon));
		cIpAdmon[16]={0};
		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
		if(shRet != OK__)
		{
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpAdmon, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpAdmon, 0, sizeof(cIpAdmon));
			memcpy(cIpAdmon, &cBuff[20], SIZE_BUFF_DAT-20);
			cIpAdmon[16]={0};
			CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
			sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
			if(shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpAdmon, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if(shRet == OK__)
		{
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_CTA_SALDO_VOL, stInfShmSaldoVolCta, cOutTexto);	
			if(shRet == OK__)
			{
				sprintf(cTexto, "[%s] Info shm cta_regimen : id=%i regs=%i  bytesx  = %ld ",  __FUNCTION__,
						stInfShmSaldoVolCta.iIdShm, stInfShmSaldoVolCta.iTotalReg, sizeof(SALDO_VOL) * stInfShmSaldoVolCta.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				shmSalVol = (SALDO_VOL *) CUtileriasAfo::obtenerShm(stInfShmSaldoVolCta.iIdShm, SHM_RDONLY, cOutTexto);
				if( shmSalVol != (void *)-1 )
				{
					shRet = this->buscarCtaSaldoVolD();
					if(shRet!=OK__)
					{
						shRet= this->ConsultarSaldoVolBD();
					}
				}
			}
			else if(shRet==ERR_NO_HAY_REG_BD)
			{
				sprintf(cTexto, "[%s] Error: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet= this->ConsultarSaldoVolBD(); //Metodo a Informix directo
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				sprintf(cTexto, "[%s] Error al consultar info Shm: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet= this->ConsultarSaldoVolBD(); // Metodo a Informix directo
			}
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


short CConsultarCtaSaldoVol::buscarCtaSaldoVolD()
{
	short shRet = DEFAULT__;
	SALDO_VOL *stSalVolBuscar = NULL;

	memcpy(stSalVol.cNss, cNss, SIZE_NSS);
	stSalVolBuscar = (SALDO_VOL *)bsearch(&stSalVol, (void *)shmSalVol, stInfShmSaldoVolCta.iTotalReg, sizeof(SALDO_VOL), compararNssCtaSaldoVol);
	if(stSalVolBuscar != NULL)
	{
		memcpy(&stSalVol, stSalVolBuscar, sizeof(SALDO_VOL));
		shRet = OK__;
	}
	else
	{
		shRet = ERR_NO_HAY_REG_SHM;
		sprintf(cTexto, "[%s] Nss: %s - NO_HAY_REG",  __FUNCTION__, cNss);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}
	return shRet;
}

short CConsultarCtaSaldoVol::ConsultarSaldoVolBD()
{
	odbcPg.Close();
	short shRet = DEFAULT__;
	//consultar si está cargada la memoria 18001
	short banderaShm = DEFAULT__;
	sprintf(cTexto, "[ConsultarSaldoVolBD] ------ Inicia [ConsultarMemoria: ]------");
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_ADMONAFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet==OK__)
	{
		//abrimos conexion a admonafore
		memcpy(cIpAdmon, cBuff, sizeof(cIpAdmon));
		cIpAdmon[16]={0};
		CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
		sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
		if(shRet != OK__)
		{
			sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpAdmon, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			memset(cIpAdmon, 0, sizeof(cIpAdmon));
			memcpy(cIpAdmon, &cBuff[20], SIZE_BUFF_DAT-20);
			cIpAdmon[16]={0};
			CUtileriasAfo::quitarEspacioDerecha(cIpAdmon);
			sprintf(cTexto, "[%s] ipAdmonAfore: %s",  __FUNCTION__, cIpAdmon);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			shRet = CBaseDato::abrirConexion(&odbcPg, cIpAdmon, (char *)USR_BD_SYSADMONAFORE, (char *)BD_ADMON_AFORE, cOutTexto);
			if(shRet != OK__)
			{
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpAdmon, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		if(shRet == OK__)
		{
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_SALDO_CUENTA_DIARIO, stInfShmSaldoVolCta, cOutTexto);	
			if(shRet == OK__)
			{
				sprintf(cTexto, "[%s] se encontró la memoria 18001 : id=%i regs=%i  bytesx  = %ld ",  __FUNCTION__,
						stInfShmSaldoVolCta.iIdShm, stInfShmSaldoVolCta.iTotalReg, sizeof(SALDO_VOL) * stInfShmSaldoVolCta.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				//bandera en 1
				banderaShm = OK__;
				shRet = ERR_EXEC_SQL;

			}
			else if(shRet==ERR_NO_HAY_REG_BD)
			{
				sprintf(cTexto, "[%s] Error no se encontró memoria: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				//bandera en 0
				banderaShm = DEFAULT__ ;
				sprintf(cTexto,"valor de Bandera: %i",banderaShm);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				sprintf(cTexto, "[%s] Error al consultar info Shm: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				banderaShm = DEFAULT__;
				sprintf(cTexto,"valor de Bandera: %i",banderaShm);
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
	short shCont=0;
	int iTotalReg=0;

	sprintf(cTexto, "[%s] Buscar Cta_saldo_vol en BD: %s",  __FUNCTION__, cNss);
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
			cIpInfx[sizeof(cIpInfx)-1]={0};
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);

			strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
			memset(cOutTexto,0,sizeof(cOutTexto));
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);

			if(shRet==OK__)
			{
				sprintf(cSql, "SELECT count(folio)::int FROM cta_saldo_vol WHERE nss = '%s'", cNss);
				CUtileriasAfo::grabarLogx(cRutaLog, cSql);
				shRet=CBaseDato::consultarNumero(&odbcIfx, cSql, iTotalReg, cOutTexto);
				if(shRet==OK__)
				{

					sprintf(cTexto, "TotalRegs: %i", iTotalReg);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

					stSalVol.arrCtaSaldoVol = (SALDO_VOL_SUB_EST*)calloc(iTotalReg, sizeof(SALDO_VOL_SUB_EST));
					CConsultarSaldoVol xSelSaldoVol(&odbcIfx);
					//validar si mememoria está cargada
					if (banderaShm == OK__)
					{
						sprintf(cTexto, "Entra aconsultar con fecha de dia anterior: %i", banderaShm);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						sprintf(cSql, "EXECUTE FUNCTION fn_obtenerinfo_saldovol('%s',1);",cNss);
						CUtileriasAfo::grabarLogx(cRutaLog, cSql);
					}
					else
					{
						sprintf(cTexto, "Entra a consultar todos los registro: %i", banderaShm);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						sprintf(cSql, "EXECUTE FUNCTION fn_obtenerinfo_saldovol('%s');",cNss);
						CUtileriasAfo::grabarLogx(cRutaLog, cSql);
				
					}
					
					if(xSelSaldoVol.Exec(cSql))
					{
						memcpy(stSalVol.cNss, cNss, SIZE_NSS);

						xSelSaldoVol.activarCols();

						while(xSelSaldoVol.leer())
						{
							stSalVol.arrCtaSaldoVol[shCont].iFolio=xSelSaldoVol.folio;
							stSalVol.arrCtaSaldoVol[shCont].iConsecutivoLote=xSelSaldoVol.consecutivoLote;
							memcpy(stSalVol.arrCtaSaldoVol[shCont].cNss, xSelSaldoVol.nss, SIZE_NSS);
							stSalVol.arrCtaSaldoVol[shCont].iSiefore=xSelSaldoVol.siefore;
							stSalVol.arrCtaSaldoVol[shCont].iSubCuenta=xSelSaldoVol.subCuenta;
							memcpy(stSalVol.arrCtaSaldoVol[shCont].cFechaValor, xSelSaldoVol.fechaValor, SIZE_FECHA-2);
							memcpy(stSalVol.arrCtaSaldoVol[shCont].cFechaConversion, xSelSaldoVol.fechaConversion, SIZE_FECHA-2);
							stSalVol.arrCtaSaldoVol[shCont].iMontoEnPesos=xSelSaldoVol.montoEnPesos;
							stSalVol.arrCtaSaldoVol[shCont].iMontoEnAcciones=xSelSaldoVol.montoEnAcciones;
							stSalVol.arrCtaSaldoVol[shCont].iSaldoAcciones=xSelSaldoVol.saldoAcciones;
							memcpy(stSalVol.arrCtaSaldoVol[shCont].cFechaSaldo, xSelSaldoVol.fechaSaldo, SIZE_FECHA-2);
							memset(stSalVol.arrCtaSaldoVol[shCont].cUsuario, 0, sizeof(stSalVol.arrCtaSaldoVol[shCont].cUsuario));
							memcpy(stSalVol.arrCtaSaldoVol[shCont].cUsuario, xSelSaldoVol.usuario, strlen(xSelSaldoVol.usuario));
								
							sprintf(cTexto, "Folio [%i] Saldo acciones [%f]", xSelSaldoVol.folio,  xSelSaldoVol.saldoAcciones);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
							sprintf(cTexto, "fechaSaldo [%s] fechaValor [%s]", xSelSaldoVol.fechaSaldo, xSelSaldoVol.fechaValor);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
							sprintf(cTexto, "Fecha Conversion [%s]", xSelSaldoVol.fechaConversion);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
							sprintf(cTexto, "se ejecuto correctamente funcion");
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					
							shCont++;
						}
						if(shCont>0)
						{
							shRet=OK__;
							shRegistros = shCont;
							sprintf(cTexto, "[%s] Total Reg. leidos: %i",  __FUNCTION__, shCont);
							CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						}
						else
							shRet=ERR_NO_HAY_REG_BD;
					}
					else
					{
						shRet = ERR_EXEC_SQL;
						xSelSaldoVol.odbc->GetLastError( xSelSaldoVol.GetHstmt() );
						sprintf( cTexto, "[%s] Err: %s", __FUNCTION__, xSelSaldoVol.odbc->LastErrStr() );
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					}
				}
			}
			else
			{
				shRet = ERR_CNX_BASE_DATO;
				sprintf(cTexto, "[%s] Error al abrir cnx[%s]: %s",  __FUNCTION__, cIpInfx, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
		else if(shRet==ERR_NO_HAY_REG_BD)
		{
			sprintf(cTexto, "[%s] Error: %s",  __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
		else
		{
			shRet = ERR_EXEC_SQL;
			sprintf(cTexto, "[%s] Error al consultar control: %s",  __FUNCTION__, cOutTexto);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
		}
	}
	
	return shRet;
}

short CConsultarCtaSaldoVol::abrirConexionServAfo()
{
	short shRet=DEFAULT__;
	shRet = CUtileriasAfo::leerArchivoCnf((char *)IP_SERV_AFO_DAT, cBuff, SIZE_BUFF_DAT, cOutTexto);
	if(shRet == OK__)
	{
		memcpy(cIpServAfo, cBuff, sizeof(cIpServAfo));
		cIpServAfo[16]={0};
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
			cIpServAfo[16]={0};
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

//Funcion para retornar los datos obtenidos en instancia de la estructura shmSalVol.
SALDO_VOL *CConsultarCtaSaldoVol::RespCtaSaldoVol()
{
		return &stSalVol;
}

short CConsultarCtaSaldoVol::registrosCtaSaldoVol()
{
	return shRegistros;
}

