#include "CFoliadorServicios.hpp"
CFoliadorServicios::CFoliadorServicios()
{
	memset(cTexto, 0, sizeof(cTexto));
	memset(cOutTexto, 0, sizeof(cOutTexto));
	memset(cIpServAfo, 0, sizeof(cIpServAfo));
	memset(cIpInfx, 0, sizeof(cIpInfx));
	memset(cRutaLog, 0, sizeof(cRutaLog));
	memset(cSql, 0, sizeof(cSql));
	memset(cBuff, 0, sizeof(cBuff));
	sprintf(cRutaLog, "%s", RUTA_LOGX);
	iFolio=0;
}
CFoliadorServicios::~CFoliadorServicios()
{
	odbcPg.Close();
}
short CFoliadorServicios::ObtenerFolio(short shTipoServicio)
{
	short shRet = DEFAULT__;
	char cTpoServ=' ';

	switch(shTipoServicio)
	{
		case 0: cTpoServ='0'; break;
		case 1: cTpoServ='A'; break;
		case 2: cTpoServ='C'; break;
		case 3: cTpoServ='N'; break;
		case 4: cTpoServ='P'; break;
		case 5: cTpoServ='Q'; break;//se cambia la R por la Q
		case 6: cTpoServ='S'; break;
		default: break;
	}

	sprintf(cTexto, "[%s] ------ Inicia [TipoServ: %i %c]------",  __FUNCTION__, shTipoServicio, cTpoServ);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
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
		if(shRet == OK__)
		{
			sprintf(cSql, "SELECT fnFoliadorServicios from fnFoliadorServicios('%c');", cTpoServ);
			shRet=CBaseDato::consultarNumero(&odbcPg, cSql, iFolio, cOutTexto);
			if(shRet==OK__)
			{
				sprintf(cTexto, "[%s] Folio generado: %i", __FUNCTION__, iFolio);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
			else
			{
				sprintf(cTexto, "[%s] Excep. al obtener folio: %i %s", __FUNCTION__, shRet, cOutTexto);
				CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
			}
		}
	}
	else
	{
		shRet = ERR_LEER_ARCHIVO_CNF;
		sprintf(cTexto, "[%s] Error al leer archivo dat: %s [%s]",  __FUNCTION__, cOutTexto, (char *)IP_SERV_AFO_DAT);
		CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	}

	sprintf(cTexto, "[%s] Retorno: %i",  __FUNCTION__, shRet);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);

	sprintf(cTexto, "[%s] ------ Termina [TipoServ: %i %c]------",  __FUNCTION__, shTipoServicio, cTpoServ);
	CUtileriasAfo::grabarLogx(cRutaLog, cTexto);
	return shRet;
}
int CFoliadorServicios::Folio()
{
	return iFolio;
}

