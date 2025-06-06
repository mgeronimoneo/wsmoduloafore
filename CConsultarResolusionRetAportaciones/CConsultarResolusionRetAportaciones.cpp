/*
 * CConsultarResolusionRetAportaciones.cpp
 *
 *  Created on: Nov 14, 2016
 *      Author: eosunap
 */

#include "CConsultarResolusionRetAportaciones.hpp"

CConsultarResolusionRetAportaciones::CConsultarResolusionRetAportaciones()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	snprintf(cRutaLog, sizeof(cRutaLog), "%s", RUTA_LOGX);
}

CConsultarResolusionRetAportaciones::~CConsultarResolusionRetAportaciones()
{
	odbcPg.Close();
	odbcIfx.Close();
}

short CConsultarResolusionRetAportaciones::ConsultarResolusion(char *cCurpx, char *cNssx)
{
	short shRet = DEFAULT__;
	int iFlagReplica = -1;

	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Inicia [Curp: %s Nss: %s]------", __FUNCTION__, cCurpx, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

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
			strncpy(cIpServAfo, &cBuff[20], sizeof(SIZE_BUFF_DAT - 20));
			cIpServAfo[sizeof(cIpServAfo) - 1] = '\0';
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

		if (shRet == OK__)
		{
			snprintf(cSql, sizeof(cSql), "SELECT fnValidarReplica FROM fnValidarReplica();");
			CUtileriasAfo::grabarLogx(cRutaLog, cSql);

			shRet = CBaseDato::consultarNumero(&odbcPg, cSql, iFlagReplica, cOutTexto);
			if (shRet != OK__)
			{
				snprintf(cTexto, sizeof(cTexto), "[%s] Excep. validar replica: %i %s", __FUNCTION__, shRet, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}

			if (iFlagReplica == 0)
			{
				C_SumaRegs xSelResol(&odbcPg);
				snprintf(cSql, sizeof(cSql), "SELECT fnconsultaresolpensionretaportaciones AS suma FROM fnconsultaresolpensionretaportaciones('%s','%s');", cCurpx, cNssx);
				CUtileriasAfo::grabarLogx(cRutaLog, cSql);

				if (xSelResol.Exec(cSql))
				{
					xSelResol.activarCols();
					if (xSelResol.leer())
					{
						iResolusion = (int)xSelResol.suma;
						shRet = OK__;
					}
					else
					{
						// shRet=ERR_NO_HAY_REG_BD;
						snprintf(cTexto, sizeof(cTexto), "No hay Registros en la BD de SERVICIOSAFORE CURP-> %s NSS-> %s", cCurpx, cNssx);
						CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
						shRet = this->ConsultarResolusionBD(cCurpx, cNssx);
					}
				}
				else
				{
					shRet = ERR_EXEC_SQL;
					xSelResol.odbc->GetLastError(xSelResol.GetHstmt());
					snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelResol.odbc->LastErrStr());
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				}
			}
			else
			{
				snprintf(cTexto, sizeof(cTexto), "No se a replicado a la BD de SERVICIOSAFORE CURP-> %s NSS-> %s", cCurpx, cNssx);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
				shRet = this->ConsultarResolusionBD(cCurpx, cNssx);
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		snprintf(cTexto, sizeof(cTexto), "[%s] Error al leer archivo dat: %s [%s]", __FUNCTION__, cOutTexto, (char *)IP_SERV_AFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	snprintf(cTexto, sizeof(cTexto), "[%s] Retorno: %i", __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	snprintf(cTexto, sizeof(cTexto), "[%s] ------ Termina [Curp: %s Nss: %s]------", __FUNCTION__, cCurpx, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
short CConsultarResolusionRetAportaciones::ConsultarResolusionBD(char *cCurpx, char *cNssx)
{
	short shRet = DEFAULT__;
	int iTipoSeguro = 0;
	char cAnio[6] = {0}, cMes[4] = {0}, cDia[4] = {0}, cTipoPension[2] = {0};
	snprintf(cTexto, sizeof(cTexto), "[%s] validar registro en BD CURP: %s NSS: %s", __FUNCTION__, cCurpx, cNssx);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	snprintf(cSql, sizeof(cSql), "SELECT valor FROM controlwsmoduloafore WHERE idcontrol=%i;", CTRL_SVR_INFORMIX);
	CUtileriasAfo::grabarLogx(cRutaLog, cSql);

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
			CFnConsultarResolucionRetAportaciones xSelResol(&odbcIfx);
			snprintf(cSql, sizeof(cSql), "EXECUTE FUNCTION fn_consulta_resolpension_retaportaciones('%s','%s');", cCurpx, cNssx);
			CUtileriasAfo::grabarLogx(cRutaLog, cSql);

			if (xSelResol.Exec(cSql))
			{
				xSelResol.activarCols();
				if (xSelResol.leer())
				{
					iTipoSeguro = xSelResol.iOpcion;
					snprintf(cTipoPension, sizeof(cTipoPension), "%s", xSelResol.tTipo_Pension);
					shRet = CConsultarResolusionRetAportaciones::obtenerTipoSolicitante(iTipoSeguro, cTipoPension);
					// shRet=OK__;
				}
				else
				{
					snprintf(cTexto, sizeof(cTexto), "No hay Registros en la BD de SAFRE CURP-> %s NSS-> %s", cCurpx, cNssx);
					CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
					iResolusion = 0;
					shRet = ERR_NO_HAY_REG_BD;
				}
			}
			else
			{
				shRet = ERR_EXEC_SQL;
				xSelResol.odbc->GetLastError(xSelResol.GetHstmt());
				snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelResol.odbc->LastErrStr());
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
	return shRet;
}

short CConsultarResolusionRetAportaciones::obtenerTipoSolicitante(int iTipoSeguro, char *cTipoPension)
{
	short shRet = DEFAULT__;
	C_SumaRegs xSelResol(&odbcPg);
	snprintf(cSql, sizeof(cSql), "SELECT fnobtenertiposolicitantextipopension AS suma FROM fnobtenertiposolicitantextipopension(%i,'%s');", iTipoSeguro, cTipoPension);
	if (xSelResol.Exec(cSql))
	{
		xSelResol.activarCols();
		if (xSelResol.leer())
		{
			iResolusion = (int)xSelResol.suma;
			shRet = OK__;
		}
		else
		{
			snprintf(cTexto, sizeof(cTexto), "No hay Registros en la BD de SERVICIOSAFORE TipoSeguro-> %s TipoPension-> %s", iTipoSeguro, cTipoPension);
			CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			iResolusion = 0;
			shRet = ERR_NO_HAY_REG_BD;
		}
	}
	else
	{
		shRet = ERR_EXEC_SQL;
		xSelResol.odbc->GetLastError(xSelResol.GetHstmt());
		snprintf(cTexto, sizeof(cTexto), "[%s] Err: %s", __FUNCTION__, xSelResol.odbc->LastErrStr());
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	return shRet;
}

int CConsultarResolusionRetAportaciones::Resolucion()
{
	return iResolusion;
}
