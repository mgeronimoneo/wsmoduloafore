#include "CSaldoDiarioRetParcial.hpp"
CSaldoDiarioRetParcial::CSaldoDiarioRetParcial()
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
	memset(&stInfShmSaldoDiario, 0, sizeof(SHM_INFOX));
	memset(&stSaldoDiario, 0, sizeof(SALDO_CUENTA));
	shmSaldoDiario = NULL;
	dSaldoDiario=0;
	iTipoRetiro=0;
	sprintf(cRutaLog, "%s", RUTA_LOGX);
}
CSaldoDiarioRetParcial::~CSaldoDiarioRetParcial()
{
	odbcPg.Close();
	odbcIfx.Close();
}
short CSaldoDiarioRetParcial::obtenerSaldoDiarioRetParcial(char *cNssx, int iTipoRetirox)
{
	short shRet = DEFAULT__;
	sprintf(cNss, "%s", cNssx);
	iTipoRetiro=iTipoRetirox;
	sprintf(cTexto, "[%s] ------ Inicia [Nss: %s tpoRet: %i]------",  __FUNCTION__, cNss, iTipoRetiro);
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
			shRet = CBaseDato::consultarInfoShm(&odbcPg, ID_SALDO_CUENTA_DIARIO, stInfShmSaldoDiario, cOutTexto);
			if(shRet == OK__)
			{
				sprintf(cTexto, "[%s] Info shm saldo diario: id=%i regs=%i  bytesx  = %ld ",  __FUNCTION__,
						stInfShmSaldoDiario.iIdShm, stInfShmSaldoDiario.iTotalReg, sizeof(SALDO_CUENTA) * stInfShmSaldoDiario.iTotalReg);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

				shmSaldoDiario = (SALDO_CUENTA *) CUtileriasAfo::obtenerShm(stInfShmSaldoDiario.iIdShm, SHM_RDONLY, cOutTexto);
				if( shmSaldoDiario != (void *)-1 )
				{
					shRet = this->buscarSaldoDiario();
					if(shRet!=OK__)
						shRet= this->ConsultarSaldoBD();
				}
				else
				{
					memset(cTexto,0,sizeof(cTexto));
					sprintf(cTexto, "[%s]Error accesar shm [idShm: %i] : %s", __FUNCTION__, stInfShmSaldoDiario.iIdShm ,cOutTexto );
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					shRet = ERR_ACCESAR_SHM;
				}
				CUtileriasAfo::liberarShm((void*)shmSaldoDiario);
			}
			else if(shRet==ERR_NO_HAY_REG_BD)
			{
				sprintf(cTexto, "[%s] Error: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet= this->ConsultarSaldoBD();
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				sprintf(cTexto, "[%s] Error al consultar info Shm: %s",  __FUNCTION__, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet= this->ConsultarSaldoBD();
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
	sprintf(cTexto, "[%s] ------ Termina [Nss: %s tpoRet: %i]------",  __FUNCTION__, cNss, iTipoRetiro);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	return shRet;
}
short CSaldoDiarioRetParcial::buscarSaldoDiario()
{
	short shRet = DEFAULT__;
	SALDO_CUENTA *stSaldoBuscar = NULL;
	dSaldoDiario=0;
	memcpy(stSaldoDiario.cNss, cNss, SIZE_NSS);
	stSaldoBuscar = (SALDO_CUENTA *)bsearch(&stSaldoDiario, (void *)shmSaldoDiario, stInfShmSaldoDiario.iTotalReg, sizeof(SALDO_CUENTA), compararNssEnSaldoCuenta);
	if(stSaldoBuscar != NULL)
	{
		sprintf(cTexto, "[%s] Nss: %s iTipoRetiro: %i encontro saldo",  __FUNCTION__, cNss, iTipoRetiro);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

		memcpy(&stSaldoDiario, stSaldoBuscar, sizeof(SALDO_CUENTA));
		for(short i=0; i<NUM_SUBCUENTAS;i++)
		{
			if(stSaldoDiario.arrSubCta[i].shSubCuenta>0)
			{
				switch(stSaldoDiario.arrSubCta[i].shSubCuenta)
				{
					case 1:
					case 2:
					case 6:
					case 9:
						if(iTipoRetiro==CVE_RET_DESEMPLEO_IMSS)
							dSaldoDiario+=stSaldoDiario.arrSubCta[i].dMontoPesos;
						break;
					case 5:
						if(iTipoRetiro==CVE_RET_DESEMPLEO_IMSS || iTipoRetiro==CVE_RET_MATRIMONIO_IMSS)
							dSaldoDiario+=stSaldoDiario.arrSubCta[i].dMontoPesos;
						break;
					default:
						break;
				}
			}
		}
		sprintf(cTexto, "[%s] Nss: %s SaldoDia: %.02f iTipoRetiro: %i",  __FUNCTION__, cNss, dSaldoDiario, iTipoRetiro);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
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
short CSaldoDiarioRetParcial::ConsultarSaldoBD()
{
	short shRet=DEFAULT__, shCont=0;
	dSaldoDiario=0;
	sprintf(cTexto, "[%s] Buscar saldo en BD: %s",  __FUNCTION__, cNss);
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
			strcpy(cIpInfx, cOutTexto);
			CUtileriasAfo::quitarEspacioDerecha(cIpInfx);
			strcat(cIpInfx, ";SERVER=safre_tcp;Client Locale=en_us.CP1252;Database Locale=en_US.819;");
			shRet=CBaseDato::abrirConexionInfx(&odbcIfx, cIpInfx,(char*)USR_BD_SAFRE_AF, (char*)BD_SAFRE_AF, (char*)PWD_BD_SAFRE_AF,cOutTexto);
			if(shRet==OK__)
			{
				CConsultaDouble xSelSaldo(&odbcIfx);
				sprintf(cSql, "EXECUTE FUNCTION fnRetParObtenerSaldo('%s', %i,TODAY)", cNss, iTipoRetiro);
				if(xSelSaldo.Exec(cSql))
				{
					xSelSaldo.activarCols();
					if(xSelSaldo.leer())
					{
						dSaldoDiario=xSelSaldo.campo_double;
						shRet=OK__;
					}
					else
						shRet=ERR_NO_HAY_REG_BD;
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
double CSaldoDiarioRetParcial::SaldoDiario()
{
	return dSaldoDiario;
}
short CSaldoDiarioRetParcial::abrirConexionServAfo()
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
