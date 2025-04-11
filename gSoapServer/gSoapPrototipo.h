//gsoap ns1 service name: wsModuloAfore
//gsoap ns1 service style: rpc
//gsoap ns1 service encoding: encoded
//gsoap ns1 service location: http://0.0.0.0:0/
//gsoap ns1 service documentation: Servicio consultas de modulo afore
//gsoap ns1 schema namespace: urn:wsModuloAfore

//#include "tipoDatos.h"
//enum ns1__TipoServicio {0='0',A='A',C='C',N='N',P='P',R='R',S='S'};
//enum week { sunday, monday, tuesday, wednesday, thursday, friday, saturday };
enum ns1__ClasificacionServicio { O,A,C,N,P,Q,S }; //O equivale al Cero//se cambia la R por Q
enum ns1__TipoTrabajador {Imss=1,Issste};
enum ns1__TipoRetiroParcial{Desempleo=1024,Matrimonio=1025};
class EstadoProceso
{
	public:
		short	Estado;
		char*	DescripcionEstado;
		struct soap 	*soap;
};

class ParamConsultarAfiliado
{
	public:
		int iFolioServicio;
};

class ParametroNss
{
	public:
		char* Nss;
		struct soap *soap;
};

class ParametroSaldoPmg
{
	public:
		char* Nss;
		char* Secuencia;
		int	  EstadoViv;
		struct soap *soap;
};

class ParametroNssTipoRetiro
{
	public:
		char* Nss;
		enum ns1__TipoRetiroParcial TipoRetiro;
		struct soap *soap;
};

class Afiliado
{
	public:
		char * cApellidoPaterno,
			 * cApellidoMaterno,
			 * cNombre,
			 * cCurp,
			 * cNss,
			 * cFechaServicio,
			 * cFechaNacimiento,
			 * cEntidadNacimiento;
		int iSexo;
		struct soap *soap;
};

class RespConsultarAfiliado
{
	public:
		Afiliado datosAfiliado;
		EstadoProceso EstadoProc;
		struct soap *soap;
};

class SaldoSubCuenta
{
	public:
		int 	SubCuenta;
		int 	Siefore;
		char*	SaldoAccion; //-- se maneja en char para el manejo de decimales en el ws
		char*	SaldoPesos;  //-- se maneja en char para el manejo de decimales en el ws
		struct soap *soap;
};
class ListaSaldoSubCuenta
{
	public:
		SaldoSubCuenta *__ptrSaldo;
		int		__size;
		struct soap *soap;
};
class SaldoPorSubcuenta
{
	public:
		EstadoProceso EstadoProc;
		char*	Nss;
		ListaSaldoSubCuenta *ListaSaldos;
		struct soap *soap;
};
class SaldosCuentasPmg
{
	public:
		double Retroactivo;
		double Devengado;	
		double SaldoDia;
		int    Suficiencia;
		int    Registros;
		struct soap 	*soap;
};
class SaldoPorSubcuentaPmg
{
	public:
		EstadoProceso EstadoProc;
		SaldosCuentasPmg SaldosPmg;
		struct soap *soap;
};
class FoliadorServicio
{
	public:
		enum ns1__ClasificacionServicio TipoServicio;
		struct soap *soap;
};
class FolioServicio
{
	public:
		EstadoProceso EstadoProc;
		int	Folio;
		struct soap *soap;
};
class ConvivenciaMarcas
{
	public:
		enum ns1__TipoTrabajador TipoTrabajador;
		int     Opcion;
		char*	Nss;
		char*	TipoRetiro;
		struct soap *soap;
};
class ValidaConvivenciaMarcas
{
	public:
		int   Codigo;
		char* Descripcion;
		int   TieneMarca;
		struct soap *soap;
};
class RespuestaConvivenciaMarca
{
	public:
		EstadoProceso EstadoProc;
		ValidaConvivenciaMarcas ConvivenciaMarca;
		struct soap *soap;
};

class TipoSolicitante
{
	public:
		int   iTipoSolicitante;
		struct soap *soap;
};

class RespuestaTipoSolicitante
{
	public:
		TipoSolicitante Solicitante;
		struct soap *soap;
};

class ConsultaSaldoDiarioRetiroParcial
{
	public:
		char*	Nss;
		enum ns1__TipoRetiroParcial TipoRetiro;
		struct soap 	*soap;
};
class SaldoDiarioRetiroParcial
{
	EstadoProceso EstadoProc;
	char*	SaldoPesos;  //-- se maneja en char para el manejo de decimales en el ws
	struct soap 	*soap;
};
class ValidarResolucionImss
{
	public:
		char*	FechaVigencia;
		char*	FechaVigenciaAnterior;
		char*	FechaProximaSolicitud;
		int		Retorno;
		char*	Descripcion;
		int		DiagnosticoProcesar;
		struct soap 	*soap;
};
class RespuestaValidarResolucionImss
{
	public:
		EstadoProceso EstadoProc;
		ValidarResolucionImss ResolucionImss;
		struct soap 	*soap;
};
class MontosFechasDatamart
{
	public:
		char*	Nss;
		char*	FechaMatrimonioDesempleo;
		char*	FechaInicioVigencia;
		char*	SalarioBaseA;
		char*	SalarioBaseB;
		char*	UltimoSalarioBaseCotizado;
		char*	Ultimas250SemanasSBC;
		struct soap 	*soap;
};
class RespuestaMontosFechasDatamart
{
	public:
		EstadoProceso EstadoProc;
		MontosFechasDatamart MontosFechaDat;
		struct soap 	*soap;
};
class DatosResolucion
{
	public:
		char*	Nss;
		char*	FechaInicioVigencia;
		char*	FechaFinVigencia;
		int		NumeroResolucion;
		struct soap 	*soap;
};
class RespuestaDatosResolucion
{
	public:
		EstadoProceso EstadoProc;
		DatosResolucion Resolucion;
		struct soap 	*soap;
};
class ComplementoResolucion
{
	public:
		int		IdentificadorComplemento;
		int		NumeroResolucion;
		char*	MontoPagado;
		char*	MontoPagadoComplemento;
		char*	SaldoRCV;
		char*	SaldoRCVAnterior;
		int		RespuestaOtroDatamart;
		struct soap 	*soap;
};

class RespuestaComplementoResolucion
{
	public:
		EstadoProceso EstadoProc;
		ComplementoResolucion ResolucionComplemento;
		struct soap 	*soap;
};

class ParamResolucionTotalImss
{
	public:
		int		Opcion;
		char*	Nss;
		char*	Curp;
		struct soap		*soap;
};

class ParamResolucionTotalImssRI
{
	public:
		int		Opcion;
		char*	Nss;
		char*	Curp;
		int		Documento;
		struct soap		*soap;
};

class ResolucionTotalImss
{
	public:
		int		ExisteResolucion;
		int		ExisteAfiliado;
		char*	Regimen;
		char*	TipoSeguro;
		char*	TipoPension;
		int		TipoPrestacion;
		char*	NombreDatamart;
		int		Folio;
		char*	SecPension;
		char*	FechaInicio;
		char*	FechaPago;
		int		SemanasCotizadas;
		int		DiagDatamart;
		int		EstadoSubViv;
		char*	FechaResolucion;
		char*	DescRegimen;
		char*	DescTipoPension;
		char*	DescTipoPrestacion;
		char*	DescTipoRetiro;
		char*	DescTipoSeguro;
		char*	DescEdoSubViv;
		int		IdMovimiento;
		struct soap 	*soap;
};

class RespuestaResolucionTotalImss
{
	public:
		EstadoProceso EstadoProc;
		ResolucionTotalImss ResolTotalImss;
		struct soap 	*soap;
};

class ParamOperacionPmgImss
{
	public:
		char* Nss;
		char* Secuencia;
		int Folio;
		int iOpcion;
		struct soap		*soap;
};

class ResolucionOperacionPmg
{
	public:
		char* Mensaje;
		int   Codigo;	
		double Importe;
		struct soap 	*soap;
};

class RespuestaCOperacionesPmgImss
{
	public:
		EstadoProceso EstadoProc;
		ResolucionOperacionPmg ResolOperacionPmg;
		struct soap 	*soap;
};

class ParamMovimientosCuentas
{
	public:
		char* Nss;
		struct soap *soap;
};

class MovimientosCuentas
{
	public:
		int		Movimientos;
};

class RespuestaMovimientosCuentas
{
	public:
		EstadoProceso EstadoProc;
		MovimientosCuentas RespMovimientosCuentas;
		struct soap 	*soap;
};

class ParamResolucionTotalIssste
{
	public:
		char* Curp;
		char* TipoRetiro;
		struct soap *soap;
};

class ResolucionTotalIssste
{
	public:
		char*		Nss;			
		char*		NssIssste;		
		char*		Curp;			
		char*		SecPension;		
		char*		NombreDatamart;	
		char*		PaternoDatamart;
		char*		MaternoDatamart;			
		char*		NumConcesion;	
		char*		TipoRetiro;		
		char*		Regimen;		
		char*		TipoSeguro;		
		char*		TipoPension;	
		char*		CvePension;		
		char*		FechaInicioPen;	
		char*		FechaResolucion;
		int			Delegacion;
		int			TipoMovimiento;
		int			TipoPrestacion;
		int			SemanasCotizadas;
		int			DiagDatamart;
		int			Estadoregistro;
		int			Folio;
		struct  	soap 	*soap;
};

class RespuestaResolucionTotalIssste
{
	public:
		EstadoProceso EstadoProc;
		ResolucionTotalIssste RespResolucionTotalIssste;
		struct soap 	*soap;
};

class ParamAportacionesPosteriores
{
	public:
		char* Nss;
		char* FechaIniPension;
		int iOpcion;
		struct soap		*soap;
};

class AportacionesPosteriores
{
	public:
		double		Aportacion;
		struct  	soap 	*soap;
};

class RespuestaAportacionesPosteriores
{
	public:
		EstadoProceso EstadoProc;
		AportacionesPosteriores RespAportacionesPosteriores;
		struct soap 	*soap;
};

class ParamNssCtaRegimen
{
	public:
		char*	Nss;		
		struct soap 	*soap;
};

class SieforeCtaRegimen
{
	public:
		char*		Nss;	
		int			subCuenta;
		int			Siefore;
		struct  	soap 	*soap;
};

class ListadoSiefore
{
	public:
		SieforeCtaRegimen *__ptrSiefore;
		int		__size;
		struct soap *soap;
};
class RespuestaSieforeCtaRegimen
{
	public:
		EstadoProceso EstadoProc;
		ListadoSiefore *ListaSiefore;
		struct soap 	*soap;
};

class ParamNssCtaSaldoVol
{
	public:
		char*	Nss;		
		struct soap 	*soap;
};

class DatosCtaSaldoVol
{	
	public:
		int		iFolio;
		int		iConsecutivoLote;
		char* 	cNss;
		int		iSiefore;
		int		iSubCuenta;
		char* 	cFechaValor;
		char* 	cFechaConversion;
		char*	iMontoEnPesos; // se maneja en char para los decimales
		char*	iMontoEnAcciones; // se maneja en char para los decimales
		char*	iSaldoAcciones; // se maneja en char para los decimales
		char*  	cFechaSaldo;
		char* 	cUsuario;
		struct  	soap 	*soap;
};

class ListadoSaldoVol
{
	public:
		DatosCtaSaldoVol *__ptrSaldovol;
		int		__size;
		struct soap *soap;
};

class RespuestaCtaSaldoVol
{
	public:
		EstadoProceso EstadoProc;
		ListadoSaldoVol *ListaSaldoVol;
		struct soap 	*soap;
};

class ParamConsultaSaldoVivienda
{
	public:
		char*	sNss;
		char*	sFechaSolicitud;
		char*	sFechaIncioPension;
		struct soap 	*soap;
};

class ConsultarSaldoVivienda
{
	public:
		char*	sNss;
		char*	dMontoAccionViv;
		char*	dMontoPesosViv;
		struct soap 	*soap;
};
class RespuestaConsultaSaldoVivienda
{
	public:
		EstadoProceso EstadoProc;
		ConsultarSaldoVivienda ConsultarSaldoViv;
		struct soap 	*soap;
};

class ParametroAportacionesPost
{
	public:
		int iOpcion;
		char* Nss;
		char* FechaIniPension;
		int iGrupo;
		struct soap		*soap;
};

class ParametroCurpNss
{
	public:
		char* Curp;
		char* Nss;
		struct soap		*soap;
};
/*
class AportacionesPosteriores
{
	public:
		EstadoProceso EstadoProc;
		AportacionesPosteriores RespAportacionesPost;
		struct soap		*soap;
};*/

class SaldoRecuperar
{
	public:
		int   iRespuestaSaldo;
		struct soap *soap;
};

class RespuestaSaldoRecuperar
{
	public:
		EstadoProceso EstadoProc;
		SaldoRecuperar Saldo;
		struct soap *soap;
};

class FechaLiquida
{
	public:
		char* cFechaLiquida;
		struct soap *soap;
};

class RespuestaFechaLiquida
{
	public:
		EstadoProceso EstadoProc;
		FechaLiquida Fecha;
		struct soap *soap;
};

class NumMensualidad
{
	public:
		int iRespuestaMensualidad;
		struct soap *soap;
};

class RespuestaMensualidad
{
	public:
		EstadoProceso EstadoProc;
		NumMensualidad Mensualidad;
		struct soap *soap;
};

int ns1__consultarAfiliado(ParamConsultarAfiliado in, RespConsultarAfiliado *out);
int ns1__consultarSaldos(ParametroNss inParam, SaldoPorSubcuenta *outSaldos);
int ns1__ObtenerFolioServicio(FoliadorServicio inFoliador, FolioServicio *outFolioServ);
int ns1__ValidarConvivenciaMarcaOperativa(ConvivenciaMarcas inParam, RespuestaConvivenciaMarca *outParam);
int ns1__ObtenerSaldoDiarioRetiroParcial(ConsultaSaldoDiarioRetiroParcial inParam, SaldoDiarioRetiroParcial *outParam);
int ns1__ValidarResolucionImss(ParametroNssTipoRetiro inParam, RespuestaValidarResolucionImss *outParam);
int ns1__MontosFechasDatamart(ParametroNssTipoRetiro inParam, RespuestaMontosFechasDatamart *outParam);
int ns1__consultarResolucion(ParametroNssTipoRetiro inParam, RespuestaDatosResolucion *outParam);
int ns1__ConsultarComplementoResolucion(ParametroNssTipoRetiro inParam, RespuestaComplementoResolucion *outParam);
int ns1__ConsultarResolucionTotalImss(ParamResolucionTotalImss inParam, RespuestaResolucionTotalImss *outParam);
int ns1__COperacionesPmg(ParamOperacionPmgImss inParam, RespuestaCOperacionesPmgImss *outParam);
int ns1__movimientosCuentas(ParamMovimientosCuentas inParam, RespuestaMovimientosCuentas *outParam);
int ns1__ConsultarResolucionTotalIssste(ParamResolucionTotalIssste inParam, RespuestaResolucionTotalIssste *outParam);
int ns1__consultarSaldosPmg(ParametroSaldoPmg inParam, SaldoPorSubcuentaPmg *outSaldosPmg);
int ns1__consultarAportaciones(ParamAportacionesPosteriores inParam, RespuestaAportacionesPosteriores *outParam);
int ns1__ConsultarResolucionTotalImssRI(ParamResolucionTotalImssRI inParam, RespuestaResolucionTotalImss *outParam);
int ns1__ConsultarSieforeCtaRegimen(ParamNssCtaRegimen inParam, RespuestaSieforeCtaRegimen *outParam);
int ns1__ConsultarSaldoVol(ParametroNss inParam, RespuestaCtaSaldoVol *outParam);
int ns1__ConsultaSaldoVivienda(ParamConsultaSaldoVivienda inParam, RespuestaConsultaSaldoVivienda *outParam);
int ns1__ConsultaAportacionesPosteriores(ParametroAportacionesPost inParam, RespuestaAportacionesPosteriores *outParam);
int ns1__CConsultarResolusionRetAportaciones(ParametroCurpNss inParam, RespuestaTipoSolicitante *outParam);
int ns1__CConsultarSaldoRecuperar(ParametroNss inParam, RespuestaSaldoRecuperar *outParam);
int ns1__CConsultarFechaLiquida(ParametroNss inParam, RespuestaFechaLiquida *outParam);
int ns1__CConsultarNumMensualidad(ParametroNss inParam, RespuestaMensualidad *outParam);
int ns1__consultarSaldosPenMinGar(ParametroNss inParam, SaldoPorSubcuenta *outSaldos);