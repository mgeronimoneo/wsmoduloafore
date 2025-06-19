/*
 * wsModuloAfore.cpp
 *
 *  Created on: 22/04/2015
 *      Author: carlos
 */
#define MAIN
#include "wsModuloAfore.hpp"

int main()
{
	soap_init(&soap);
	obtenerIpRemoto(cIpCliente);
	snprintf(cTexto, sizeof(cTexto), "[%s][%s] Host remoto: %s", __FILE__, __FUNCTION__, cIpCliente);
	CUtileriasAfo::grabarLogx((char *)RUTA_LOGX, cTexto);

	logSoap();
	soap_serve(&soap);	 // serve request, one thread, CGI style abrirLog();  int i;
	soap_destroy(&soap); // dealloc C++ data
	soap_end(&soap);	 // dealloc data and clean up if ( pthread_create( &mythread, NULL, thread_function, NULL) ) {
	soap_done(&soap);	 // detach soap struct

	return 0;
}
void logSoap(void)
{
	char cLog[1024] = {0};
	time_t tTiempo;
	struct tm *tHora;

	tTiempo = time(NULL);
	tHora = localtime(&tTiempo);
	snprintf(cLog, sizeof(cLog), "%s_%02d.txt", RUTA_LOGX, tHora->tm_mday);

	if (freopen(cLog, "a", stderr) == NULL)
	{
		fprintf(stderr, "No se pudo redireccionar stderr a %s", cLog);
	}
}
void obtenerIpRemoto(char *cOutIp)
{
	struct sockaddr sa;
	unsigned int iTam;
	// obtener ip
	iTam = sizeof(struct sockaddr);
	if (getpeername(1, (struct sockaddr *)&sa, &iTam) == 0)
	{
		snprintf(cOutIp, sizeof(cOutIp), "%d.%d.%d.%d", sa.sa_data[2] & 0xFF, sa.sa_data[3] & 0xFF, sa.sa_data[4] & 0xFF, sa.sa_data[5] & 0xFF);
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__consultarAfiliado(struct soap *, ns2__ParamConsultarAfiliado *in, struct ns2__consultarAfiliadoResponse &_param_1)
{
	// Se crea una instancia de la clase contenida en la libreria
	short shRet = DEFAULT__;

	CConsultarAfiliado objConsultaAfil;
	DATOS_TRABAJADOR_EXP_IDEN *stDatosAfil;

	shRet = objConsultaAfil.obtenerInformacionTrabajador(in->iFolioServicio);
	_param_1.out = new ns2__RespConsultarAfiliado();
	_param_1.out->EstadoProc = new ns2__EstadoProceso();
	_param_1.out->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	memset(_param_1.out->EstadoProc->DescripcionEstado, 0, sizeof(LONG_DESCRIPCION));

	if (shRet == OK__)
	{
		// Obtener los datos generados en la instancia
		stDatosAfil = objConsultaAfil.getInformacionTrabajador();

		// Indicamos que todo se realizo con exito
		snprintf(_param_1.out->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "%s", "EXITO");
		_param_1.out->EstadoProc->Estado = OK__;

		// Se instancia el objeto donde se guardaran los datos que se regresara el webservice
		_param_1.out->datosAfiliado = new ns2__Afiliado();

		// Asignar memoria a las variables del objeto
		_param_1.out->datosAfiliado->cApellidoPaterno = new char[sizeof(stDatosAfil->cApellidoPaterno) + 2];
		_param_1.out->datosAfiliado->cApellidoMaterno = new char[sizeof(stDatosAfil->cApellidoMaterno) + 2];
		_param_1.out->datosAfiliado->cNombre = new char[sizeof(stDatosAfil->cNombre) + 2];
		_param_1.out->datosAfiliado->cCurp = new char[sizeof(stDatosAfil->cCurp) + 2];
		_param_1.out->datosAfiliado->cNss = new char[sizeof(stDatosAfil->cNss) + 2];
		_param_1.out->datosAfiliado->cEntidadNacimiento = new char[sizeof(stDatosAfil->cEntidadNacimiento) + 2];
		_param_1.out->datosAfiliado->cFechaNacimiento = new char[sizeof(stDatosAfil->cFechaNacimiento) + 2];
		_param_1.out->datosAfiliado->cFechaServicio = new char[sizeof(stDatosAfil->cFechaServicio) + 2];

		// Inicializar las variables del objeto
		memset(_param_1.out->datosAfiliado->cApellidoPaterno, 0, sizeof(stDatosAfil->cApellidoPaterno) + 2);
		memset(_param_1.out->datosAfiliado->cApellidoMaterno, 0, sizeof(stDatosAfil->cApellidoMaterno) + 2);
		memset(_param_1.out->datosAfiliado->cNombre, 0, sizeof(stDatosAfil->cNombre) + 2);
		memset(_param_1.out->datosAfiliado->cCurp, 0, sizeof(stDatosAfil->cCurp) + 2);
		memset(_param_1.out->datosAfiliado->cNss, 0, sizeof(stDatosAfil->cNss) + 2);
		memset(_param_1.out->datosAfiliado->cEntidadNacimiento, 0, sizeof(stDatosAfil->cEntidadNacimiento) + 2);
		memset(_param_1.out->datosAfiliado->cFechaNacimiento, 0, sizeof(stDatosAfil->cFechaNacimiento) + 2);
		memset(_param_1.out->datosAfiliado->cFechaServicio, 0, sizeof(stDatosAfil->cFechaServicio) + 2);

		// Asignar valores a las variables del objeto
		memmove(_param_1.out->datosAfiliado->cApellidoPaterno, stDatosAfil->cApellidoPaterno, sizeof(stDatosAfil->cApellidoPaterno));
		memmove(_param_1.out->datosAfiliado->cApellidoMaterno, stDatosAfil->cApellidoMaterno, sizeof(stDatosAfil->cApellidoMaterno));
		memmove(_param_1.out->datosAfiliado->cNombre, stDatosAfil->cNombre, sizeof(stDatosAfil->cNombre));
		memmove(_param_1.out->datosAfiliado->cCurp, stDatosAfil->cCurp, sizeof(stDatosAfil->cCurp));
		memmove(_param_1.out->datosAfiliado->cNss, stDatosAfil->cNss, sizeof(stDatosAfil->cNss));
		memmove(_param_1.out->datosAfiliado->cEntidadNacimiento, stDatosAfil->cEntidadNacimiento, sizeof(stDatosAfil->cEntidadNacimiento));
		memmove(_param_1.out->datosAfiliado->cFechaNacimiento, stDatosAfil->cFechaNacimiento, sizeof(stDatosAfil->cFechaNacimiento));
		memmove(_param_1.out->datosAfiliado->cFechaServicio, stDatosAfil->cFechaServicio, sizeof(stDatosAfil->cFechaServicio));
		_param_1.out->datosAfiliado->iSexo = stDatosAfil->iSexo;
	}
	else
	{
		_param_1.out->EstadoProc->Estado = ERR__;
		switch (shRet)
		{
		case ERR_LEER_ARCHIVO_CNF:
			snprintf(_param_1.out->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "%s", "Se presento un error al leer la configuración del servidor, por favor reporte a incidencias");
			break;
		case ERR_EXEC_SQL:
		case ERR_CNX_BASE_DATO:
		case ERR_LEER_REG_BD:
			snprintf(_param_1.out->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "%s", "Se presento un error al consultar los datos del afiliado, por favor reporte a incidencias");
			break;
		case ERR_NO_HAY_REG_BD:
			snprintf(_param_1.out->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "%s", "No se encontro información del afiliado, por favor reporte a incidencias");
			break;
		default:
			snprintf(_param_1.out->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "%s", "Ocurrio un error desconocido, por favor reporte a incidencias");
			break;
		}
	}

	// Liberación manual de memoria para evitar fugas
	if (_param_1.out)
	{
		if (_param_1.out->EstadoProc)
		{
			if (_param_1.out->EstadoProc->DescripcionEstado)
				delete[] _param_1.out->EstadoProc->DescripcionEstado;
			delete _param_1.out->EstadoProc;
		}
		if (_param_1.out->datosAfiliado)
		{
			if (_param_1.out->datosAfiliado->cApellidoPaterno)
				delete[] _param_1.out->datosAfiliado->cApellidoPaterno;
			if (_param_1.out->datosAfiliado->cApellidoMaterno)
				delete[] _param_1.out->datosAfiliado->cApellidoMaterno;
			if (_param_1.out->datosAfiliado->cNombre)
				delete[] _param_1.out->datosAfiliado->cNombre;
			if (_param_1.out->datosAfiliado->cCurp)
				delete[] _param_1.out->datosAfiliado->cCurp;
			if (_param_1.out->datosAfiliado->cNss)
				delete[] _param_1.out->datosAfiliado->cNss;
			if (_param_1.out->datosAfiliado->cEntidadNacimiento)
				delete[] _param_1.out->datosAfiliado->cEntidadNacimiento;
			if (_param_1.out->datosAfiliado->cFechaNacimiento)
				delete[] _param_1.out->datosAfiliado->cFechaNacimiento;
			if (_param_1.out->datosAfiliado->cFechaServicio)
				delete[] _param_1.out->datosAfiliado->cFechaServicio;
			delete _param_1.out->datosAfiliado;
		}
		delete _param_1.out;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__consultarSaldos(struct soap *, ns2__ParametroNss *inParam, struct ns2__consultarSaldosResponse &_param_2)
{
	CSaldosPorSubCuenta objSaldo;
	SALDO_CUENTA *stSaldoCta = NULL;
	short shRet = DEFAULT__, shTotalReg = 0, i = 0, shCont = 0;
	_param_2.outSaldos = new ns2__SaldoPorSubcuenta();
	_param_2.outSaldos->EstadoProc = new ns2__EstadoProceso();
	_param_2.outSaldos->ListaSaldos = new ArrayOfSaldoSubCuenta();
	_param_2.outSaldos->Nss = new char[SIZE_NSS + 2];

	_param_2.outSaldos->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	memset(_param_2.outSaldos->Nss, 0, SIZE_NSS + 2);
	memset(_param_2.outSaldos->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);
	shRet = objSaldo.consultarSaldoPorSubcuenta(inParam->Nss);
	if (shRet == OK__)
	{
		stSaldoCta = objSaldo.SaldoPorSubcuenta();
		for (i = 0; i < NUM_SUBCUENTAS; i++)
		{
			if (stSaldoCta->arrSubCta[i].shSubCuenta > 0)
				shTotalReg++;
		}
		snprintf(_param_2.outSaldos->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		memmove(_param_2.outSaldos->Nss, inParam->Nss, sizeof(SIZE_NSS));
		_param_2.outSaldos->ListaSaldos->__size = shTotalReg;
		_param_2.outSaldos->ListaSaldos->__ptrSaldo = new ns2__SaldoSubCuenta *[shTotalReg];
		for (i = 0; i < NUM_SUBCUENTAS; i++)
		{
			if (stSaldoCta->arrSubCta[i].shSubCuenta > 0)
			{
				_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont] = new ns2__SaldoSubCuenta();
				_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion = new char[SIZE_DECIMAL_];
				_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos = new char[SIZE_DECIMAL_];
				memset(_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion, 0, SIZE_DECIMAL_);
				memset(_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos, 0, SIZE_DECIMAL_);
				_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SubCuenta = stSaldoCta->arrSubCta[i].shSubCuenta;
				_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->Siefore = stSaldoCta->arrSubCta[i].shSiefore;
				snprintf(_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion, sizeof(_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion), "%.06f", stSaldoCta->arrSubCta[i].dMontoAccion);
				snprintf(_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos, sizeof(_param_2.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos), "%.02f", stSaldoCta->arrSubCta[i].dMontoPesos);
				shCont++;
			}
		}
	}
	else
	{
		snprintf(_param_2.outSaldos->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}
	_param_2.outSaldos->EstadoProc->Estado = shRet;

	liberar_memoria_saldos(_param_2.outSaldos);
	_param_2.outSaldos = nullptr; // Opcional, previene double free

	return SOAP_OK;
}
void liberar_memoria_saldos(ns2__SaldoPorSubcuenta *outSaldos)
{
	if (!outSaldos)
		return;

	// Liberar EstadoProc y su miembro dinámico
	if (outSaldos->EstadoProc)
	{
		if (outSaldos->EstadoProc->DescripcionEstado)
			delete[] outSaldos->EstadoProc->DescripcionEstado;
		outSaldos->EstadoProc->DescripcionEstado = nullptr;
		delete outSaldos->EstadoProc;
		outSaldos->EstadoProc = nullptr;
	}

	// Liberar Nss
	if (outSaldos->Nss)
	{
		delete[] outSaldos->Nss;
		outSaldos->Nss = nullptr;
	}

	// Liberar ListaSaldos y cada elemento dinámico
	if (outSaldos->ListaSaldos)
	{
		if (outSaldos->ListaSaldos->__ptrSaldo)
		{
			for (int i = 0; i < outSaldos->ListaSaldos->__size; ++i)
			{
				ns2__SaldoSubCuenta *saldo = outSaldos->ListaSaldos->__ptrSaldo[i];
				if (saldo)
				{
					if (saldo->SaldoAccion)
					{
						delete[] saldo->SaldoAccion;
						saldo->SaldoAccion = nullptr;
					}
					if (saldo->SaldoPesos)
					{
						delete[] saldo->SaldoPesos;
						saldo->SaldoPesos = nullptr;
					}
					delete saldo;
				}
			}
			delete[] outSaldos->ListaSaldos->__ptrSaldo;
			outSaldos->ListaSaldos->__ptrSaldo = nullptr;
		}
		delete outSaldos->ListaSaldos;
		outSaldos->ListaSaldos = nullptr;
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ObtenerFolioServicio(struct soap *, ns2__FoliadorServicio *inFoliador, struct ns2__ObtenerFolioServicioResponse &_param_3)
{
	short shRet = DEFAULT__;
	CFoliadorServicios objFolioServ;
	_param_3.outFolioServ = new ns2__FolioServicio();
	_param_3.outFolioServ->EstadoProc = new ns2__EstadoProceso();
	_param_3.outFolioServ->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	memset(_param_3.outFolioServ->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);
	shRet = objFolioServ.ObtenerFolio(inFoliador->TipoServicio);
	if (shRet == OK__)
	{
		snprintf(_param_3.outFolioServ->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		_param_3.outFolioServ->Folio = objFolioServ.Folio();
	}
	else
		snprintf(_param_3.outFolioServ->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_3.outFolioServ->EstadoProc->Estado = shRet;

	liberar_memoria_folio_servicio(_param_3.outFolioServ);
	_param_3.outFolioServ = nullptr; // Previene double free

	return SOAP_OK;
}
void liberar_memoria_folio_servicio(ns2__FolioServicio *outFolioServ)
{
	if (!outFolioServ)
		return;

	// Liberar EstadoProc y su miembro dinámico
	if (outFolioServ->EstadoProc)
	{
		if (outFolioServ->EstadoProc->DescripcionEstado)
			delete[] outFolioServ->EstadoProc->DescripcionEstado;
		outFolioServ->EstadoProc->DescripcionEstado = nullptr;
		delete outFolioServ->EstadoProc;
		outFolioServ->EstadoProc = nullptr;
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ValidarConvivenciaMarcaOperativa(struct soap *, ns2__ConvivenciaMarcas *inParam, struct ns2__ValidarConvivenciaMarcaOperativaResponse &_param_4)
{
	short shRet = DEFAULT__;
	CValidarConvivenciaMarca objConvMarca;

	_param_4.outParam = new ns2__RespuestaConvivenciaMarca();
	_param_4.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_4.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	_param_4.outParam->ConvivenciaMarca = new ns2__ValidaConvivenciaMarcas();
	_param_4.outParam->ConvivenciaMarca->Descripcion = new char[LONG_DESCRIPCION];
	memset(_param_4.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	shRet = objConvMarca.ValidarConvivencia(inParam->Opcion, inParam->TipoTrabajador, inParam->Nss, inParam->TipoRetiro);
	if (shRet == OK__)
	{
		memset(_param_4.outParam->ConvivenciaMarca->Descripcion, 0, LONG_DESCRIPCION);
		snprintf(_param_4.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		_param_4.outParam->ConvivenciaMarca->Codigo = objConvMarca.ConviveMarcas()->shCodigo;
		_param_4.outParam->ConvivenciaMarca->TieneMarca = objConvMarca.ConviveMarcas()->shTieneMarca;
		snprintf(_param_4.outParam->ConvivenciaMarca->Descripcion, sizeof(LONG_DESCRIPCION), "%s", objConvMarca.ConviveMarcas()->cMensaje);
	}
	else
		snprintf(_param_4.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_4.outParam->EstadoProc->Estado = shRet;

	liberar_memoria_validar_convivencia(_param_4.outParam);
	_param_4.outParam = nullptr; // Previene double free

	return SOAP_OK;
}
void liberar_memoria_validar_convivencia(ns2__RespuestaConvivenciaMarca *outParam)
{
	if (!outParam)
		return;

	// Liberar EstadoProc y su miembro dinámico
	if (outParam->EstadoProc)
	{
		if (outParam->EstadoProc->DescripcionEstado)
			delete[] outParam->EstadoProc->DescripcionEstado;
		outParam->EstadoProc->DescripcionEstado = nullptr;
		delete outParam->EstadoProc;
		outParam->EstadoProc = nullptr;
	}

	// Liberar ConvivenciaMarca y su miembro dinámico
	if (outParam->ConvivenciaMarca)
	{
		if (outParam->ConvivenciaMarca->Descripcion)
			delete[] outParam->ConvivenciaMarca->Descripcion;
		outParam->ConvivenciaMarca->Descripcion = nullptr;
		delete outParam->ConvivenciaMarca;
		outParam->ConvivenciaMarca = nullptr;
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ObtenerSaldoDiarioRetiroParcial(struct soap *, ns2__ConsultaSaldoDiarioRetiroParcial *inParam, struct ns2__ObtenerSaldoDiarioRetiroParcialResponse &_param_5)
{
	short shRet = DEFAULT__;
	CSaldoDiarioRetParcial objSdoDiario;

	_param_5.outParam = new ns2__SaldoDiarioRetiroParcial();
	_param_5.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_5.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	_param_5.outParam->SaldoPesos = new char[LONG_DESCRIPCION];
	memset(_param_5.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);
	memset(_param_5.outParam->SaldoPesos, 0, LONG_DESCRIPCION);

	shRet = objSdoDiario.obtenerSaldoDiarioRetParcial(inParam->Nss, inParam->TipoRetiro);
	if (shRet == OK__)
		snprintf(_param_5.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
	else
		snprintf(_param_5.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_5.outParam->EstadoProc->Estado = shRet;
	snprintf(_param_5.outParam->SaldoPesos, sizeof(_param_5.outParam->SaldoPesos), "%.02f", objSdoDiario.SaldoDiario());

	liberar_memoria_saldo_diario_retiro_parcial(_param_5.outParam);
	_param_5.outParam = nullptr; // Previene double free

	return SOAP_OK;
}
void liberar_memoria_saldo_diario_retiro_parcial(ns2__SaldoDiarioRetiroParcial *outParam)
{
	if (!outParam)
		return;

	// Liberar EstadoProc y su miembro dinámico
	if (outParam->EstadoProc)
	{
		if (outParam->EstadoProc->DescripcionEstado)
			delete[] outParam->EstadoProc->DescripcionEstado;
		outParam->EstadoProc->DescripcionEstado = nullptr;
		delete outParam->EstadoProc;
		outParam->EstadoProc = nullptr;
	}

	// Liberar SaldoPesos
	if (outParam->SaldoPesos)
	{
		delete[] outParam->SaldoPesos;
		outParam->SaldoPesos = nullptr;
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ValidarResolucionImss_(struct soap *, ns2__ParametroNssTipoRetiro *inParam, struct ns2__ValidarResolucionImssResponse &_param_6)
{
	short shRet = DEFAULT__;
	CValidarResolusionImss objValRes;

	_param_6.outParam = new ns2__RespuestaValidarResolucionImss();
	_param_6.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_6.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	_param_6.outParam->ResolucionImss = new ns2__ValidarResolucionImss();
	_param_6.outParam->ResolucionImss->Descripcion = new char[LONG_DESCRIPCION];
	_param_6.outParam->ResolucionImss->FechaProximaSolicitud = new char[SIZE_FECHA];
	_param_6.outParam->ResolucionImss->FechaVigencia = new char[SIZE_FECHA];
	_param_6.outParam->ResolucionImss->FechaVigenciaAnterior = new char[SIZE_FECHA];

	memset(_param_6.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);
	shRet = objValRes.ValidarResolucionImss(inParam->Nss, inParam->TipoRetiro);
	if (shRet == OK__)
	{
		snprintf(_param_6.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");

		memset(_param_6.outParam->ResolucionImss->FechaProximaSolicitud, 0, SIZE_FECHA);
		memset(_param_6.outParam->ResolucionImss->FechaVigencia, 0, SIZE_FECHA);
		memset(_param_6.outParam->ResolucionImss->FechaVigenciaAnterior, 0, SIZE_FECHA);
		memset(_param_6.outParam->ResolucionImss->Descripcion, 0, LONG_DESCRIPCION);
		snprintf(_param_6.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");

		snprintf(_param_6.outParam->ResolucionImss->FechaProximaSolicitud, sizeof(_param_6.outParam->ResolucionImss->FechaProximaSolicitud), "%s", objValRes.ResolucionImss()->cFechaProximaSolicitud);
		snprintf(_param_6.outParam->ResolucionImss->FechaVigencia, sizeof(_param_6.outParam->ResolucionImss->FechaVigencia), "%s", objValRes.ResolucionImss()->cFechaVigencia);
		snprintf(_param_6.outParam->ResolucionImss->FechaVigenciaAnterior, sizeof(_param_6.outParam->ResolucionImss->FechaVigenciaAnterior), "%s", objValRes.ResolucionImss()->cFechaVigenciaAnterior);
		snprintf(_param_6.outParam->ResolucionImss->Descripcion, sizeof(_param_6.outParam->ResolucionImss->Descripcion), "%s", objValRes.ResolucionImss()->cMensaje);
		_param_6.outParam->ResolucionImss->DiagnosticoProcesar = objValRes.ResolucionImss()->iDiagProcesar;
		_param_6.outParam->ResolucionImss->Retorno = objValRes.ResolucionImss()->iRetorno;
	}
	else
		snprintf(_param_6.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_6.outParam->EstadoProc->Estado = shRet;

	liberar_memoria_validar_resolucion_imss(_param_6.outParam);
	_param_6.outParam = nullptr; // Previene double free

	return SOAP_OK;
}
void liberar_memoria_validar_resolucion_imss(ns2__RespuestaValidarResolucionImss *outParam)
{
	if (!outParam)
		return;

	// Liberar EstadoProc y su miembro dinámico
	if (outParam->EstadoProc)
	{
		if (outParam->EstadoProc->DescripcionEstado)
			delete[] outParam->EstadoProc->DescripcionEstado;
		outParam->EstadoProc->DescripcionEstado = nullptr;
		delete outParam->EstadoProc;
		outParam->EstadoProc = nullptr;
	}

	// Liberar ResolucionImss y sus miembros dinámicos
	if (outParam->ResolucionImss)
	{
		if (outParam->ResolucionImss->Descripcion)
			delete[] outParam->ResolucionImss->Descripcion;
		outParam->ResolucionImss->Descripcion = nullptr;

		if (outParam->ResolucionImss->FechaProximaSolicitud)
			delete[] outParam->ResolucionImss->FechaProximaSolicitud;
		outParam->ResolucionImss->FechaProximaSolicitud = nullptr;

		if (outParam->ResolucionImss->FechaVigencia)
			delete[] outParam->ResolucionImss->FechaVigencia;
		outParam->ResolucionImss->FechaVigencia = nullptr;

		if (outParam->ResolucionImss->FechaVigenciaAnterior)
			delete[] outParam->ResolucionImss->FechaVigenciaAnterior;
		outParam->ResolucionImss->FechaVigenciaAnterior = nullptr;

		delete outParam->ResolucionImss;
		outParam->ResolucionImss = nullptr;
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__MontosFechasDatamart_(struct soap *, ns2__ParametroNssTipoRetiro *inParam, struct ns2__MontosFechasDatamartResponse &_param_7)
{
	short shRet = DEFAULT__;
	CMontosFechasDatamart objMontos;

	_param_7.outParam = new ns2__RespuestaMontosFechasDatamart();
	_param_7.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_7.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	_param_7.outParam->MontosFechaDat = new ns2__MontosFechasDatamart();
	_param_7.outParam->MontosFechaDat->FechaInicioVigencia = new char[SIZE_FECHA];
	_param_7.outParam->MontosFechaDat->FechaMatrimonioDesempleo = new char[SIZE_FECHA];
	_param_7.outParam->MontosFechaDat->Nss = new char[SIZE_NSS + 2];
	_param_7.outParam->MontosFechaDat->SalarioBaseA = new char[SIZE_DECIMAL_];
	_param_7.outParam->MontosFechaDat->SalarioBaseB = new char[SIZE_DECIMAL_];
	_param_7.outParam->MontosFechaDat->Ultimas250SemanasSBC = new char[SIZE_DECIMAL_];
	_param_7.outParam->MontosFechaDat->UltimoSalarioBaseCotizado = new char[SIZE_DECIMAL_];

	shRet = objMontos.MontosFechasDatamart(inParam->Nss, inParam->TipoRetiro);
	if (shRet == OK__)
	{
		snprintf(_param_7.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");

		snprintf(_param_7.outParam->MontosFechaDat->FechaInicioVigencia, sizeof(_param_7.outParam->MontosFechaDat->FechaInicioVigencia), "%s", objMontos.MontosDatamart()->cFechaIniVigencia);
		snprintf(_param_7.outParam->MontosFechaDat->FechaMatrimonioDesempleo, sizeof(_param_7.outParam->MontosFechaDat->FechaMatrimonioDesempleo), "%s", objMontos.MontosDatamart()->cFechaMatDes);
		snprintf(_param_7.outParam->MontosFechaDat->Nss, sizeof(_param_7.outParam->MontosFechaDat->Nss), "%s", objMontos.MontosDatamart()->cNss);
		snprintf(_param_7.outParam->MontosFechaDat->SalarioBaseA, sizeof(_param_7.outParam->MontosFechaDat->SalarioBaseA), "%.02f", objMontos.MontosDatamart()->dSalarioBaseA);
		snprintf(_param_7.outParam->MontosFechaDat->SalarioBaseB, sizeof(_param_7.outParam->MontosFechaDat->SalarioBaseB), "%.02f", objMontos.MontosDatamart()->DSalarioBaseB);
		snprintf(_param_7.outParam->MontosFechaDat->Ultimas250SemanasSBC, sizeof(_param_7.outParam->MontosFechaDat->Ultimas250SemanasSBC), "%.02f", objMontos.MontosDatamart()->dUltimas250SemanasSbc);
		snprintf(_param_7.outParam->MontosFechaDat->UltimoSalarioBaseCotizado, sizeof(_param_7.outParam->MontosFechaDat->UltimoSalarioBaseCotizado), "%.02f", objMontos.MontosDatamart()->dUltimaSbc);
	}
	else
		snprintf(_param_7.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_7.outParam->EstadoProc->Estado = shRet;

	liberar_memoria_montos_fechas_datamart(_param_7.outParam);
	_param_7.outParam = nullptr; // Previene double free

	return SOAP_OK;
}
void liberar_memoria_montos_fechas_datamart(ns2__RespuestaMontosFechasDatamart *outParam)
{
	if (!outParam)
		return;

	// Liberar EstadoProc y su miembro dinámico
	if (outParam->EstadoProc)
	{
		if (outParam->EstadoProc->DescripcionEstado)
			delete[] outParam->EstadoProc->DescripcionEstado;
		outParam->EstadoProc->DescripcionEstado = nullptr;
		delete outParam->EstadoProc;
		outParam->EstadoProc = nullptr;
	}

	// Liberar MontosFechaDat y sus miembros dinámicos
	if (outParam->MontosFechaDat)
	{
		if (outParam->MontosFechaDat->FechaInicioVigencia)
			delete[] outParam->MontosFechaDat->FechaInicioVigencia;
		outParam->MontosFechaDat->FechaInicioVigencia = nullptr;

		if (outParam->MontosFechaDat->FechaMatrimonioDesempleo)
			delete[] outParam->MontosFechaDat->FechaMatrimonioDesempleo;
		outParam->MontosFechaDat->FechaMatrimonioDesempleo = nullptr;

		if (outParam->MontosFechaDat->Nss)
			delete[] outParam->MontosFechaDat->Nss;
		outParam->MontosFechaDat->Nss = nullptr;

		if (outParam->MontosFechaDat->SalarioBaseA)
			delete[] outParam->MontosFechaDat->SalarioBaseA;
		outParam->MontosFechaDat->SalarioBaseA = nullptr;

		if (outParam->MontosFechaDat->SalarioBaseB)
			delete[] outParam->MontosFechaDat->SalarioBaseB;
		outParam->MontosFechaDat->SalarioBaseB = nullptr;

		if (outParam->MontosFechaDat->Ultimas250SemanasSBC)
			delete[] outParam->MontosFechaDat->Ultimas250SemanasSBC;
		outParam->MontosFechaDat->Ultimas250SemanasSBC = nullptr;

		if (outParam->MontosFechaDat->UltimoSalarioBaseCotizado)
			delete[] outParam->MontosFechaDat->UltimoSalarioBaseCotizado;
		outParam->MontosFechaDat->UltimoSalarioBaseCotizado = nullptr;

		delete outParam->MontosFechaDat;
		outParam->MontosFechaDat = nullptr;
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__consultarResolucion(struct soap *, ns2__ParametroNssTipoRetiro *inParam, struct ns2__consultarResolucionResponse &_param_8)
{
	short shRet = DEFAULT__;
	CConsultarResolusion objResol;
	_param_8.outParam = new ns2__RespuestaDatosResolucion();
	_param_8.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_8.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	_param_8.outParam->Resolucion = new ns2__DatosResolucion();
	_param_8.outParam->Resolucion->Nss = new char[SIZE_NSS + 2];
	_param_8.outParam->Resolucion->FechaInicioVigencia = new char[SIZE_FECHA];
	_param_8.outParam->Resolucion->FechaFinVigencia = new char[SIZE_FECHA];

	shRet = objResol.ConsultarResolusion(inParam->Nss, inParam->TipoRetiro);
	if (shRet == OK__)
	{
		snprintf(_param_8.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		memset(_param_8.outParam->Resolucion->Nss, 0, SIZE_NSS + 2);
		memset(_param_8.outParam->Resolucion->FechaInicioVigencia, 0, SIZE_FECHA);
		memset(_param_8.outParam->Resolucion->FechaFinVigencia, 0, SIZE_FECHA);

		snprintf(_param_8.outParam->Resolucion->Nss, sizeof(_param_8.outParam->Resolucion->Nss), "%s", objResol.Resolucion()->cNss);
		snprintf(_param_8.outParam->Resolucion->FechaInicioVigencia, sizeof(_param_8.outParam->Resolucion->FechaInicioVigencia), "%s", objResol.Resolucion()->cFechaIniVigencia);
		snprintf(_param_8.outParam->Resolucion->FechaFinVigencia, sizeof(_param_8.outParam->Resolucion->FechaFinVigencia), "%s", objResol.Resolucion()->cFechaFinVigencia);

		_param_8.outParam->Resolucion->NumeroResolucion = objResol.Resolucion()->iResolucion;
	}
	else
		snprintf(_param_8.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_8.outParam->EstadoProc->Estado = shRet;

	liberar_memoria_consultar_resolucion(_param_8.outParam);
	_param_8.outParam = nullptr; // Previene double free

	return SOAP_OK;
}
void liberar_memoria_consultar_resolucion(ns2__RespuestaDatosResolucion *outParam)
{
	if (!outParam)
		return;

	// Liberar EstadoProc y su miembro dinámico
	if (outParam->EstadoProc)
	{
		if (outParam->EstadoProc->DescripcionEstado)
			delete[] outParam->EstadoProc->DescripcionEstado;
		outParam->EstadoProc->DescripcionEstado = nullptr;
		delete outParam->EstadoProc;
		outParam->EstadoProc = nullptr;
	}

	// Liberar Resolucion y sus miembros dinámicos
	if (outParam->Resolucion)
	{
		if (outParam->Resolucion->Nss)
			delete[] outParam->Resolucion->Nss;
		outParam->Resolucion->Nss = nullptr;

		if (outParam->Resolucion->FechaInicioVigencia)
			delete[] outParam->Resolucion->FechaInicioVigencia;
		outParam->Resolucion->FechaInicioVigencia = nullptr;

		if (outParam->Resolucion->FechaFinVigencia)
			delete[] outParam->Resolucion->FechaFinVigencia;
		outParam->Resolucion->FechaFinVigencia = nullptr;

		delete outParam->Resolucion;
		outParam->Resolucion = nullptr;
	}
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultarComplementoResolucion(struct soap *, ns2__ParametroNssTipoRetiro *inParam, struct ns2__ConsultarComplementoResolucionResponse &_param_9)
{
	short shRet = DEFAULT__;
	CComplementoResolucion objComplResol;
	COMPLEMENTO_RESOLUCION *stComplRes = NULL;
	_param_9.outParam = new ns2__RespuestaComplementoResolucion();
	_param_9.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_9.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	_param_9.outParam->ResolucionComplemento = new ns2__ComplementoResolucion();
	_param_9.outParam->ResolucionComplemento->MontoPagado = new char[SIZE_DECIMAL_];
	_param_9.outParam->ResolucionComplemento->MontoPagadoComplemento = new char[SIZE_DECIMAL_];
	_param_9.outParam->ResolucionComplemento->SaldoRCV = new char[SIZE_DECIMAL_];
	_param_9.outParam->ResolucionComplemento->SaldoRCVAnterior = new char[SIZE_DECIMAL_];

	shRet = objComplResol.ConsultarComplementoResolusion(inParam->Nss, inParam->TipoRetiro);
	if (shRet == OK__)
	{
		snprintf(_param_9.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stComplRes = objComplResol.ResolucionComplemento();
		memset(_param_9.outParam->ResolucionComplemento->MontoPagado, 0, SIZE_DECIMAL_);
		memset(_param_9.outParam->ResolucionComplemento->MontoPagadoComplemento, 0, SIZE_DECIMAL_);
		memset(_param_9.outParam->ResolucionComplemento->SaldoRCV, 0, SIZE_DECIMAL_);
		memset(_param_9.outParam->ResolucionComplemento->SaldoRCVAnterior, 0, SIZE_DECIMAL_);

		_param_9.outParam->ResolucionComplemento->IdentificadorComplemento = stComplRes->iIdComplemento;
		_param_9.outParam->ResolucionComplemento->NumeroResolucion = stComplRes->iNumResolucion;
		_param_9.outParam->ResolucionComplemento->RespuestaOtroDatamart = stComplRes->shRespOtroDatamart;
		snprintf(_param_9.outParam->ResolucionComplemento->MontoPagado, sizeof(_param_9.outParam->ResolucionComplemento->MontoPagado), "%.02f", stComplRes->dMontoPagado);
		snprintf(_param_9.outParam->ResolucionComplemento->MontoPagadoComplemento, sizeof(_param_9.outParam->ResolucionComplemento->MontoPagadoComplemento), "%.02f", stComplRes->dMontoPagadoComplemento);
		snprintf(_param_9.outParam->ResolucionComplemento->SaldoRCV, sizeof(_param_9.outParam->ResolucionComplemento->SaldoRCV), "%.02f", stComplRes->dSaldoRcv);
		snprintf(_param_9.outParam->ResolucionComplemento->SaldoRCVAnterior, sizeof(_param_9.outParam->ResolucionComplemento->SaldoRCVAnterior), "%.02f", stComplRes->dSaldoRcvAnterior);
	}
	else
		snprintf(_param_9.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_9.outParam->EstadoProc->Estado = shRet;

	// Liberación manual de memoria
	if (_param_9.outParam)
	{
		if (_param_9.outParam->EstadoProc)
		{
			if (_param_9.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_9.outParam->EstadoProc->DescripcionEstado;
			delete _param_9.outParam->EstadoProc;
		}
		if (_param_9.outParam->ResolucionComplemento)
		{
			if (_param_9.outParam->ResolucionComplemento->MontoPagado)
				delete[] _param_9.outParam->ResolucionComplemento->MontoPagado;
			if (_param_9.outParam->ResolucionComplemento->MontoPagadoComplemento)
				delete[] _param_9.outParam->ResolucionComplemento->MontoPagadoComplemento;
			if (_param_9.outParam->ResolucionComplemento->SaldoRCV)
				delete[] _param_9.outParam->ResolucionComplemento->SaldoRCV;
			if (_param_9.outParam->ResolucionComplemento->SaldoRCVAnterior)
				delete[] _param_9.outParam->ResolucionComplemento->SaldoRCVAnterior;
			delete _param_9.outParam->ResolucionComplemento;
		}
		delete _param_9.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultarResolucionTotalImss(struct soap *, ns2__ParamResolucionTotalImss *inParam, struct ns2__ConsultarResolucionTotalImssResponse &_param_10)
{
	// pega tu codigo y cambia los nombres de las variables
	short shRet = DEFAULT__;
	CConsultarResolucionTotalImss objResolTotal; // objComplResol;
	CONSULTA_RESOLUCION_TOTAL_IMSS *stResolTotal = NULL;

	// Se crea una instancia del objeto en el cual se recibiran los datos que se retornará.
	// Nota: Es similar a un arreglo y sus elementos estan definidos en el wsdl del WS.
	_param_10.outParam = new ns2__RespuestaResolucionTotalImss();

	// Se crea una instancia del objeto que recibirá los datos de EstadoProceso y se inicializan sus elementos.
	// Nota: Solo se asigna memoria a los que son CHAR.
	_param_10.outParam->EstadoProc = new ns2__EstadoProceso();
	// Asignar memoria a las variables del objeto ns2__EstadoProceso.
	_param_10.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	// Se crea una instancia del objeto que recibirá los datos de ResolucionTotalImss y se asigna memoria a sus elementos.
	// Nota: Solo se asigna memoria a los que son CHAR.
	_param_10.outParam->ResolTotalImss = new ns2__ResolucionTotalImss();
	// Asignar memoria a las variables del objeto ns2__ResolucionTotalImss.
	_param_10.outParam->ResolTotalImss->Regimen = new char[sizeof(stResolTotal->cRegimen)];
	_param_10.outParam->ResolTotalImss->TipoSeguro = new char[sizeof(stResolTotal->cTipoSeguro)];
	_param_10.outParam->ResolTotalImss->TipoPension = new char[sizeof(stResolTotal->cTipoPension)];
	_param_10.outParam->ResolTotalImss->NombreDatamart = new char[sizeof(stResolTotal->cNombreDatamart)];
	_param_10.outParam->ResolTotalImss->SecPension = new char[sizeof(stResolTotal->cSecPension)];
	_param_10.outParam->ResolTotalImss->FechaInicio = new char[SIZE_FECHA];
	_param_10.outParam->ResolTotalImss->FechaPago = new char[SIZE_FECHA];
	_param_10.outParam->ResolTotalImss->FechaResolucion = new char[SIZE_FECHA];
	_param_10.outParam->ResolTotalImss->DescRegimen = new char[sizeof(stResolTotal->cDescripRegimen)];
	_param_10.outParam->ResolTotalImss->DescTipoPension = new char[sizeof(stResolTotal->cDescripTPension)];
	_param_10.outParam->ResolTotalImss->DescTipoPrestacion = new char[sizeof(stResolTotal->cDescripTPresta)];
	_param_10.outParam->ResolTotalImss->DescTipoRetiro = new char[sizeof(stResolTotal->cDescripTRetiro)];
	_param_10.outParam->ResolTotalImss->DescTipoSeguro = new char[sizeof(stResolTotal->cDescripTSeguro)];
	_param_10.outParam->ResolTotalImss->DescEdoSubViv = new char[sizeof(stResolTotal->cDescripEdoSubViv)];

	// Se manda llamar el metodo que ejecuta la consulta y si lo hace correctamente
	shRet = objResolTotal.ConsultarResolucionTotalImss(inParam->Opcion, inParam->Nss, inParam->Curp);
	if (shRet == OK__)
	{
		snprintf(_param_10.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stResolTotal = objResolTotal.ResolucionTotalImss();

		// Inicializar las variables del objeto.
		memset(_param_10.outParam->ResolTotalImss->Regimen, 0, sizeof(stResolTotal->cRegimen));
		memset(_param_10.outParam->ResolTotalImss->TipoSeguro, 0, sizeof(stResolTotal->cTipoSeguro));
		memset(_param_10.outParam->ResolTotalImss->TipoPension, 0, sizeof(stResolTotal->cTipoPension));
		memset(_param_10.outParam->ResolTotalImss->NombreDatamart, 0, sizeof(stResolTotal->cNombreDatamart));
		memset(_param_10.outParam->ResolTotalImss->SecPension, 0, sizeof(stResolTotal->cSecPension));
		memset(_param_10.outParam->ResolTotalImss->FechaInicio, 0, SIZE_FECHA);
		memset(_param_10.outParam->ResolTotalImss->FechaPago, 0, SIZE_FECHA);
		memset(_param_10.outParam->ResolTotalImss->FechaResolucion, 0, SIZE_FECHA);
		memset(_param_10.outParam->ResolTotalImss->DescTipoPension, 0, sizeof(stResolTotal->cDescripTPension));
		memset(_param_10.outParam->ResolTotalImss->DescRegimen, 0, sizeof(stResolTotal->cDescripRegimen));
		memset(_param_10.outParam->ResolTotalImss->DescTipoPrestacion, 0, sizeof(stResolTotal->cDescripTPresta));
		memset(_param_10.outParam->ResolTotalImss->DescTipoRetiro, 0, sizeof(stResolTotal->cDescripTRetiro));
		memset(_param_10.outParam->ResolTotalImss->DescTipoSeguro, 0, sizeof(stResolTotal->cDescripTSeguro));
		memset(_param_10.outParam->ResolTotalImss->DescEdoSubViv, 0, sizeof(stResolTotal->cDescripEdoSubViv));

		// Asignar valores a las variables de la estructura obtenidas a traves de la consulta al objeto
		_param_10.outParam->ResolTotalImss->ExisteResolucion = stResolTotal->iExisteResolucion;
		_param_10.outParam->ResolTotalImss->ExisteAfiliado = stResolTotal->iExisteAfiliado;
		snprintf(_param_10.outParam->ResolTotalImss->Regimen, sizeof(_param_10.outParam->ResolTotalImss->Regimen), "%s", stResolTotal->cRegimen);
		snprintf(_param_10.outParam->ResolTotalImss->TipoSeguro, sizeof(_param_10.outParam->ResolTotalImss->TipoSeguro), "%s", stResolTotal->cTipoSeguro);
		snprintf(_param_10.outParam->ResolTotalImss->TipoPension, sizeof(_param_10.outParam->ResolTotalImss->TipoPension), "%s", stResolTotal->cTipoPension);
		snprintf(_param_10.outParam->ResolTotalImss->NombreDatamart, sizeof(_param_10.outParam->ResolTotalImss->NombreDatamart), "%s", stResolTotal->cNombreDatamart);
		_param_10.outParam->ResolTotalImss->TipoPrestacion = stResolTotal->iTipoPrestacion;
		_param_10.outParam->ResolTotalImss->Folio = stResolTotal->iFolio;
		snprintf(_param_10.outParam->ResolTotalImss->SecPension, sizeof(_param_10.outParam->ResolTotalImss->SecPension), "%s", stResolTotal->cSecPension);
		snprintf(_param_10.outParam->ResolTotalImss->FechaInicio, sizeof(_param_10.outParam->ResolTotalImss->FechaInicio), "%s", stResolTotal->dFechaIni);
		snprintf(_param_10.outParam->ResolTotalImss->FechaPago, sizeof(_param_10.outParam->ResolTotalImss->FechaPago), "%s", stResolTotal->dFechaPago);
		_param_10.outParam->ResolTotalImss->SemanasCotizadas = stResolTotal->iSemanasCotizadas;
		_param_10.outParam->ResolTotalImss->DiagDatamart = stResolTotal->iDiagDatamart;
		_param_10.outParam->ResolTotalImss->EstadoSubViv = stResolTotal->iEstadoSubViv;
		snprintf(_param_10.outParam->ResolTotalImss->FechaResolucion, sizeof(_param_10.outParam->ResolTotalImss->FechaResolucion), "%s", stResolTotal->dFechaResolucion);
		snprintf(_param_10.outParam->ResolTotalImss->DescRegimen, sizeof(_param_10.outParam->ResolTotalImss->DescRegimen), "%s", stResolTotal->cDescripRegimen);
		snprintf(_param_10.outParam->ResolTotalImss->DescTipoPension, sizeof(_param_10.outParam->ResolTotalImss->DescTipoPension), "%s", stResolTotal->cDescripTPension);
		snprintf(_param_10.outParam->ResolTotalImss->DescTipoPrestacion, sizeof(_param_10.outParam->ResolTotalImss->DescTipoPrestacion), "%s", stResolTotal->cDescripTPresta);
		snprintf(_param_10.outParam->ResolTotalImss->DescTipoRetiro, sizeof(_param_10.outParam->ResolTotalImss->DescTipoRetiro), "%s", stResolTotal->cDescripTRetiro);
		snprintf(_param_10.outParam->ResolTotalImss->DescTipoSeguro, sizeof(_param_10.outParam->ResolTotalImss->DescTipoSeguro), "%s", stResolTotal->cDescripTSeguro);
		snprintf(_param_10.outParam->ResolTotalImss->DescEdoSubViv, sizeof(_param_10.outParam->ResolTotalImss->DescEdoSubViv), "%s", stResolTotal->cDescripEdoSubViv);
		_param_10.outParam->ResolTotalImss->IdMovimiento = stResolTotal->iIdMovimiento;
	}
	else
		snprintf(_param_10.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_10.outParam->EstadoProc->Estado = shRet;

	// Liberación manual de memoria para evitar fugas
	if (_param_10.outParam)
	{
		if (_param_10.outParam->EstadoProc)
		{
			if (_param_10.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_10.outParam->EstadoProc->DescripcionEstado;
			delete _param_10.outParam->EstadoProc;
		}
		if (_param_10.outParam->ResolTotalImss)
		{
			if (_param_10.outParam->ResolTotalImss->Regimen)
				delete[] _param_10.outParam->ResolTotalImss->Regimen;
			if (_param_10.outParam->ResolTotalImss->TipoSeguro)
				delete[] _param_10.outParam->ResolTotalImss->TipoSeguro;
			if (_param_10.outParam->ResolTotalImss->TipoPension)
				delete[] _param_10.outParam->ResolTotalImss->TipoPension;
			if (_param_10.outParam->ResolTotalImss->NombreDatamart)
				delete[] _param_10.outParam->ResolTotalImss->NombreDatamart;
			if (_param_10.outParam->ResolTotalImss->SecPension)
				delete[] _param_10.outParam->ResolTotalImss->SecPension;
			if (_param_10.outParam->ResolTotalImss->FechaInicio)
				delete[] _param_10.outParam->ResolTotalImss->FechaInicio;
			if (_param_10.outParam->ResolTotalImss->FechaPago)
				delete[] _param_10.outParam->ResolTotalImss->FechaPago;
			if (_param_10.outParam->ResolTotalImss->FechaResolucion)
				delete[] _param_10.outParam->ResolTotalImss->FechaResolucion;
			if (_param_10.outParam->ResolTotalImss->DescRegimen)
				delete[] _param_10.outParam->ResolTotalImss->DescRegimen;
			if (_param_10.outParam->ResolTotalImss->DescTipoPension)
				delete[] _param_10.outParam->ResolTotalImss->DescTipoPension;
			if (_param_10.outParam->ResolTotalImss->DescTipoPrestacion)
				delete[] _param_10.outParam->ResolTotalImss->DescTipoPrestacion;
			if (_param_10.outParam->ResolTotalImss->DescTipoRetiro)
				delete[] _param_10.outParam->ResolTotalImss->DescTipoRetiro;
			if (_param_10.outParam->ResolTotalImss->DescTipoSeguro)
				delete[] _param_10.outParam->ResolTotalImss->DescTipoSeguro;
			if (_param_10.outParam->ResolTotalImss->DescEdoSubViv)
				delete[] _param_10.outParam->ResolTotalImss->DescEdoSubViv;
			delete _param_10.outParam->ResolTotalImss;
		}
		delete _param_10.outParam;
	}

	return SOAP_OK;
}

// Agregado para el saldo del servicio retiro PMG(Pension Minima Garantizada)
SOAP_FMAC5 int SOAP_FMAC6 ns2__COperacionesPmg(struct soap *, ns2__ParamOperacionPmgImss *inParam, struct ns2__COperacionesPmgResponse &_param_11)
{
	COperacionesPmg objOperacionPmg;
	OPERACIONES_PMG_IMSS *stOperaciones = NULL;
	short shRet = DEFAULT__;

	// Se tienen que crear instancias para recesarvar memoria para estos datos en gsoapcliente
	_param_11.outParam = new ns2__RespuestaCOperacionesPmgImss();

	_param_11.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_11.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	memset(_param_11.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	_param_11.outParam->ResolOperacionPmg = new ns2__ResolucionOperacionPmg();
	// Asignar memoria a las variables del objeto ns2__ResolucionTotalImss.
	_param_11.outParam->ResolOperacionPmg->Mensaje = new char[sizeof(stOperaciones->cMensaje)];

	shRet = objOperacionPmg.ConsultaInsuficiencia(inParam->Nss, inParam->Secuencia, inParam->Folio, inParam->iOpcion);
	if (shRet == OK__)
	{
		snprintf(_param_11.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stOperaciones = objOperacionPmg.OperacionesPmg();

		memset(_param_11.outParam->ResolOperacionPmg->Mensaje, 0, sizeof(stOperaciones->cMensaje));

		// Asignamos el valor retornado de la consulta a la estructura y posteriormente retornarlo
		snprintf(_param_11.outParam->ResolOperacionPmg->Mensaje, sizeof(_param_11.outParam->ResolOperacionPmg->Mensaje), "%s", stOperaciones->cMensaje);
		_param_11.outParam->ResolOperacionPmg->Codigo = stOperaciones->sCodigo;
		_param_11.outParam->ResolOperacionPmg->Importe = stOperaciones->dImporte;
		// sprintf(_param_11.outParam->ResolOperacionPmg->Importe, "%.02f", stOperaciones->dImporte);
	}
	else
	{
		snprintf(_param_11.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}
	_param_11.outParam->EstadoProc->Estado = shRet;

	// Liberación manual de memoria para evitar fugas
	if (_param_11.outParam)
	{
		if (_param_11.outParam->EstadoProc)
		{
			if (_param_11.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_11.outParam->EstadoProc->DescripcionEstado;
			delete _param_11.outParam->EstadoProc;
		}
		if (_param_11.outParam->ResolOperacionPmg)
		{
			if (_param_11.outParam->ResolOperacionPmg->Mensaje)
				delete[] _param_11.outParam->ResolOperacionPmg->Mensaje;
			delete _param_11.outParam->ResolOperacionPmg;
		}
		delete _param_11.outParam;
	}

	return SOAP_OK;
}

// Se consultan los movimientos de las cuentas (1 a 6)
SOAP_FMAC5 int SOAP_FMAC6 ns2__movimientosCuentas(struct soap *, ns2__ParamMovimientosCuentas *inParam, struct ns2__movimientosCuentasResponse &_param_12)
{
	short shRet = DEFAULT__;
	CMovimientosCuentas objMovimientosCuenta;
	_param_12.outParam = new ns2__RespuestaMovimientosCuentas();
	_param_12.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_12.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	_param_12.outParam->RespMovimientosCuentas = new ns2__MovimientosCuentas();

	memset(_param_12.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);
	shRet = objMovimientosCuenta.ObtenerMovimientos(inParam->Nss);
	if (shRet == OK__)
	{
		snprintf(_param_12.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		_param_12.outParam->RespMovimientosCuentas->Movimientos = objMovimientosCuenta.movimientos();
	}
	else
		snprintf(_param_12.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_12.outParam->EstadoProc->Estado = shRet;

	// Liberación manual de memoria para evitar fugas
	if (_param_12.outParam)
	{
		if (_param_12.outParam->EstadoProc)
		{
			if (_param_12.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_12.outParam->EstadoProc->DescripcionEstado;
			delete _param_12.outParam->EstadoProc;
		}
		if (_param_12.outParam->RespMovimientosCuentas)
		{
			// ATENCIÓN: solo liberar Movimientos si fue asignado con new/new[] aquí.
			// Si lo administra la clase CMovimientosCuentas, NO lo borres aquí.
			// Ejemplo: delete[] _param_12.outParam->RespMovimientosCuentas->Movimientos;
			delete _param_12.outParam->RespMovimientosCuentas;
		}
		delete _param_12.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultarResolucionTotalIssste(struct soap *, ns2__ParamResolucionTotalIssste *inParam, struct ns2__ConsultarResolucionTotalIsssteResponse &_param_13)
{
	// pega tu codigo y cambia los nombres de las variables
	short shRet = DEFAULT__;
	CConsultarResolucionTotalIssste objResolTotal; // objComplResol;
	DATOS_DATAMART_ISSSTE *stResolTotal = NULL;

	// Se crea una instancia del objeto en el cual se recibiran los datos que se retornará.
	// Nota: Es similar a un arreglo y sus elementos estan definidos en el wsdl del WS.
	_param_13.outParam = new ns2__RespuestaResolucionTotalIssste();

	// Se crea una instancia del objeto que recibirá los datos de EstadoProceso y se inicializan sus elementos.
	// Nota: Solo se asigna memoria a los que son CHAR.
	_param_13.outParam->EstadoProc = new ns2__EstadoProceso();
	// Asignar memoria a las variables del objeto ns2__EstadoProceso.
	_param_13.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	// Se crea una instancia del objeto que recibirá los datos de ResolucionTotalImss y se asigna memoria a sus elementos.
	// Nota: Solo se asigna memoria a los que son CHAR.
	_param_13.outParam->RespResolucionTotalIssste = new ns2__ResolucionTotalIssste();
	// Asignar memoria a las variables del objeto ns2__ResolucionTotalImss.
	_param_13.outParam->RespResolucionTotalIssste->Nss = new char[sizeof(stResolTotal->cNss)];
	_param_13.outParam->RespResolucionTotalIssste->NssIssste = new char[sizeof(stResolTotal->cNssissste)];
	_param_13.outParam->RespResolucionTotalIssste->Curp = new char[sizeof(stResolTotal->cCurp)];
	_param_13.outParam->RespResolucionTotalIssste->SecPension = new char[sizeof(stResolTotal->cSecpension)];
	_param_13.outParam->RespResolucionTotalIssste->NombreDatamart = new char[sizeof(stResolTotal->cNombreDatamart)];
	_param_13.outParam->RespResolucionTotalIssste->PaternoDatamart = new char[sizeof(stResolTotal->cPaternoDatamart)];
	_param_13.outParam->RespResolucionTotalIssste->MaternoDatamart = new char[sizeof(stResolTotal->cMaternoDatamart)];
	_param_13.outParam->RespResolucionTotalIssste->NumConcesion = new char[sizeof(stResolTotal->cNumconcesion)];
	_param_13.outParam->RespResolucionTotalIssste->TipoRetiro = new char[sizeof(stResolTotal->cTiporetiro)];
	_param_13.outParam->RespResolucionTotalIssste->Regimen = new char[sizeof(stResolTotal->cRegimen)];
	_param_13.outParam->RespResolucionTotalIssste->TipoSeguro = new char[sizeof(stResolTotal->cTiposeguro)];
	_param_13.outParam->RespResolucionTotalIssste->TipoPension = new char[sizeof(stResolTotal->cTipopension)];
	_param_13.outParam->RespResolucionTotalIssste->CvePension = new char[sizeof(stResolTotal->cCvepension)];
	_param_13.outParam->RespResolucionTotalIssste->FechaInicioPen = new char[SIZE_FECHA];
	_param_13.outParam->RespResolucionTotalIssste->FechaResolucion = new char[SIZE_FECHA];

	// Se manda llamar el metodo que ejecuta la consulta y si lo hace correctamente
	shRet = objResolTotal.consultaDatamart(inParam->Curp, inParam->TipoRetiro);
	if (shRet == OK__)
	{
		snprintf(_param_13.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stResolTotal = objResolTotal.getInformacionDataMart();

		// Inicializar las variables del objeto.
		memset(_param_13.outParam->RespResolucionTotalIssste->Nss, 0, sizeof(stResolTotal->cNss));
		memset(_param_13.outParam->RespResolucionTotalIssste->NssIssste, 0, sizeof(stResolTotal->cNssissste));
		memset(_param_13.outParam->RespResolucionTotalIssste->Curp, 0, sizeof(stResolTotal->cCurp));
		memset(_param_13.outParam->RespResolucionTotalIssste->SecPension, 0, sizeof(stResolTotal->cSecpension));
		memset(_param_13.outParam->RespResolucionTotalIssste->NombreDatamart, 0, sizeof(stResolTotal->cNombreDatamart));
		memset(_param_13.outParam->RespResolucionTotalIssste->PaternoDatamart, 0, sizeof(stResolTotal->cPaternoDatamart));
		memset(_param_13.outParam->RespResolucionTotalIssste->MaternoDatamart, 0, sizeof(stResolTotal->cMaternoDatamart));
		memset(_param_13.outParam->RespResolucionTotalIssste->NumConcesion, 0, sizeof(stResolTotal->cNumconcesion));
		memset(_param_13.outParam->RespResolucionTotalIssste->TipoRetiro, 0, sizeof(stResolTotal->cTiporetiro));
		memset(_param_13.outParam->RespResolucionTotalIssste->Regimen, 0, sizeof(stResolTotal->cRegimen));
		memset(_param_13.outParam->RespResolucionTotalIssste->TipoSeguro, 0, sizeof(stResolTotal->cTiposeguro));
		memset(_param_13.outParam->RespResolucionTotalIssste->TipoPension, 0, sizeof(stResolTotal->cTipopension));
		memset(_param_13.outParam->RespResolucionTotalIssste->CvePension, 0, sizeof(stResolTotal->cCvepension));
		memset(_param_13.outParam->RespResolucionTotalIssste->FechaInicioPen, 0, sizeof(SIZE_FECHA));
		memset(_param_13.outParam->RespResolucionTotalIssste->FechaResolucion, 0, sizeof(SIZE_FECHA));

		// Asignar valores a las variables de la estructura obtenidas a traves de la consulta al objeto
		snprintf(_param_13.outParam->RespResolucionTotalIssste->Nss, sizeof(_param_13.outParam->RespResolucionTotalIssste->Nss), "%s", stResolTotal->cNss);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->NssIssste, sizeof(_param_13.outParam->RespResolucionTotalIssste->NssIssste), "%s", stResolTotal->cNssissste);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->Curp, sizeof(_param_13.outParam->RespResolucionTotalIssste->Curp), "%s", stResolTotal->cCurp);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->SecPension, sizeof(_param_13.outParam->RespResolucionTotalIssste->SecPension), "%s", stResolTotal->cSecpension);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->NombreDatamart, sizeof(_param_13.outParam->RespResolucionTotalIssste->NombreDatamart), "%s", stResolTotal->cNombreDatamart);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->PaternoDatamart, sizeof(_param_13.outParam->RespResolucionTotalIssste->PaternoDatamart), "%s", stResolTotal->cPaternoDatamart);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->MaternoDatamart, sizeof(_param_13.outParam->RespResolucionTotalIssste->MaternoDatamart), "%s", stResolTotal->cMaternoDatamart);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->NumConcesion, sizeof(_param_13.outParam->RespResolucionTotalIssste->NumConcesion), "%s", stResolTotal->cNumconcesion);

		snprintf(_param_13.outParam->RespResolucionTotalIssste->TipoRetiro, sizeof(_param_13.outParam->RespResolucionTotalIssste->TipoRetiro), "%s", stResolTotal->cTiporetiro);

		snprintf(_param_13.outParam->RespResolucionTotalIssste->Regimen, sizeof(_param_13.outParam->RespResolucionTotalIssste->Regimen), "%s", stResolTotal->cRegimen);

		snprintf(_param_13.outParam->RespResolucionTotalIssste->TipoSeguro, sizeof(_param_13.outParam->RespResolucionTotalIssste->TipoSeguro), "%s", stResolTotal->cTiposeguro);

		snprintf(_param_13.outParam->RespResolucionTotalIssste->TipoPension, sizeof(_param_13.outParam->RespResolucionTotalIssste->TipoPension), "%s", stResolTotal->cTipopension);

		snprintf(_param_13.outParam->RespResolucionTotalIssste->CvePension, sizeof(_param_13.outParam->RespResolucionTotalIssste->CvePension), "%s", stResolTotal->cCvepension);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->FechaInicioPen, sizeof(_param_13.outParam->RespResolucionTotalIssste->FechaInicioPen), "%s", stResolTotal->cFechainiciopen);
		snprintf(_param_13.outParam->RespResolucionTotalIssste->FechaResolucion, sizeof(_param_13.outParam->RespResolucionTotalIssste->FechaResolucion), "%s", stResolTotal->cFecharesolucion);
		_param_13.outParam->RespResolucionTotalIssste->Delegacion = stResolTotal->shDelegacion;
		_param_13.outParam->RespResolucionTotalIssste->TipoMovimiento = stResolTotal->shTipomovimiento;
		_param_13.outParam->RespResolucionTotalIssste->TipoPrestacion = stResolTotal->shTipoprestacion;
		_param_13.outParam->RespResolucionTotalIssste->SemanasCotizadas = stResolTotal->shSemanascotizadas;
		_param_13.outParam->RespResolucionTotalIssste->DiagDatamart = stResolTotal->shDiagDatamart;
		_param_13.outParam->RespResolucionTotalIssste->Estadoregistro = stResolTotal->shEstadoregistro;
		_param_13.outParam->RespResolucionTotalIssste->Folio = stResolTotal->iFolio;
	}
	else
		snprintf(_param_13.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_13.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_13.outParam)
	{
		if (_param_13.outParam->EstadoProc)
		{
			if (_param_13.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_13.outParam->EstadoProc->DescripcionEstado;
			delete _param_13.outParam->EstadoProc;
		}
		if (_param_13.outParam->RespResolucionTotalIssste)
		{
			if (_param_13.outParam->RespResolucionTotalIssste->Nss)
				delete[] _param_13.outParam->RespResolucionTotalIssste->Nss;
			if (_param_13.outParam->RespResolucionTotalIssste->NssIssste)
				delete[] _param_13.outParam->RespResolucionTotalIssste->NssIssste;
			if (_param_13.outParam->RespResolucionTotalIssste->Curp)
				delete[] _param_13.outParam->RespResolucionTotalIssste->Curp;
			if (_param_13.outParam->RespResolucionTotalIssste->SecPension)
				delete[] _param_13.outParam->RespResolucionTotalIssste->SecPension;
			if (_param_13.outParam->RespResolucionTotalIssste->NombreDatamart)
				delete[] _param_13.outParam->RespResolucionTotalIssste->NombreDatamart;
			if (_param_13.outParam->RespResolucionTotalIssste->PaternoDatamart)
				delete[] _param_13.outParam->RespResolucionTotalIssste->PaternoDatamart;
			if (_param_13.outParam->RespResolucionTotalIssste->MaternoDatamart)
				delete[] _param_13.outParam->RespResolucionTotalIssste->MaternoDatamart;
			if (_param_13.outParam->RespResolucionTotalIssste->NumConcesion)
				delete[] _param_13.outParam->RespResolucionTotalIssste->NumConcesion;
			if (_param_13.outParam->RespResolucionTotalIssste->TipoRetiro)
				delete[] _param_13.outParam->RespResolucionTotalIssste->TipoRetiro;
			if (_param_13.outParam->RespResolucionTotalIssste->Regimen)
				delete[] _param_13.outParam->RespResolucionTotalIssste->Regimen;
			if (_param_13.outParam->RespResolucionTotalIssste->TipoSeguro)
				delete[] _param_13.outParam->RespResolucionTotalIssste->TipoSeguro;
			if (_param_13.outParam->RespResolucionTotalIssste->TipoPension)
				delete[] _param_13.outParam->RespResolucionTotalIssste->TipoPension;
			if (_param_13.outParam->RespResolucionTotalIssste->CvePension)
				delete[] _param_13.outParam->RespResolucionTotalIssste->CvePension;
			if (_param_13.outParam->RespResolucionTotalIssste->FechaInicioPen)
				delete[] _param_13.outParam->RespResolucionTotalIssste->FechaInicioPen;
			if (_param_13.outParam->RespResolucionTotalIssste->FechaResolucion)
				delete[] _param_13.outParam->RespResolucionTotalIssste->FechaResolucion;
			delete _param_13.outParam->RespResolucionTotalIssste;
		}
		delete _param_13.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__consultarSaldosPmg(struct soap *, ns2__ParametroSaldoPmg *inParam, struct ns2__consultarSaldosPmgResponse &_param_14)
{
	CSaldosPorSubCuentaPmg objSaldo;
	SALDOS_PMG *stSaldosPmg = NULL;
	short shRet = DEFAULT__;
	_param_14.outSaldosPmg = new ns2__SaldoPorSubcuentaPmg();

	_param_14.outSaldosPmg->EstadoProc = new ns2__EstadoProceso();
	_param_14.outSaldosPmg->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION]; // ns2__SaldoPorSubcuentaPmg ns2__SaldosCuentasPmg
	memset(_param_14.outSaldosPmg->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	_param_14.outSaldosPmg->SaldosPmg = new ns2__SaldosCuentasPmg();

	shRet = objSaldo.consultarSaldoPorSubcuentaPmg(inParam->Nss, inParam->Secuencia, inParam->EstadoViv);
	if (shRet == OK__)
	{
		snprintf(_param_14.outSaldosPmg->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stSaldosPmg = objSaldo.SaldoPorSubcuentaPmg();

		// Asignamos el valor retornado de la consulta a la estructura y posteriormente retornarlo
		_param_14.outSaldosPmg->SaldosPmg->Retroactivo = stSaldosPmg->dRetroactivo;
		_param_14.outSaldosPmg->SaldosPmg->Devengado = stSaldosPmg->dDevengado;
		_param_14.outSaldosPmg->SaldosPmg->SaldoDia = stSaldosPmg->dSaldoDia;
		_param_14.outSaldosPmg->SaldosPmg->Suficiencia = stSaldosPmg->iSuficiencia;
		_param_14.outSaldosPmg->SaldosPmg->Registros = stSaldosPmg->iRegistros;
	}
	else
	{
		snprintf(_param_14.outSaldosPmg->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}
	_param_14.outSaldosPmg->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_14.outSaldosPmg)
	{
		if (_param_14.outSaldosPmg->EstadoProc)
		{
			if (_param_14.outSaldosPmg->EstadoProc->DescripcionEstado)
				delete[] _param_14.outSaldosPmg->EstadoProc->DescripcionEstado;
			delete _param_14.outSaldosPmg->EstadoProc;
		}
		if (_param_14.outSaldosPmg->SaldosPmg)
			delete _param_14.outSaldosPmg->SaldosPmg;
		delete _param_14.outSaldosPmg;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__consultarAportaciones(struct soap *, ns2__ParamAportacionesPosteriores *inParam, struct ns2__consultarAportacionesResponse &_param_15)
{
	CAportaciones objAportaciones;
	APORTACION_POSTERIORES *stAportaciones = NULL;
	short shRet = DEFAULT__;
	_param_15.outParam = new ns2__RespuestaAportacionesPosteriores();

	_param_15.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_15.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION]; // ns2__SaldoPorSubcuentaPmg ns2__SaldosCuentasPmg
	memset(_param_15.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	_param_15.outParam->RespAportacionesPosteriores = new ns2__AportacionesPosteriores();

	shRet = objAportaciones.ObtenerAportaciones(inParam->Nss, inParam->FechaIniPension, inParam->iOpcion);
	if (shRet == OK__)
	{
		snprintf(_param_15.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stAportaciones = objAportaciones.Aportaciones();

		// Asignamos el valor retornado de la consulta a la estructura y posteriormente retornarlo
		_param_15.outParam->RespAportacionesPosteriores->Aportacion = stAportaciones->iAportacion;
	}
	else
	{
		snprintf(_param_15.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}
	_param_15.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_15.outParam)
	{
		if (_param_15.outParam->EstadoProc)
		{
			if (_param_15.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_15.outParam->EstadoProc->DescripcionEstado;
			delete _param_15.outParam->EstadoProc;
		}
		if (_param_15.outParam->RespAportacionesPosteriores)
			delete _param_15.outParam->RespAportacionesPosteriores;
		delete _param_15.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultarResolucionTotalImssRI(struct soap *, ns2__ParamResolucionTotalImssRI *inParam, struct ns2__ConsultarResolucionTotalImssRIResponse &_param_16)
{
	// pega tu codigo y cambia los nombres de las variables
	short shRet = DEFAULT__;
	CConsultarResolucionTotalImss objResolTotal; // objComplResol;
	CONSULTA_RESOLUCION_TOTAL_IMSS *stResolTotal = NULL;

	// Se crea una instancia del objeto en el cual se recibiran los datos que se retornará.
	// Nota: Es similar a un arreglo y sus elementos estan definidos en el wsdl del WS.
	_param_16.outParam = new ns2__RespuestaResolucionTotalImss();

	// Se crea una instancia del objeto que recibirá los datos de EstadoProceso y se inicializan sus elementos.
	// Nota: Solo se asigna memoria a los que son CHAR.
	_param_16.outParam->EstadoProc = new ns2__EstadoProceso();
	// Asignar memoria a las variables del objeto ns2__EstadoProceso.
	_param_16.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	// Se crea una instancia del objeto que recibirá los datos de ResolucionTotalImss y se asigna memoria a sus elementos.
	// Nota: Solo se asigna memoria a los que son CHAR.
	_param_16.outParam->ResolTotalImss = new ns2__ResolucionTotalImss();
	// Asignar memoria a las variables del objeto ns2__ResolucionTotalImss.
	_param_16.outParam->ResolTotalImss->Regimen = new char[sizeof(stResolTotal->cRegimen)];
	_param_16.outParam->ResolTotalImss->TipoSeguro = new char[sizeof(stResolTotal->cTipoSeguro)];
	_param_16.outParam->ResolTotalImss->TipoPension = new char[sizeof(stResolTotal->cTipoPension)];
	_param_16.outParam->ResolTotalImss->NombreDatamart = new char[sizeof(stResolTotal->cNombreDatamart)];
	_param_16.outParam->ResolTotalImss->SecPension = new char[sizeof(stResolTotal->cSecPension)];
	_param_16.outParam->ResolTotalImss->FechaInicio = new char[SIZE_FECHA];
	_param_16.outParam->ResolTotalImss->FechaPago = new char[SIZE_FECHA];
	_param_16.outParam->ResolTotalImss->FechaResolucion = new char[SIZE_FECHA];
	_param_16.outParam->ResolTotalImss->DescRegimen = new char[sizeof(stResolTotal->cDescripRegimen)];
	_param_16.outParam->ResolTotalImss->DescTipoPension = new char[sizeof(stResolTotal->cDescripTPension)];
	_param_16.outParam->ResolTotalImss->DescTipoPrestacion = new char[sizeof(stResolTotal->cDescripTPresta)];
	_param_16.outParam->ResolTotalImss->DescTipoRetiro = new char[sizeof(stResolTotal->cDescripTRetiro)];
	_param_16.outParam->ResolTotalImss->DescTipoSeguro = new char[sizeof(stResolTotal->cDescripTSeguro)];
	_param_16.outParam->ResolTotalImss->DescEdoSubViv = new char[sizeof(stResolTotal->cDescripEdoSubViv)];

	// Se manda llamar el metodo que ejecuta la consulta y si lo hace correctamente
	shRet = objResolTotal.ConsultarResolucionTotalImssNuevos(inParam->Opcion, inParam->Nss, inParam->Curp, inParam->Documento);
	if (shRet == OK__)
	{
		snprintf(_param_16.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stResolTotal = objResolTotal.ResolucionTotalImss();

		// Inicializar las variables del objeto.
		memset(_param_16.outParam->ResolTotalImss->Regimen, 0, sizeof(stResolTotal->cRegimen));
		memset(_param_16.outParam->ResolTotalImss->TipoSeguro, 0, sizeof(stResolTotal->cTipoSeguro));
		memset(_param_16.outParam->ResolTotalImss->TipoPension, 0, sizeof(stResolTotal->cTipoPension));
		memset(_param_16.outParam->ResolTotalImss->NombreDatamart, 0, sizeof(stResolTotal->cNombreDatamart));
		memset(_param_16.outParam->ResolTotalImss->SecPension, 0, sizeof(stResolTotal->cSecPension));
		memset(_param_16.outParam->ResolTotalImss->FechaInicio, 0, SIZE_FECHA);
		memset(_param_16.outParam->ResolTotalImss->FechaPago, 0, SIZE_FECHA);
		memset(_param_16.outParam->ResolTotalImss->FechaResolucion, 0, SIZE_FECHA);
		memset(_param_16.outParam->ResolTotalImss->DescTipoPension, 0, sizeof(stResolTotal->cDescripTPension));
		memset(_param_16.outParam->ResolTotalImss->DescRegimen, 0, sizeof(stResolTotal->cDescripRegimen));
		memset(_param_16.outParam->ResolTotalImss->DescTipoPrestacion, 0, sizeof(stResolTotal->cDescripTPresta));
		memset(_param_16.outParam->ResolTotalImss->DescTipoRetiro, 0, sizeof(stResolTotal->cDescripTRetiro));
		memset(_param_16.outParam->ResolTotalImss->DescTipoSeguro, 0, sizeof(stResolTotal->cDescripTSeguro));
		memset(_param_16.outParam->ResolTotalImss->DescEdoSubViv, 0, sizeof(stResolTotal->cDescripEdoSubViv));

		// Asignar valores a las variables de la estructura obtenidas a traves de la consulta al objeto
		_param_16.outParam->ResolTotalImss->ExisteResolucion = stResolTotal->iExisteResolucion;
		_param_16.outParam->ResolTotalImss->ExisteAfiliado = stResolTotal->iExisteAfiliado;
		snprintf(_param_16.outParam->ResolTotalImss->Regimen, sizeof(_param_16.outParam->ResolTotalImss->Regimen), "%s", stResolTotal->cRegimen);
		snprintf(_param_16.outParam->ResolTotalImss->TipoSeguro, sizeof(_param_16.outParam->ResolTotalImss->TipoSeguro), "%s", stResolTotal->cTipoSeguro);
		snprintf(_param_16.outParam->ResolTotalImss->TipoPension, sizeof(_param_16.outParam->ResolTotalImss->TipoPension), "%s", stResolTotal->cTipoPension);
		snprintf(_param_16.outParam->ResolTotalImss->NombreDatamart, sizeof(_param_16.outParam->ResolTotalImss->NombreDatamart), "%s", stResolTotal->cNombreDatamart);
		_param_16.outParam->ResolTotalImss->TipoPrestacion = stResolTotal->iTipoPrestacion;
		_param_16.outParam->ResolTotalImss->Folio = stResolTotal->iFolio;
		snprintf(_param_16.outParam->ResolTotalImss->SecPension, sizeof(_param_16.outParam->ResolTotalImss->SecPension), "%s", stResolTotal->cSecPension);
		snprintf(_param_16.outParam->ResolTotalImss->FechaInicio, sizeof(_param_16.outParam->ResolTotalImss->FechaInicio), "%s", stResolTotal->dFechaIni);
		snprintf(_param_16.outParam->ResolTotalImss->FechaPago, sizeof(_param_16.outParam->ResolTotalImss->FechaPago), "%s", stResolTotal->dFechaPago);
		_param_16.outParam->ResolTotalImss->SemanasCotizadas = stResolTotal->iSemanasCotizadas;
		_param_16.outParam->ResolTotalImss->DiagDatamart = stResolTotal->iDiagDatamart;
		_param_16.outParam->ResolTotalImss->EstadoSubViv = stResolTotal->iEstadoSubViv;
		snprintf(_param_16.outParam->ResolTotalImss->FechaResolucion, sizeof(_param_16.outParam->ResolTotalImss->FechaResolucion), "%s", stResolTotal->dFechaResolucion);
		snprintf(_param_16.outParam->ResolTotalImss->DescRegimen, sizeof(_param_16.outParam->ResolTotalImss->DescRegimen), "%s", stResolTotal->cDescripRegimen);
		snprintf(_param_16.outParam->ResolTotalImss->DescTipoPension, sizeof(_param_16.outParam->ResolTotalImss->DescTipoPension), "%s", stResolTotal->cDescripTPension);
		snprintf(_param_16.outParam->ResolTotalImss->DescTipoPrestacion, sizeof(_param_16.outParam->ResolTotalImss->DescTipoPrestacion), "%s", stResolTotal->cDescripTPresta);
		snprintf(_param_16.outParam->ResolTotalImss->DescTipoRetiro, sizeof(_param_16.outParam->ResolTotalImss->DescTipoRetiro), "%s", stResolTotal->cDescripTRetiro);
		snprintf(_param_16.outParam->ResolTotalImss->DescTipoSeguro, sizeof(_param_16.outParam->ResolTotalImss->DescTipoSeguro), "%s", stResolTotal->cDescripTSeguro);
		snprintf(_param_16.outParam->ResolTotalImss->DescEdoSubViv, sizeof(_param_16.outParam->ResolTotalImss->DescEdoSubViv), "%s", stResolTotal->cDescripEdoSubViv);
		_param_16.outParam->ResolTotalImss->IdMovimiento = stResolTotal->iIdMovimiento;
	}
	else
		snprintf(_param_16.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_16.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_16.outParam)
	{
		if (_param_16.outParam->EstadoProc)
		{
			if (_param_16.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_16.outParam->EstadoProc->DescripcionEstado;
			delete _param_16.outParam->EstadoProc;
		}
		if (_param_16.outParam->ResolTotalImss)
		{
			if (_param_16.outParam->ResolTotalImss->Regimen)
				delete[] _param_16.outParam->ResolTotalImss->Regimen;
			if (_param_16.outParam->ResolTotalImss->TipoSeguro)
				delete[] _param_16.outParam->ResolTotalImss->TipoSeguro;
			if (_param_16.outParam->ResolTotalImss->TipoPension)
				delete[] _param_16.outParam->ResolTotalImss->TipoPension;
			if (_param_16.outParam->ResolTotalImss->NombreDatamart)
				delete[] _param_16.outParam->ResolTotalImss->NombreDatamart;
			if (_param_16.outParam->ResolTotalImss->SecPension)
				delete[] _param_16.outParam->ResolTotalImss->SecPension;
			if (_param_16.outParam->ResolTotalImss->FechaInicio)
				delete[] _param_16.outParam->ResolTotalImss->FechaInicio;
			if (_param_16.outParam->ResolTotalImss->FechaPago)
				delete[] _param_16.outParam->ResolTotalImss->FechaPago;
			if (_param_16.outParam->ResolTotalImss->FechaResolucion)
				delete[] _param_16.outParam->ResolTotalImss->FechaResolucion;
			if (_param_16.outParam->ResolTotalImss->DescRegimen)
				delete[] _param_16.outParam->ResolTotalImss->DescRegimen;
			if (_param_16.outParam->ResolTotalImss->DescTipoPension)
				delete[] _param_16.outParam->ResolTotalImss->DescTipoPension;
			if (_param_16.outParam->ResolTotalImss->DescTipoPrestacion)
				delete[] _param_16.outParam->ResolTotalImss->DescTipoPrestacion;
			if (_param_16.outParam->ResolTotalImss->DescTipoRetiro)
				delete[] _param_16.outParam->ResolTotalImss->DescTipoRetiro;
			if (_param_16.outParam->ResolTotalImss->DescTipoSeguro)
				delete[] _param_16.outParam->ResolTotalImss->DescTipoSeguro;
			if (_param_16.outParam->ResolTotalImss->DescEdoSubViv)
				delete[] _param_16.outParam->ResolTotalImss->DescEdoSubViv;
			delete _param_16.outParam->ResolTotalImss;
		}
		delete _param_16.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultarSieforeCtaRegimen(struct soap *, ns2__ParamNssCtaRegimen *inParam, struct ns2__ConsultarSieforeCtaRegimenResponse &_param_17)
{
	short shRet = DEFAULT__, shTotalReg = 0, i = 0, shCont = 0, shNumRegistros = 0;

	CConsultarCtaRegimen objsieforeCtaReg;
	// SIEFORE	*shmSiefore = NULL;
	SIEFORE *stSiefore = NULL;

	_param_17.outParam = new ns2__RespuestaSieforeCtaRegimen();
	_param_17.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_17.outParam->ListaSiefore = new ArrayOfSieforeCtaRegimen();
	_param_17.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	memset(_param_17.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	// Asignar memoria a las variables del objeto ns2__EstadoProceso.
	_param_17.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	shRet = objsieforeCtaReg.consultarSieforeCtaRegimen(inParam->Nss);
	if (shRet == OK__)
	{
		snprintf(_param_17.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");

		stSiefore = objsieforeCtaReg.RespSiefore();

		shNumRegistros = objsieforeCtaReg.registrosctaregimen();

		for (i = 0; i < NUM_CTA_REGIMEN; i++)
		{
			if (stSiefore->arrCtaRegimen[i].cSubCuenta > 0)
				shTotalReg++;
		}

		_param_17.outParam->ListaSiefore->__size = shTotalReg;
		_param_17.outParam->ListaSiefore->__ptrSiefore = new ns2__SieforeCtaRegimen *[shTotalReg];

		for (i = 0; i < shNumRegistros; i++)
		{
			if (i == 0)
			{
				_param_17.outParam->ListaSiefore->__size = shNumRegistros;
				_param_17.outParam->ListaSiefore->__ptrSiefore = new ns2__SieforeCtaRegimen *[shNumRegistros];
			}

			_param_17.outParam->ListaSiefore->__ptrSiefore[shCont] = new ns2__SieforeCtaRegimen();
			_param_17.outParam->ListaSiefore->__ptrSiefore[shCont]->Nss = new char[12];
			memset(_param_17.outParam->ListaSiefore->__ptrSiefore[shCont]->Nss, 0, 12);

			snprintf(_param_17.outParam->ListaSiefore->__ptrSiefore[shCont]->Nss, sizeof(_param_17.outParam->ListaSiefore->__ptrSiefore[shCont]->Nss), "%s", stSiefore->arrCtaRegimen[i].cNss);
			_param_17.outParam->ListaSiefore->__ptrSiefore[shCont]->subCuenta = stSiefore->arrCtaRegimen[i].cSubCuenta; // arrCtaRegimen[i].cSubCuenta;
			_param_17.outParam->ListaSiefore->__ptrSiefore[shCont]->Siefore = stSiefore->arrCtaRegimen[i].cSiefore;

			shCont++;
		}
	}
	else
		snprintf(_param_17.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");

	_param_17.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_17.outParam)
	{
		if (_param_17.outParam->EstadoProc)
		{
			if (_param_17.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_17.outParam->EstadoProc->DescripcionEstado;
			delete _param_17.outParam->EstadoProc;
		}
		if (_param_17.outParam->ListaSiefore)
		{
			if (_param_17.outParam->ListaSiefore->__ptrSiefore)
			{
				for (int i = 0; i < _param_17.outParam->ListaSiefore->__size; i++)
				{
					if (_param_17.outParam->ListaSiefore->__ptrSiefore[i])
					{
						if (_param_17.outParam->ListaSiefore->__ptrSiefore[i]->Nss)
							delete[] _param_17.outParam->ListaSiefore->__ptrSiefore[i]->Nss;
						delete _param_17.outParam->ListaSiefore->__ptrSiefore[i];
					}
				}
				delete[] _param_17.outParam->ListaSiefore->__ptrSiefore;
			}
			delete _param_17.outParam->ListaSiefore;
		}
		delete _param_17.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultarSaldoVol(struct soap *, ns2__ParametroNss *inParam, struct ns2__ConsultarSaldoVolResponse &_param_18)
{
	short shRet = DEFAULT__, shTotalReg = 0, i = 0, shCont = 0, shNumRegistros = 0;

	CConsultarCtaSaldoVol objCtaSaldoVol;
	SALDO_VOL *stSalVol = NULL;

	_param_18.outParam = new ns2__RespuestaCtaSaldoVol();
	_param_18.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_18.outParam->ListaSaldoVol = new ArrayOfDatosCtaSaldoVol();
	_param_18.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	memset(_param_18.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	// Asignar memoria a las variables del objeto ns2__EstadoProceso.
	//_param_18.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	shRet = objCtaSaldoVol.consultarFechaAporte(inParam->Nss);

	if (shRet == OK__)
	{
		snprintf(cTexto, sizeof(cTexto), "EXITO");
		CUtileriasAfo::grabarLogx(RUTA_LOGX, cTexto);
		snprintf(_param_18.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");

		stSalVol = objCtaSaldoVol.RespCtaSaldoVol();
		shNumRegistros = objCtaSaldoVol.registrosCtaSaldoVol();

		for (i = 0; i < NUM_CTA_SALDO_VOL; i++)
		{
			if (stSalVol->arrCtaSaldoVol[i].iFolio > 0)
				shTotalReg++;
		}

		_param_18.outParam->ListaSaldoVol->__size = shNumRegistros;
		_param_18.outParam->ListaSaldoVol->__ptrSaldovol = new ns2__DatosCtaSaldoVol *[shNumRegistros];

		for (i = 0; i < shNumRegistros; i++)
		{
			/*if(i==0)
			{
				_param_18.outParam->ListaSaldoVol->__size=shNumRegistros;
				_param_18.outParam->ListaSaldoVol->__ptrSaldovol = new ns2__DatosCtaSaldoVol*[shNumRegistros];
			}*/
			if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont] != nullptr)
			{
				delete _param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont];
			}
			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont] = new ns2__DatosCtaSaldoVol();

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iFolio = stSalVol->arrCtaSaldoVol[i].iFolio;
			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iConsecutivoLote = stSalVol->arrCtaSaldoVol[i].iConsecutivoLote;

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cNss = new char[SIZE_NSS + 2];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cNss, 0, SIZE_NSS + 2);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cNss, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cNss), "%s", stSalVol->arrCtaSaldoVol[i].cNss);

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iSiefore = stSalVol->arrCtaSaldoVol[i].iSiefore;
			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iSubCuenta = stSalVol->arrCtaSaldoVol[i].iSubCuenta;

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaValor = new char[12];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaValor, 0, 12);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaValor, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaValor), "%s", stSalVol->arrCtaSaldoVol[i].cFechaValor);

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaConversion = new char[12];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaConversion, 0, 12);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaConversion, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaConversion), "%s", stSalVol->arrCtaSaldoVol[i].cFechaConversion);

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnPesos = new char[SIZE_DECIMAL_];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnPesos, 0, SIZE_DECIMAL_);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnPesos, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnPesos), "%.02f", stSalVol->arrCtaSaldoVol[i].iMontoEnPesos);

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnAcciones = new char[SIZE_DECIMAL_];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnAcciones, 0, SIZE_DECIMAL_);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnAcciones, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iMontoEnAcciones), "%.06f", stSalVol->arrCtaSaldoVol[i].iMontoEnAcciones);

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iSaldoAcciones = new char[SIZE_DECIMAL_];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iSaldoAcciones, 0, SIZE_DECIMAL_);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iSaldoAcciones, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->iSaldoAcciones), "%.06f", stSalVol->arrCtaSaldoVol[i].iSaldoAcciones);

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaSaldo = new char[12];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaSaldo, 0, 12);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaSaldo, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cFechaSaldo), "%s", stSalVol->arrCtaSaldoVol[i].cFechaSaldo);

			_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cUsuario = new char[14];
			memset(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cUsuario, 0, 14);
			snprintf(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cUsuario, sizeof(_param_18.outParam->ListaSaldoVol->__ptrSaldovol[shCont]->cUsuario), "%s", stSalVol->arrCtaSaldoVol[i].cUsuario);

			shCont++;
		}
	}
	else
	{
		snprintf(_param_18.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}

	_param_18.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_18.outParam)
	{
		if (_param_18.outParam->EstadoProc)
		{
			if (_param_18.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_18.outParam->EstadoProc->DescripcionEstado;
			delete _param_18.outParam->EstadoProc;
		}
		if (_param_18.outParam->ListaSaldoVol)
		{
			if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol)
			{
				for (int i = 0; i < _param_18.outParam->ListaSaldoVol->__size; i++)
				{
					if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i])
					{
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cNss)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cNss;
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cFechaValor)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cFechaValor;
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cFechaConversion)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cFechaConversion;
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->iMontoEnPesos)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->iMontoEnPesos;
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->iMontoEnAcciones)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->iMontoEnAcciones;
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->iSaldoAcciones)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->iSaldoAcciones;
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cFechaSaldo)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cFechaSaldo;
						if (_param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cUsuario)
							delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i]->cUsuario;
						delete _param_18.outParam->ListaSaldoVol->__ptrSaldovol[i];
					}
				}
				delete[] _param_18.outParam->ListaSaldoVol->__ptrSaldovol;
			}
			delete _param_18.outParam->ListaSaldoVol;
		}
		delete _param_18.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultaSaldoVivienda(struct soap *, ns2__ParamConsultaSaldoVivienda *inParam, struct ns2__ConsultaSaldoViviendaResponse &_param_19)
{
	short shRet = DEFAULT__;

	CConsultaSaldoVivienda objSaldoViv;
	CONSULTA_SALDOS_VIVIENDA *stSalViv = NULL;

	_param_19.outParam = new ns2__RespuestaConsultaSaldoVivienda();
	_param_19.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_19.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	_param_19.outParam->ConsultarSaldoViv = new ns2__ConsultarSaldoVivienda();

	// Asignar memoria a las variables del objeto ns2__EstadoProceso.
	memset(_param_19.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	shRet = objSaldoViv.consultavivienda(inParam->sNss, inParam->sFechaSolicitud, inParam->sFechaIncioPension);

	if (shRet == OK__)
	{
		snprintf(_param_19.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");

		stSalViv = objSaldoViv.getConsultaSaldosVivienda();

		_param_19.outParam->ConsultarSaldoViv->sNss = new char[SIZE_NSS];
		memset(_param_19.outParam->ConsultarSaldoViv->sNss, 0, SIZE_NSS);
		snprintf(_param_19.outParam->ConsultarSaldoViv->sNss, sizeof(_param_19.outParam->ConsultarSaldoViv->sNss), "%s", stSalViv->sNss);

		_param_19.outParam->ConsultarSaldoViv->dMontoAccionViv = new char[sizeof(stSalViv->dMontoaccviv)];
		memset(_param_19.outParam->ConsultarSaldoViv->dMontoAccionViv, 0, sizeof(stSalViv->dMontoaccviv));
		snprintf(_param_19.outParam->ConsultarSaldoViv->dMontoAccionViv, sizeof(_param_19.outParam->ConsultarSaldoViv->dMontoAccionViv), "%s", stSalViv->dMontoaccviv);

		_param_19.outParam->ConsultarSaldoViv->dMontoPesosViv = new char[sizeof(stSalViv->dMontopesviv)];
		memset(_param_19.outParam->ConsultarSaldoViv->dMontoPesosViv, 0, sizeof(stSalViv->dMontopesviv));
		snprintf(_param_19.outParam->ConsultarSaldoViv->dMontoPesosViv, sizeof(_param_19.outParam->ConsultarSaldoViv->dMontoPesosViv), "%s", stSalViv->dMontopesviv);
	}
	else
	{
		snprintf(_param_19.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}

	_param_19.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_19.outParam)
	{
		if (_param_19.outParam->EstadoProc)
		{
			if (_param_19.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_19.outParam->EstadoProc->DescripcionEstado;
			delete _param_19.outParam->EstadoProc;
		}
		if (_param_19.outParam->ConsultarSaldoViv)
		{
			if (_param_19.outParam->ConsultarSaldoViv->sNss)
				delete[] _param_19.outParam->ConsultarSaldoViv->sNss;
			if (_param_19.outParam->ConsultarSaldoViv->dMontoAccionViv)
				delete[] _param_19.outParam->ConsultarSaldoViv->dMontoAccionViv;
			if (_param_19.outParam->ConsultarSaldoViv->dMontoPesosViv)
				delete[] _param_19.outParam->ConsultarSaldoViv->dMontoPesosViv;
			delete _param_19.outParam->ConsultarSaldoViv;
		}
		delete _param_19.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__ConsultaAportacionesPosteriores(struct soap *, ns2__ParametroAportacionesPost *inParam, struct ns2__ConsultaAportacionesPosterioresResponse &_param_20)
{
	short shRet = DEFAULT__;

	CConsultaAportacionesPosteriores objAportacionesPosteriores;
	CONSULTA_APORTACIONES_POSTERIORES *stAportacionesPosteriores = NULL;

	_param_20.outParam = new ns2__RespuestaAportacionesPosteriores();
	_param_20.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_20.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	memset(_param_20.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	_param_20.outParam->RespAportacionesPosteriores = new ns2__AportacionesPosteriores();

	shRet = objAportacionesPosteriores.CConsultarAportacionesPosterioresBD(inParam->iOpcion, inParam->Nss, inParam->FechaIniPension, inParam->iGrupo);
	if (shRet == OK__)
	{
		snprintf(_param_20.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		stAportacionesPosteriores = objAportacionesPosteriores.AportacionesPosteriores();

		// Asignamos el valor retornado de la consulta a la estructura y posteriormente retornarlo
		_param_20.outParam->RespAportacionesPosteriores->Aportacion = stAportacionesPosteriores->iAportacion;
	}
	else
	{
		snprintf(_param_20.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}

	_param_20.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_20.outParam)
	{
		if (_param_20.outParam->EstadoProc)
		{
			if (_param_20.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_20.outParam->EstadoProc->DescripcionEstado;
			delete _param_20.outParam->EstadoProc;
		}
		if (_param_20.outParam->RespAportacionesPosteriores)
			delete _param_20.outParam->RespAportacionesPosteriores;
		delete _param_20.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__CConsultarResolusionRetAportaciones(struct soap *, ns2__ParametroCurpNss *inParam, struct ns2__CConsultarResolusionRetAportacionesResponse &_param_1)
{
	short shRet = DEFAULT__;
	CConsultarResolusionRetAportaciones objAportaciones;

	_param_1.outParam = new ns2__RespuestaTipoSolicitante();
	_param_1.outParam->Solicitante = new ns2__TipoSolicitante();

	shRet = objAportaciones.ConsultarResolusion(inParam->Curp, inParam->Nss);

	_param_1.outParam->Solicitante->iTipoSolicitante = objAportaciones.Resolucion();

	// Liberación de memoria dinámica
	if (_param_1.outParam)
	{
		if (_param_1.outParam->Solicitante)
			delete _param_1.outParam->Solicitante;
		delete _param_1.outParam;
	}

	return SOAP_OK;
}

// 418
SOAP_FMAC5 int SOAP_FMAC6 ns2__CConsultarSaldoRecuperar(struct soap *, ns2__ParametroNss *inParam, struct ns2__CConsultarSaldoRecuperarResponse &_param_21)
{
	short shRet = DEFAULT__;
	COperacionesPmg objRespuestaSaldos;

	_param_21.outParam = new ns2__RespuestaSaldoRecuperar();
	_param_21.outParam->Saldo = new ns2__SaldoRecuperar();
	_param_21.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_21.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	memset(_param_21.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	shRet = objRespuestaSaldos.buscarSaldoaRecuperar(inParam->Nss);
	if (shRet == OK__)
	{
		snprintf(_param_21.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		_param_21.outParam->Saldo->iRespuestaSaldo = objRespuestaSaldos.RespuestaSaldo();
	}

	_param_21.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_21.outParam)
	{
		if (_param_21.outParam->EstadoProc)
		{
			if (_param_21.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_21.outParam->EstadoProc->DescripcionEstado;
			delete _param_21.outParam->EstadoProc;
		}
		if (_param_21.outParam->Saldo)
			delete _param_21.outParam->Saldo;
		delete _param_21.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__CConsultarFechaLiquida(struct soap *, ns2__ParametroNss *inParam, struct ns2__CConsultarFechaLiquidaResponse &_param_22)
{
	short shRet = DEFAULT__;
	CSaldosPorSubCuentaPmg objFechaLiquida;

	_param_22.outParam = new ns2__RespuestaFechaLiquida();
	_param_22.outParam->Fecha = new ns2__FechaLiquida();
	_param_22.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_22.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	memset(_param_22.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	shRet = objFechaLiquida.obtenerFechaLiquidaPmg(inParam->Nss);
	if (shRet == OK__)
	{
		snprintf(_param_22.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		_param_22.outParam->Fecha->cFechaLiquida = objFechaLiquida.RespuestaFecha();
	}

	_param_22.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_22.outParam)
	{
		if (_param_22.outParam->EstadoProc)
		{
			if (_param_22.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_22.outParam->EstadoProc->DescripcionEstado;
			delete _param_22.outParam->EstadoProc;
		}
		if (_param_22.outParam->Fecha)
			delete _param_22.outParam->Fecha;
		delete _param_22.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__CConsultarNumMensualidad(struct soap *, ns2__ParametroNss *inParam, struct ns2__CConsultarNumMensualidadResponse &_param_23)
{
	short shRet = DEFAULT__;
	COperacionesPmg objNumMensualidad;

	_param_23.outParam = new ns2__RespuestaMensualidad();
	_param_23.outParam->Mensualidad = new ns2__NumMensualidad();
	_param_23.outParam->EstadoProc = new ns2__EstadoProceso();
	_param_23.outParam->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];
	memset(_param_23.outParam->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);

	shRet = objNumMensualidad.obtenerNumMensualidad(inParam->Nss);
	if (shRet == OK__)
	{
		snprintf(_param_23.outParam->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		_param_23.outParam->Mensualidad->iRespuestaMensualidad = objNumMensualidad.RespuestaMensualidad();
	}

	_param_23.outParam->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_23.outParam)
	{
		if (_param_23.outParam->EstadoProc)
		{
			if (_param_23.outParam->EstadoProc->DescripcionEstado)
				delete[] _param_23.outParam->EstadoProc->DescripcionEstado;
			delete _param_23.outParam->EstadoProc;
		}
		if (_param_23.outParam->Mensualidad)
			delete _param_23.outParam->Mensualidad;
		delete _param_23.outParam;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 ns2__consultarSaldosPenMinGar(struct soap *, ns2__ParametroNss *inParam, struct ns2__consultarSaldosPenMinGarResponse &_param_24)
{
	CSaldosPorSubCuenta objSaldo;
	SALDO_CUENTA *stSaldoCta = NULL;
	short shRet = DEFAULT__, shTotalReg = 0, i = 0, shCont = 0;
	_param_24.outSaldos = new ns2__SaldoPorSubcuenta();
	_param_24.outSaldos->EstadoProc = new ns2__EstadoProceso();
	_param_24.outSaldos->ListaSaldos = new ArrayOfSaldoSubCuenta();
	_param_24.outSaldos->Nss = new char[SIZE_NSS + 2];

	_param_24.outSaldos->EstadoProc->DescripcionEstado = new char[LONG_DESCRIPCION];

	memset(_param_24.outSaldos->Nss, 0, SIZE_NSS + 2);
	memset(_param_24.outSaldos->EstadoProc->DescripcionEstado, 0, LONG_DESCRIPCION);
	shRet = objSaldo.consultarSaldoPorSubcuentaPMG(inParam->Nss);
	if (shRet == OK__)
	{
		stSaldoCta = objSaldo.SaldoPorSubcuenta();
		for (i = 0; i < NUM_SUBCUENTAS; i++)
		{
			if (stSaldoCta->arrSubCta[i].shSubCuenta > 0)
				shTotalReg++;
		}
		snprintf(_param_24.outSaldos->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "EXITO");
		memmove(_param_24.outSaldos->Nss, inParam->Nss, sizeof(SIZE_NSS));
		_param_24.outSaldos->ListaSaldos->__size = shTotalReg;
		_param_24.outSaldos->ListaSaldos->__ptrSaldo = new ns2__SaldoSubCuenta *[shTotalReg];
		for (i = 0; i < NUM_SUBCUENTAS; i++)
		{
			if (stSaldoCta->arrSubCta[i].shSubCuenta > 0)
			{
				_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont] = new ns2__SaldoSubCuenta();
				_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion = new char[SIZE_DECIMAL_];
				_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos = new char[SIZE_DECIMAL_];
				memset(_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion, 0, SIZE_DECIMAL_);
				memset(_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos, 0, SIZE_DECIMAL_);
				_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SubCuenta = stSaldoCta->arrSubCta[i].shSubCuenta;
				_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->Siefore = stSaldoCta->arrSubCta[i].shSiefore;
				snprintf(_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion, sizeof(_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoAccion), "%.06f", stSaldoCta->arrSubCta[i].dMontoAccion);
				snprintf(_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos, sizeof(_param_24.outSaldos->ListaSaldos->__ptrSaldo[shCont]->SaldoPesos), "%.02f", stSaldoCta->arrSubCta[i].dMontoPesos);
				shCont++;
			}
		}
	}
	else
	{
		snprintf(_param_24.outSaldos->EstadoProc->DescripcionEstado, sizeof(LONG_DESCRIPCION), "PROCESO EJECUTADO CON ERROR");
	}

	_param_24.outSaldos->EstadoProc->Estado = shRet;

	// Liberación de memoria dinámica
	if (_param_24.outSaldos)
	{
		if (_param_24.outSaldos->EstadoProc)
		{
			if (_param_24.outSaldos->EstadoProc->DescripcionEstado)
				delete[] _param_24.outSaldos->EstadoProc->DescripcionEstado;
			delete _param_24.outSaldos->EstadoProc;
		}
		if (_param_24.outSaldos->Nss)
			delete[] _param_24.outSaldos->Nss;

		if (_param_24.outSaldos->ListaSaldos)
		{
			if (_param_24.outSaldos->ListaSaldos->__ptrSaldo)
			{
				for (int i = 0; i < _param_24.outSaldos->ListaSaldos->__size; ++i)
				{
					if (_param_24.outSaldos->ListaSaldos->__ptrSaldo[i])
					{
						if (_param_24.outSaldos->ListaSaldos->__ptrSaldo[i]->SaldoAccion)
							delete[] _param_24.outSaldos->ListaSaldos->__ptrSaldo[i]->SaldoAccion;
						if (_param_24.outSaldos->ListaSaldos->__ptrSaldo[i]->SaldoPesos)
							delete[] _param_24.outSaldos->ListaSaldos->__ptrSaldo[i]->SaldoPesos;
						delete _param_24.outSaldos->ListaSaldos->__ptrSaldo[i];
					}
				}
				delete[] _param_24.outSaldos->ListaSaldos->__ptrSaldo;
			}
			delete _param_24.outSaldos->ListaSaldos;
		}
		delete _param_24.outSaldos;
	}

	return SOAP_OK;
}